/*
 * Copyright (c) Contributors to the Open 3D Engine Project.
 * For complete copyright and license terms please see the LICENSE at the root of this distribution.
 *
 * SPDX-License-Identifier: Apache-2.0 OR MIT
 *
 */

#include "VehicleController.h"

#include <AzCore/Component/ComponentApplicationBus.h>
#include <AzCore/Component/Entity.h>
#include <AzCore/Serialization/EditContext.h>
#include <AzFramework/Input/Devices/Keyboard/InputDeviceKeyboard.h>
#include <AzFramework/Physics/RigidBodyBus.h>

namespace VehicleDynamics
{
    VehicleController::VehicleController(const VehicleConfiguration& vehicleControlConfiguration)
        : m_configuration(vehicleControlConfiguration)
    {
    }

    void VehicleController::Reflect(AZ::ReflectContext* context)
    {
        VehicleConfiguration::Reflect(context);

        if (auto serialize = azrtti_cast<AZ::SerializeContext*>(context))
        {
            serialize->Class<VehicleController, AZ::Component>()->Version(0)->Field("Configuration", &VehicleController::m_configuration);
        }
    }

    void VehicleController::Activate()
    {
        InputChannelEventListener::Connect();
        AZ::TickBus::Handler::BusConnect();
        VehicleDynamicsRequestBus::Handler::BusConnect(GetEntityId());
    }

    void VehicleController::Deactivate()
    {
        InputChannelEventListener::Disconnect();
        AZ::TickBus::Handler::BusDisconnect();
        VehicleDynamicsRequestBus::Handler::BusDisconnect();
    }

    void VehicleController::SetSteeringAngle(const float steeringAngle)
    {
        m_steeringTarget = AZ::GetClamp(steeringAngle, -m_maxSteeringAngle, m_maxSteeringAngle);
    }

    void VehicleController::SetAcceleration(const float acceleration)
    {
        m_inputAcceleration = AZ::GetClamp(acceleration, -m_configuration.m_maxAcceleration, m_configuration.m_maxAcceleration);
    }

    bool VehicleController::OnInputChannelEventFiltered(const AzFramework::InputChannel& inputChannel)
    {
        const AzFramework::InputDeviceId& deviceId = inputChannel.GetInputDevice().GetInputDeviceId();

        if (AzFramework::InputDeviceKeyboard::IsKeyboardDevice(deviceId))
        {
            OnKeyboardEvent(inputChannel);
        }

        return false;
    }

    void VehicleController::OnKeyboardEvent(const AzFramework::InputChannel& inputChannel)
    {
        if (m_configuration.m_enableManualControl == false)
        {
            return;
        }
        const AzFramework::InputChannelId& channelId = inputChannel.GetInputChannelId();

        if (inputChannel.IsStateBegan())
        {
            if (channelId == AzFramework::InputDeviceKeyboard::Key::NavigationArrowDown)
            {
                m_inputAcceleration = -m_configuration.m_maxAcceleration;
            }
            if (channelId == AzFramework::InputDeviceKeyboard::Key::NavigationArrowUp)
            {
                m_inputAcceleration = m_configuration.m_maxAcceleration;
            }
            if (channelId == AzFramework::InputDeviceKeyboard::Key::NavigationArrowLeft)
            {
                m_steeringTarget = m_maxSteeringAngle;
            }
            if (channelId == AzFramework::InputDeviceKeyboard::Key::NavigationArrowRight)
            {
                m_steeringTarget = -m_maxSteeringAngle;
            }
        }
        else if (inputChannel.IsStateEnded())
        {
            if (channelId == AzFramework::InputDeviceKeyboard::Key::NavigationArrowLeft)
            {
                m_steeringTarget = 0.0f;
            }
            if (channelId == AzFramework::InputDeviceKeyboard::Key::NavigationArrowRight)
            {
                m_steeringTarget = 0.0f;
            }
            if (channelId == AzFramework::InputDeviceKeyboard::Key::NavigationArrowUp)
            {
                m_inputAcceleration = 0.0f;
            }
            if (channelId == AzFramework::InputDeviceKeyboard::Key::NavigationArrowDown)
            {
                m_inputAcceleration = 0.0f;
            }
        }
    }

    WheelController* VehicleController::GetWheelComponent(const AZ::EntityId& wheelId)
    {
        AZ::Entity* entity = nullptr;
        AZ::ComponentApplicationBus::BroadcastResult(entity, &AZ::ComponentApplicationRequests::FindEntity, wheelId);
        AZ_Error("VehicleController", entity, "Invalid wheel entity, asset id: %s", wheelId.ToString().c_str());
        if (entity)
        {
            auto wheel = entity->FindComponent<WheelController>();
            AZ_Error("VehicleController", wheel, "Invalid wheel component, asset id: %s", wheelId.ToString().c_str());
            return wheel;
        }
        return nullptr;
    }

    void VehicleController::Initialize()
    {
        for (const auto& wheelId : m_configuration.m_frontAxleWheelIds)
        {
            auto wheel = GetWheelComponent(wheelId);
            AZ_Assert(wheel, "Wheel component is null");
            if (wheel)
            {
                wheel->SetVehicleEntity(GetEntityId());
                m_frontAxleWheelControllers.push_back(wheel);
            }
        }
        AZ_Assert(m_frontAxleWheelControllers.size() == 2, "Vehicle controller supports only two steering wheels.");

        for (const auto& wheelId : m_configuration.m_rearAxleWheelIds)
        {
            auto wheel = GetWheelComponent(wheelId);
            AZ_Assert(wheel, "Wheel component is null");
            if (wheel)
            {
                wheel->SetVehicleEntity(GetEntityId());
                m_rearAxleWheelControllers.push_back(wheel);
            }
        }

        m_maxSteeringAngle = AZStd::atan2(m_configuration.m_wheelbase, m_configuration.m_turnRadius);
        Physics::RigidBodyRequestBus::EventResult(m_vehicleMass, GetEntityId(), &Physics::RigidBodyRequests::GetMass);

        AZ_Assert(m_configuration.m_steeringWheelId.IsValid(), "Steering wheel entity id is not valid");
        if (m_configuration.m_steeringWheelId.IsValid())
        {
            AZ::TransformBus::EventResult(
                m_steeringWheelInitialTransform, m_configuration.m_steeringWheelId, &AZ::TransformBus::Events::GetLocalTM);
        }

        m_initialized = true;
    }

    void VehicleController::RotateSteeringWheel(float steeringWheelAngle)
    {
        if (m_configuration.m_steeringWheelId.IsValid())
        {
            const auto rot = AZ::Quaternion::CreateRotationY(steeringWheelAngle);
            auto tm = m_steeringWheelInitialTransform;
            tm.SetRotation(tm.GetRotation() * rot);
            AZ::TransformBus::Event(m_configuration.m_steeringWheelId, &AZ::TransformBus::Events::SetLocalTM, tm);
        }
    }

    float VehicleController::GetVelocity()
    {
        AZ::Vector3 velocityWorld = AZ::Vector3::CreateZero();

        Physics::RigidBodyRequestBus::EventResult(velocityWorld, GetEntityId(), &Physics::RigidBodyRequests::GetLinearVelocity);
        auto localTransform = AZ::Transform::CreateIdentity();
        AZ::TransformBus::EventResult(localTransform, GetEntityId(), &AZ::TransformBus::Events::GetWorldTM);

        auto velocityLocal = localTransform.GetRotation().GetInverseFast().TransformVector(velocityWorld);

        return velocityLocal.GetX();
    }

    void VehicleController::UpdateAcceleration()
    {
        const float velocity = GetVelocity();
        float acceleration = m_inputAcceleration;
        if (AZ::GetSign(m_inputAcceleration) == AZ::GetSign(velocity))
        {
            if (AZStd::abs(velocity) >= m_configuration.m_maxVelocity)
            {
                acceleration = 0;
            }
        }
        // Dividing force between wheels
        float forceMultiplier = 1.0f;
        if (m_configuration.m_isFrontWheelDrive)
        {
            forceMultiplier *= 0.5f;
        }
        if (m_configuration.m_isRearWheelDrive)
        {
            forceMultiplier *= 0.5f;
        }

        const float driveForce = acceleration * m_vehicleMass * forceMultiplier;
        if (m_configuration.m_isFrontWheelDrive)
        {
            for (auto wheel : m_frontAxleWheelControllers)
            {
                wheel->SetDriveForce(driveForce);
            }
        }
        if (m_configuration.m_isRearWheelDrive)
        {
            for (auto wheel : m_rearAxleWheelControllers)
            {
                wheel->SetDriveForce(driveForce);
            }
        }
    }

    void VehicleController::UpdateSteering(const float deltaTime)
    {
        if (m_steeringAngle != m_steeringTarget)
        {
            const float diff = m_steeringTarget - m_steeringAngle;
            if (AZStd::abs(diff) > m_configuration.m_maxSteeringChangeRate * deltaTime * 200.0f)
            {
                m_steeringAngle += AZ::GetSign(diff) * m_configuration.m_maxSteeringChangeRate * deltaTime * 200.0f;
            }
            else
            {
                m_steeringAngle = m_steeringTarget;
            }
        }
        if (m_steeringAngle != m_lastInputSteering)
        {
            float ackermannLeft = 0.0f;
            float ackermannRight = 0.0f;

            const float steeringAngleCoef = m_steeringAngle / m_maxSteeringAngle;
            if (steeringAngleCoef > 0.0f)
            {
                ackermannLeft = -AZStd::atan2(m_configuration.m_wheelbase, m_configuration.m_turnRadius + m_configuration.m_track / 2.0f) *
                    steeringAngleCoef;
                ackermannRight = -AZStd::atan2(m_configuration.m_wheelbase, m_configuration.m_turnRadius - m_configuration.m_track / 2.0f) *
                    steeringAngleCoef;
            }
            else if (steeringAngleCoef < 0.0f)
            {
                ackermannLeft = -AZStd::atan2(m_configuration.m_wheelbase, m_configuration.m_turnRadius - m_configuration.m_track / 2.0f) *
                    steeringAngleCoef;
                ackermannRight = -AZStd::atan2(m_configuration.m_wheelbase, m_configuration.m_turnRadius + m_configuration.m_track / 2.0f) *
                    steeringAngleCoef;
            }

            m_frontAxleWheelControllers.front()->SetSteeringAngle(ackermannLeft);
            m_frontAxleWheelControllers.back()->SetSteeringAngle(ackermannRight);

            constexpr float steeringWheelTransmission = AZ::Constants::Pi * 1.3f;
            RotateSteeringWheel(steeringAngleCoef * steeringWheelTransmission);
        }

        m_lastInputSteering = m_steeringAngle;
    }

    void VehicleController::OnTick(float deltaTime, [[maybe_unused]] AZ::ScriptTimePoint time)
    {
        if (!m_initialized)
        {
            Initialize();
        }

        UpdateSteering(deltaTime);
        UpdateAcceleration();
    }

} // namespace VehicleDynamics
