/*
 * Copyright (c) Contributors to the Open 3D Engine Project.
 * For complete copyright and license terms please see the LICENSE at the root of this distribution.
 *
 * SPDX-License-Identifier: Apache-2.0 OR MIT
 *
 */

#include "VehicleConfiguration.h"

#include <AzCore/Serialization/EditContext.h>
#include <AzCore/Serialization/SerializeContext.h>

namespace VehicleDynamics
{
    void VehicleConfiguration::Reflect(AZ::ReflectContext* context)
    {
        if (auto serializeContext = azrtti_cast<AZ::SerializeContext*>(context))
        {
            serializeContext->Class<VehicleConfiguration>()
                ->Version(0)
                ->Field("collisionGroupName", &VehicleConfiguration::m_collisionGroupName)
                ->Field("wheelBase", &VehicleConfiguration::m_wheelbase)
                ->Field("rearTrack", &VehicleConfiguration::m_track)
                ->Field("turnRadius", &VehicleConfiguration::m_turnRadius)
                ->Field("maxAcceleration", &VehicleConfiguration::m_maxAcceleration)
                ->Field("maxVelocity", &VehicleConfiguration::m_maxVelocity)
                ->Field("maxSteeringChangeRate", &VehicleConfiguration::m_maxSteeringChangeRate)
                ->Field("frontAxleWheels", &VehicleConfiguration::m_frontAxleWheelIds)
                ->Field("rearAxleWheels", &VehicleConfiguration::m_rearAxleWheelIds)
                ->Field("frontWheelsDrive", &VehicleConfiguration::m_isFrontWheelDrive)
                ->Field("rearWheelsDrive", &VehicleConfiguration::m_isRearWheelDrive)
                ->Field("enableManualControl", &VehicleConfiguration::m_enableManualControl)
                ->Field("steeringWheel", &VehicleConfiguration::m_steeringWheelId);

            if (auto editContext = serializeContext->GetEditContext())
            {
                editContext->Class<VehicleConfiguration>("VehicleControl", "VehicleControl")
                    ->DataElement(
                        AZ::Edit::UIHandlers::ComboBox,
                        &VehicleConfiguration::m_collisionGroupName,
                        "Collides with",
                        "Collision group with which the wheel collides")
                    ->Attribute(AZ::Edit::Attributes::StringList, &VehicleConfiguration::GetGroupNameList)
                    ->DataElement(
                        AZ::Edit::UIHandlers::Default,
                        &VehicleConfiguration::m_wheelbase,
                        "Wheelbase",
                        "Distance between front and rear axles [m]")
                    ->Attribute(AZ::Edit::Attributes::Min, 0.0f)
                    ->DataElement(AZ::Edit::UIHandlers::Default, &VehicleConfiguration::m_track, "Track", "Distance between wheels [m]")
                    ->Attribute(AZ::Edit::Attributes::Min, 0.0f)
                    ->DataElement(AZ::Edit::UIHandlers::Default, &VehicleConfiguration::m_turnRadius, "Turn radius", "Turn radius [m]")
                    ->Attribute(AZ::Edit::Attributes::Min, 0.0f)
                    ->DataElement(
                        AZ::Edit::UIHandlers::Default,
                        &VehicleConfiguration::m_maxAcceleration,
                        "Maximal acceleration",
                        "Maximal acceleration [m/s^2]")
                    ->Attribute(AZ::Edit::Attributes::Min, 0.0f)
                    ->DataElement(
                        AZ::Edit::UIHandlers::Default, &VehicleConfiguration::m_maxVelocity, "Maximal velocity", "Maximal velocity [m/s]")
                    ->Attribute(AZ::Edit::Attributes::Min, 0.0f)
                    ->DataElement(
                        AZ::Edit::UIHandlers::Default,
                        &VehicleConfiguration::m_maxSteeringChangeRate,
                        "Maximal steering change rate",
                        "Maximal steering change rate [rad/s]")
                    ->Attribute(AZ::Edit::Attributes::Min, 0.0f)
                    ->DataElement(
                        AZ::Edit::UIHandlers::Default,
                        &VehicleConfiguration::m_frontAxleWheelIds,
                        "Front axle wheels",
                        "Front axle wheels (ordered from left to right)")
                    ->DataElement(
                        AZ::Edit::UIHandlers::Default,
                        &VehicleConfiguration::m_rearAxleWheelIds,
                        "Rear axle wheels",
                        "Rear axle wheels (ordered from left to right")
                    ->DataElement(
                        AZ::Edit::UIHandlers::Default,
                        &VehicleConfiguration::m_isFrontWheelDrive,
                        "Front-wheel drive",
                        "Front-wheel drive (use both front-wheel and rear-wheel drive for 4x4)")
                    ->DataElement(
                        AZ::Edit::UIHandlers::Default,
                        &VehicleConfiguration::m_isRearWheelDrive,
                        "Rear-wheel drive",
                        "Rear-wheel drive (use both front-wheel and rear-wheel drive for 4x4)")
                    ->DataElement(
                        AZ::Edit::UIHandlers::Default,
                        &VehicleConfiguration::m_enableManualControl,
                        "Enable manual control",
                        "Enable vehicle control using arrow keys")
                    ->DataElement(
                        AZ::Edit::UIHandlers::Default,
                        &VehicleConfiguration::m_steeringWheelId,
                        "Steering wheel entity",
                        "Steering wheel entity");
            }
        }
    }

    AZStd::vector<AZStd::string> VehicleConfiguration::GetGroupNameList()
    {
        const AzPhysics::CollisionConfiguration& configuration =
            AZ::Interface<AzPhysics::SystemInterface>::Get()->GetConfiguration()->m_collisionConfig;
        const AZStd::vector<AzPhysics::CollisionGroups::Preset>& collisionGroupPresets = configuration.m_collisionGroups.GetPresets();

        AZStd::vector<AZStd::string> groupNames;
        groupNames.reserve(collisionGroupPresets.size());

        for (const auto& preset : collisionGroupPresets)
        {
            groupNames.push_back(preset.m_name);
        }
        return groupNames;
    }

} // namespace VehicleDynamics
