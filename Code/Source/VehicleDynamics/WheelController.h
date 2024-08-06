/*
 * Copyright (c) Contributors to the Open 3D Engine Project.
 * For complete copyright and license terms please see the LICENSE at the root of this distribution.
 *
 * SPDX-License-Identifier: Apache-2.0 OR MIT
 *
 */

#pragma once

#include "WheelConfiguration.h"

#include <VehicleDynamics/VehicleDynamicsTypeIds.h>

#include <AzCore/Component/Component.h>
#include <AzCore/Component/TickBus.h>
#include <AzCore/Component/TransformBus.h>
#include <AzCore/Math/Quaternion.h>
#include <AzFramework/Physics/Common/PhysicsEvents.h>

namespace VehicleDynamics
{
    class WheelController
        : public AZ::Component
        , private AZ::TickBus::Handler
    {
    public:
        AZ_COMPONENT(WheelController, WheelControllerTypeId);

        WheelController() = default;
        WheelController(const WheelConfiguration& wheelConfiguration);
        ~WheelController() override = default;

        static void Reflect(AZ::ReflectContext* context);

        //////////////////////////////////////////////////////////////////////////
        // AZ::Component overrides
        void Activate() override;
        void Deactivate() override;
        //////////////////////////////////////////////////////////////////////////

        void SetDriveForce(const float force);
        void SetSteeringAngle(const float steeringAngle);
        void SetVehicleEntity(const AZ::EntityId& vehicleEntityId);

    protected:
        //////////////////////////////////////////////////////////////////////////
        // AZ::TickBus::Handler overrides
        void OnTick(float deltaTime, AZ::ScriptTimePoint time) override;
        //////////////////////////////////////////////////////////////////////////

    private:
        void ApplyLinearForceImpulse(const AZ::Vector3& impulse, const AZ::Vector3& worldPoint);
        void OnPhysicsTimestep(const float fixedDeltaTime);
        bool RayCast();
        void UpdateSuspension(const float deltaTime);
        void UpdateFrictionForces(const float deltaTime);
        void UpdateVisual(const float deltaTime);
        void UpdateSteering(const float deltaTime);

        WheelConfiguration m_configuration; //!< Wheel's configuration
        AZ::EntityId m_vehicleEntityId; //!< Entity id of the vehicle (needed for vehicle's speed)

        AzPhysics::SceneEvents::OnSceneSimulationStartHandler m_sceneSimStartHandler; //!< Handler that updates physics of the wheel

        float m_raycastHitDistance{ 0.0f }; //!< Current distance between the suspension mount and the ground [m]
        AZ::Vector3 m_raycastHitPosition{ AZ::Vector3::CreateZero() }; //!< Current point of contact between the wheel and the ground
        AZ::Vector3 m_velocityAtHitLocal{
            AZ::Vector3::CreateZero()
        }; //!< Current velocity at point of contact between the wheel and the ground

        float m_springLength{ 0.0f }; //!< Current suspension length [m]
        float m_lastSpringLength{ 0.0f }; //!< Previous suspension length [m]
        float m_springMaxLength{ 0.0f }; //!< Maximal suspension length (based on configuration) [m]
        float m_springMinLength{ 0.0f }; //!< Minimal suspension length (based on configuration) [m]

        float m_driveForce{ 0.0f };
        float m_groundForce{ 0.0f };

        float m_steeringAngle{ 0.0f }; //!< Current steering angle of the wheel (visualization only) [radians]
        float m_spinAngle{ 0.0f }; //!< Current wheel rotation (visualization only) [radians]

        AZ::Quaternion m_wheelInitialRotation{
            AZ::Quaternion::CreateIdentity()
        }; //!< Wheel's suspension mount initial rotation (visualization only)
        AZ::Transform m_wheelWorldTM{ AZ::Transform::CreateIdentity() }; //!< Wheel's suspension mount current position (visualization only)
    };

} // namespace VehicleDynamics
