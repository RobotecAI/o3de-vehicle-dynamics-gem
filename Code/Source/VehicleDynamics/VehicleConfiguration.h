/*
 * Copyright (c) Contributors to the Open 3D Engine Project.
 * For complete copyright and license terms please see the LICENSE at the root of this distribution.
 *
 * SPDX-License-Identifier: Apache-2.0 OR MIT
 *
 */

#pragma once

#include "VehicleDynamics/WheelController.h"

#include <VehicleDynamics/VehicleDynamicsTypeIds.h>

#include <AzCore/Component/EntityId.h>
#include <AzCore/RTTI/RTTI.h>
#include <AzCore/Serialization/SerializeContext.h>

namespace VehicleDynamics
{
    struct VehicleConfiguration
    {
        AZ_TYPE_INFO(VehicleConfiguration, VehicleConfigurationTypeId);
        static void Reflect(AZ::ReflectContext* context);

        AZStd::string m_collisionGroupName = "All"; //!< Name of the collision group with which the wheel collides. Kept as a string due to
                                                    //!< different collision groups id in different projects.

        float m_wheelbase{ 2.7f }; // [m]
        float m_track{ 1.5f }; // [m]
        float m_turnRadius{ 10.0f }; // [m]
        float m_maxAcceleration{ 4.0f }; // [m/s^2]
        float m_maxVelocity{ 40.0f }; // [m/s]
        float m_maxSteeringChangeRate{ 0.01f }; // [rad/s]

        bool m_isFrontWheelDrive{ true };
        bool m_isRearWheelDrive{ false };

        bool m_enableManualControl{ true };

        AZStd::vector<AZ::EntityId> m_frontAxleWheelIds;
        AZStd::vector<AZ::EntityId> m_rearAxleWheelIds;

        AZ::EntityId m_steeringWheelId;

    private:
        AZStd::vector<AZStd::string> GetGroupNameList();
    };
} // namespace VehicleDynamics
