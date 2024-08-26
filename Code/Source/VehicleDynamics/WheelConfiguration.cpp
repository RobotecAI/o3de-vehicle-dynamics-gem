/*
 * Copyright (c) Contributors to the Open 3D Engine Project.
 * For complete copyright and license terms please see the LICENSE at the root of this distribution.
 *
 * SPDX-License-Identifier: Apache-2.0 OR MIT
 *
 */

#include "WheelConfiguration.h"

#include <AzCore/Serialization/EditContext.h>

namespace VehicleDynamics
{
    void WheelConfiguration::Reflect(AZ::ReflectContext* context)
    {
        if (auto serializeContext = azrtti_cast<AZ::SerializeContext*>(context))
        {
            serializeContext->Class<WheelConfiguration>()
                ->Version(0)
                ->Field("collisionGroupId", &WheelConfiguration::m_collisionGroupId)
                ->Field("wheelVisualEntityId", &WheelConfiguration::m_wheelVisualEntityId)
                ->Field("springStiffness", &WheelConfiguration::m_suspensionStiffness)
                ->Field("suspensionDamping", &WheelConfiguration::m_suspensionDamping)
                ->Field("springRestLength", &WheelConfiguration::m_springRestLength)
                ->Field("springTravel", &WheelConfiguration::m_springTravel)
                ->Field("wheelRadius", &WheelConfiguration::m_wheelRadius)
                ->Field("longitudinalForceCoef", &WheelConfiguration::m_longitudinalForceCoef)
                ->Field("lateralFrictionC1", &WheelConfiguration::m_lateralFrictionC1)
                ->Field("lateralFrictionC2", &WheelConfiguration::m_lateralFrictionC2)
                ->Field("lateralFrictionC3", &WheelConfiguration::m_lateralFrictionC3);

            if (auto editContext = serializeContext->GetEditContext())
            {
                editContext->Class<WheelConfiguration>("Wheel", "Wheel")
                    ->DataElement(
                        AZ::Edit::UIHandlers::Default,
                        &WheelConfiguration::m_collisionGroupId,
                        "Collides with",
                        "Collision group with which the wheel collides")
                    ->DataElement(
                        AZ::Edit::UIHandlers::Default,
                        &WheelConfiguration::m_wheelVisualEntityId,
                        "Wheel visual entity",
                        "Wheel visual entity")
                    ->DataElement(
                        AZ::Edit::UIHandlers::Default,
                        &WheelConfiguration::m_suspensionStiffness,
                        "Suspension stiffness",
                        "Suspension stiffness")
                    ->Attribute(AZ::Edit::Attributes::Min, 0.0f)
                    ->DataElement(
                        AZ::Edit::UIHandlers::Default, &WheelConfiguration::m_suspensionDamping, "Suspension damping", "Suspension damping")
                    ->Attribute(AZ::Edit::Attributes::Min, 0.0f)
                    ->DataElement(
                        AZ::Edit::UIHandlers::Default,
                        &WheelConfiguration::m_springRestLength,
                        "Spring rest length",
                        "Spring rest length [m]")
                    ->Attribute(AZ::Edit::Attributes::Min, 0.0f)
                    ->DataElement(AZ::Edit::UIHandlers::Default, &WheelConfiguration::m_springTravel, "Spring travel", "Spring travel [m]")
                    ->Attribute(AZ::Edit::Attributes::Min, 0.0f)
                    ->DataElement(AZ::Edit::UIHandlers::Default, &WheelConfiguration::m_wheelRadius, "Wheel radius", "Wheel radius [m]")
                    ->DataElement(
                        AZ::Edit::UIHandlers::Default,
                        &WheelConfiguration::m_lateralFrictionC1,
                        "Lateral friction C1",
                        "Lateral friction C1")
                    ->Attribute(AZ::Edit::Attributes::Min, 0.0f)
                    ->DataElement(
                        AZ::Edit::UIHandlers::Default,
                        &WheelConfiguration::m_lateralFrictionC2,
                        "Lateral friction C2",
                        "Lateral friction C2")
                    ->Attribute(AZ::Edit::Attributes::Min, 1.0f)
                    ->DataElement(
                        AZ::Edit::UIHandlers::Default,
                        &WheelConfiguration::m_lateralFrictionC3,
                        "Lateral friction C3",
                        "Lateral friction C3")
                    ->Attribute(AZ::Edit::Attributes::Min, 0.0f)
                    ->DataElement(
                        AZ::Edit::UIHandlers::Default,
                        &WheelConfiguration::m_longitudinalForceCoef,
                        "Longitudinal force coefficient",
                        "Longitudinal force coefficient");
            }
        }
    }

} // namespace VehicleDynamics
