/*
 * Copyright (c) Contributors to the Open 3D Engine Project.
 * For complete copyright and license terms please see the LICENSE at the root of this distribution.
 *
 * SPDX-License-Identifier: Apache-2.0 OR MIT
 *
 */

#pragma once

#include "VehicleConfiguration.h"

#include <VehicleDynamics/VehicleDynamicsBus.h>
#include <VehicleDynamics/VehicleDynamicsTypeIds.h>

#include <AzCore/Component/Component.h>
#include <AzCore/Component/TickBus.h>
#include <AzCore/std/containers/vector.h>
#include <AzFramework/Input/Events/InputChannelEventListener.h>
#include <AzFramework/Physics/Common/PhysicsEvents.h>

namespace VehicleDynamics
{
    class VehicleController
        : public AZ::Component
        , private AZ::TickBus::Handler
        , public VehicleDynamicsRequestBus::Handler
        , public AzFramework::InputChannelEventListener
    {
    public:
        AZ_COMPONENT(VehicleController, VehicleControllerTypeId);

        VehicleController() = default;
        VehicleController(const VehicleConfiguration& vehicleControlConfiguration);
        ~VehicleController() = default;

        static void Reflect(AZ::ReflectContext* context);

        //////////////////////////////////////////////////////////////////////////
        // AZ::Component overrides
        void Activate() override;
        void Deactivate() override;
        //////////////////////////////////////////////////////////////////////////

        bool OnInputChannelEventFiltered(const AzFramework::InputChannel& inputChannel) override;
        void OnKeyboardEvent(const AzFramework::InputChannel& inputChannel);

    protected:
        //////////////////////////////////////////////////////////////////////////
        // VehicleDynamicsRequestBus::Handler overrides
        void SetSteeringAngle(float steeringAngle) override;
        void SetAcceleration(float acceleration) override;
        //////////////////////////////////////////////////////////////////////////

        //////////////////////////////////////////////////////////////////////////
        // AZ::TickBus::Handler overrides
        void OnTick(float deltaTime, AZ::ScriptTimePoint time) override;
        //////////////////////////////////////////////////////////////////////////

    private:
        void Initialize();
        void RotateSteeringWheel(float steeringWheelAngle);
        float GetVelocity();
        void UpdateAcceleration();
        void UpdateSteering(const float deltaTime);

        WheelController* GetWheelComponent(const AZ::EntityId& wheelId);

        AzPhysics::SceneEvents::OnSceneSimulationStartHandler m_sceneSimStartHandler;

        float m_maxSteeringAngle{ 0.0f }; //!< Maximal steering angle (based on vehicle parameters) [radians]
        float m_vehicleMass{ 0.0f }; //!< Vehicle's mass set in initialization [kg]

        float m_steeringAngle{ 0.0f }; //!< Current steering angle (based on vehicle parameters) [radians]
        float m_steeringTarget{ 0.0f }; //!< Requested steering angle (based on vehicle parameters) [radians]

        float m_lastInputSteering{ 0.0f };
        float m_inputAcceleration{ 0.0f };

        bool m_initialized{ false };

        AZ::Transform m_steeringWheelInitialTransform{ AZ::Transform::CreateIdentity() };

        VehicleConfiguration m_configuration;

        AZStd::vector<WheelController*> m_frontAxleWheelControllers;
        AZStd::vector<WheelController*> m_rearAxleWheelControllers;
    };

} // namespace VehicleDynamics
