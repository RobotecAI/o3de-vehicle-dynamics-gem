/*
 * Copyright (c) Contributors to the Open 3D Engine Project.
 * For complete copyright and license terms please see the LICENSE at the root of this distribution.
 *
 * SPDX-License-Identifier: Apache-2.0 OR MIT
 *
 */

#pragma once

#include <VehicleDynamics/VehicleDynamicsTypeIds.h>

#include <AzCore/Component/EntityId.h>
#include <AzCore/RTTI/RTTI.h>
#include <AzCore/Serialization/EditContext.h>
#include <AzCore/Serialization/SerializeContext.h>
#include <AzFramework/Physics/Collision/CollisionGroups.h>
#include <AzFramework/Physics/PhysicsSystem.h>

namespace VehicleDynamics
{
    struct WheelConfiguration
    {
        AZ_TYPE_INFO(WheelConfiguration, WheelConfigurationTypeId);

        static void Reflect(AZ::ReflectContext* context);

        AZ::EntityId m_wheelVisualEntityId;

        float m_suspensionStiffness{ 10000.0f };
        float m_suspensionDamping{ 1000.0f };
        float m_springRestLength{ 0.5f }; // [m]
        float m_springTravel{ 0.2f }; // [m]

        float m_wheelRadius{ 0.35f }; // [m]

        float m_lateralFrictionC1{ 2.0f };
        float m_lateralFrictionC2{ 1.7f };
        float m_lateralFrictionC3{ 2.0f };
        float m_longitudinalForceCoef{ 1.0f };

        AZStd::string m_collisionGroupName = "All"; //!< Name of the collision group with which the wheel collides. Kept as a string due to
                                                    //!< different collision groups id in different projects. This value is not serialized.
    };
} // namespace VehicleDynamics
