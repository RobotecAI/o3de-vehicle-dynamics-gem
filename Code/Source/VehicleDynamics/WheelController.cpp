/*
 * Copyright (c) Contributors to the Open 3D Engine Project.
 * For complete copyright and license terms please see the LICENSE at the root of this distribution.
 *
 * SPDX-License-Identifier: Apache-2.0 OR MIT
 *
 */

#include "WheelController.h"

#include <AzCore/Math/MathUtils.h>
#include <AzFramework/Physics/Collision/CollisionGroups.h>
#include <AzFramework/Physics/CollisionBus.h>
#include <AzFramework/Physics/PhysicsScene.h>
#include <AzFramework/Physics/PhysicsSystem.h>
#include <AzFramework/Physics/RigidBodyBus.h>

namespace VehicleDynamics
{
    WheelController::WheelController(const WheelConfiguration& wheelConfiguration)
        : m_configuration(wheelConfiguration)
    {
    }

    void WheelController::Reflect(AZ::ReflectContext* context)
    {
        WheelConfiguration::Reflect(context);

        if (AZ::SerializeContext* serialize = azrtti_cast<AZ::SerializeContext*>(context))
        {
            serialize->Class<WheelController, AZ::Component>()->Version(0)->Field("Configuration", &WheelController::m_configuration);
        }
    }

    void WheelController::SetDriveForce(const float force)
    {
        m_driveForce = force;
    }

    void WheelController::SetSteeringAngle(const float steeringAngle)
    {
        m_steeringAngle = steeringAngle;
    }

    void WheelController::SetVehicleEntity(const AZ::EntityId& vehicleEntityId)
    {
        m_vehicleEntityId = vehicleEntityId;
    }

    void WheelController::Activate()
    {
        m_springLength = m_configuration.m_springRestLength;
        m_springMaxLength = m_configuration.m_springRestLength + m_configuration.m_springTravel;
        m_springMinLength = m_configuration.m_springRestLength - m_configuration.m_springTravel;

        AZ::Transform tm = AZ::Transform::CreateIdentity();
        AZ::TransformBus::EventResult(tm, GetEntityId(), &AZ::TransformBus::Events::GetLocalTM);
        m_wheelInitialRotation = tm.GetRotation();

        AZ::SystemTickBus::QueueFunction(
            [this]()
            {
                AZ_Error("WheelController", AZ::Interface<AzPhysics::SystemInterface>::Get(), "No physics system");
                auto sceneInterface = AZ::Interface<AzPhysics::SceneInterface>::Get();
                AZ_Assert(sceneInterface, "No scene interface");
                AZ_Error("WheelController", sceneInterface, "No scene interface");
                auto defaultSceneHandle = sceneInterface->GetSceneHandle(AzPhysics::DefaultPhysicsSceneName);
                AZ_Error("WheelController", defaultSceneHandle != AzPhysics::InvalidSceneHandle, "Invalid default physics scene handle");

                m_sceneSimStartHandler = AzPhysics::SceneEvents::OnSceneSimulationStartHandler(
                    [this]([[maybe_unused]] AzPhysics::SceneHandle sceneHandle, float fixedDeltaTime)
                    {
                        this->OnPhysicsTimestep(fixedDeltaTime);
                    },
                    aznumeric_cast<int32_t>(AzPhysics::SceneEvents::PhysicsStartFinishSimulationPriority::Components));
                sceneInterface->RegisterSceneSimulationStartHandler(defaultSceneHandle, m_sceneSimStartHandler);
            });

        AZ::TickBus::Handler::BusConnect();
    }

    void WheelController::Deactivate()
    {
        AZ::TickBus::Handler::BusDisconnect();
    }

    void WheelController::OnPhysicsTimestep(const float fixedDeltaTime)
    {
        if (m_vehicleEntityId.IsValid())
        {
            const auto raycastHit = RayCast();
            UpdateSuspension(fixedDeltaTime);
            if (raycastHit)
            {
                UpdateFrictionForces(fixedDeltaTime);
            }
        }
    }

    bool WheelController::RayCast()
    {
        AZ::TransformBus::EventResult(m_wheelWorldTM, GetEntityId(), &AZ::TransformBus::Events::GetWorldTM);
        const auto downDir = m_wheelWorldTM.GetRotation().TransformVector(AZ::Vector3::CreateAxisZ(-1.0f));

        AzPhysics::CollisionGroup collisionGroup;
        Physics::CollisionRequestBus::BroadcastResult(
            collisionGroup, &Physics::CollisionRequestBus::Events::GetCollisionGroupById, m_configuration.m_collisionGroupId);

        AzPhysics::RayCastRequest request = AzPhysics::RayCastRequest();
        request.m_collisionGroup = collisionGroup;
        request.m_start = m_wheelWorldTM.GetTranslation();
        request.m_direction = downDir;
        request.m_distance = m_springMaxLength + m_configuration.m_wheelRadius;
        request.m_reportMultipleHits = false;

        auto* sceneInterface = AZ::Interface<AzPhysics::SceneInterface>::Get();
        auto requestResult = sceneInterface->QueryScene(sceneInterface->GetSceneHandle(AzPhysics::DefaultPhysicsSceneName), &request);

        m_raycastHitPosition = AZ::Vector3::CreateZero();
        m_raycastHitDistance = m_springMaxLength + m_configuration.m_wheelRadius;
        if (!requestResult.m_hits.empty())
        {
            const auto hit = requestResult.m_hits.front();
            m_raycastHitDistance = hit.m_distance;
            m_raycastHitPosition = hit.m_position;
            return true;
        }

        return false;
    }

    void WheelController::UpdateSuspension(const float deltaTime)
    {
        m_lastSpringLength = m_springLength;
        m_springLength = m_raycastHitDistance - m_configuration.m_wheelRadius;
        m_springLength = AZ::GetClamp(m_springLength, m_springMinLength, m_springMaxLength);

        const float springForce = m_configuration.m_suspensionStiffness * (m_configuration.m_springRestLength - m_springLength);
        const float springVelocity = (m_springLength - m_lastSpringLength) / deltaTime;
        const float dampingForce = m_configuration.m_suspensionDamping * springVelocity;
        m_groundForce = AZStd::max(springForce - dampingForce, 0.0f);

        const auto groundForceImpulse = m_groundForce * deltaTime;
        const auto forceImpulse = m_wheelWorldTM.GetRotation().TransformVector(AZ::Vector3::CreateAxisZ(groundForceImpulse));
        ApplyLinearForceImpulse(forceImpulse, m_wheelWorldTM.GetTranslation());
    }

    void WheelController::UpdateFrictionForces(const float deltaTime)
    {
        AZ::Vector3 velocityAtHit = AZ::Vector3::CreateZero();

        Physics::RigidBodyRequestBus::EventResult(
            velocityAtHit, m_vehicleEntityId, &Physics::RigidBodyRequests::GetLinearVelocityAtWorldPoint, m_raycastHitPosition);

        m_velocityAtHitLocal = m_wheelWorldTM.GetRotation().GetInverseFast().TransformVector(velocityAtHit);

        constexpr float groundForceMultiplier = 0.0003f; // should be in range [0, 1]
        const float groundForceCoef = m_groundForce * groundForceMultiplier;

        float longitudinalForce = m_driveForce * groundForceCoef * deltaTime * m_configuration.m_longitudinalForceCoef;

        // low velocity damping
        if (AZ::IsClose(m_driveForce, 0.0f))
        {
            const float dampingCoeff = 1.0f / (6.0f * AZ::Abs(m_velocityAtHitLocal.GetX()) + 1.0f);
            longitudinalForce -=
                m_velocityAtHitLocal.GetX() * deltaTime * 1000.0f * dampingCoeff + AZ::GetSign(m_velocityAtHitLocal.GetX()) * 1.5f;
        }

        // to minimize rollovers at higher velocity, we need to reduce lateral force at higher velocity
        const float longitudinalVelocityCoeff = 1.0f / (0.2f * AZ::Abs(m_velocityAtHitLocal.GetX()) + 1.0f);

        float lateralCoeff = abs(
            m_configuration.m_lateralFrictionC1 *
            AZ::Sin(
                m_configuration.m_lateralFrictionC2 * AZ::Atan(m_configuration.m_lateralFrictionC3 * abs(m_velocityAtHitLocal.GetY()))));
        lateralCoeff += 0.5f; // adding small value to prevent sliding when velocity is zero

        const float lateralForce = -lateralCoeff * m_velocityAtHitLocal.GetY() * m_groundForce * deltaTime * longitudinalVelocityCoeff;

        const auto forceImpulse = m_wheelWorldTM.GetRotation().TransformVector(AZ::Vector3(longitudinalForce, lateralForce, 0.0f));
        ApplyLinearForceImpulse(forceImpulse, m_raycastHitPosition);
    }

    void WheelController::UpdateVisual(const float deltaTime)
    {
        AZ::Entity* entity = nullptr;
        AZ::ComponentApplicationBus::BroadcastResult(
            entity, &AZ::ComponentApplicationRequests::FindEntity, m_configuration.m_wheelVisualEntityId);
        if (entity)
        {
            auto visualTM = AZ::Transform::CreateIdentity();
            AZ::TransformBus::EventResult(visualTM, entity->GetId(), &AZ::TransformBus::Events::GetLocalTM);
            visualTM.SetTranslation(AZ::Vector3::CreateAxisZ(-m_springLength));
            m_spinAngle += m_velocityAtHitLocal.GetX() * deltaTime / m_configuration.m_wheelRadius;
            if (m_spinAngle >= AZ::Constants::TwoPi)
            {
                m_spinAngle -= AZ::Constants::TwoPi;
            }
            if (m_spinAngle <= 0.0f)
            {
                m_spinAngle += AZ::Constants::TwoPi;
            }
            visualTM.SetRotation(AZ::Quaternion::CreateRotationY(m_spinAngle));
            AZ::TransformBus::Event(entity->GetId(), &AZ::TransformBus::Events::SetLocalTM, visualTM);
        }
    }

    void WheelController::UpdateSteering([[maybe_unused]] float deltaTime)
    {
        const auto newRotation = m_wheelInitialRotation * AZ::Quaternion::CreateRotationZ(-m_steeringAngle);
        AZ::Transform tm = AZ::Transform::CreateIdentity();
        AZ::TransformBus::EventResult(tm, GetEntityId(), &AZ::TransformBus::Events::GetLocalTM);
        tm.SetRotation(newRotation);
        AZ::TransformBus::Event(GetEntityId(), &AZ::TransformBus::Events::SetLocalTM, tm);
    }

    void WheelController::ApplyLinearForceImpulse(const AZ::Vector3& impulse, const AZ::Vector3& worldPoint)
    {
        Physics::RigidBodyRequestBus::Event(
            m_vehicleEntityId, &Physics::RigidBodyRequests::ApplyLinearImpulseAtWorldPoint, impulse, worldPoint);
    }

    void WheelController::OnTick(float deltaTime, [[maybe_unused]] AZ::ScriptTimePoint time)
    {
        UpdateSteering(deltaTime);
        UpdateVisual(deltaTime);
    }

} // namespace VehicleDynamics
