/*
 * Copyright (c) Contributors to the Open 3D Engine Project.
 * For complete copyright and license terms please see the LICENSE at the root of this distribution.
 *
 * SPDX-License-Identifier: Apache-2.0 OR MIT
 *
 */

#include <VehicleDynamics/VehicleDynamicsBus.h>

#include <AzCore/RTTI/BehaviorContext.h>

namespace VehicleDynamics
{
    void VehicleDynamicsRequests::Reflect(AZ::ReflectContext* context)
    {
        if (AZ::BehaviorContext* behaviorContext = azrtti_cast<AZ::BehaviorContext*>(context))
        {
            behaviorContext->EBus<VehicleDynamicsRequestBus>("VehicleDynamicsRequestBus")
                ->Attribute(AZ::Script::Attributes::Category, "VehicleDynamics")
                ->Attribute(AZ::Script::Attributes::Scope, AZ::Script::Attributes::ScopeFlags::Common)
                ->Attribute(AZ::Script::Attributes::Module, "VehicleDynamics")
                ->Event("SetSteeringAngle", &VehicleDynamicsRequestBus::Events::SetSteeringAngle)
                ->Event("SetAcceleration", &VehicleDynamicsRequestBus::Events::SetAcceleration);
        }
    }
} // namespace VehicleDynamics
