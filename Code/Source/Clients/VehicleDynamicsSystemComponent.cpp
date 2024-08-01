/*
 * Copyright (c) Contributors to the Open 3D Engine Project.
 * For complete copyright and license terms please see the LICENSE at the root of this distribution.
 *
 * SPDX-License-Identifier: Apache-2.0 OR MIT
 *
 */

#include "VehicleDynamicsSystemComponent.h"

#include <VehicleDynamics/VehicleDynamicsTypeIds.h>

#include <AzCore/Serialization/SerializeContext.h>

namespace VehicleDynamics
{
    AZ_COMPONENT_IMPL(VehicleDynamicsSystemComponent, "VehicleDynamicsSystemComponent", VehicleDynamicsSystemComponentTypeId);

    void VehicleDynamicsSystemComponent::Reflect(AZ::ReflectContext* context)
    {
        VehicleDynamicsRequests::Reflect(context);
        if (auto serializeContext = azrtti_cast<AZ::SerializeContext*>(context))
        {
            serializeContext->Class<VehicleDynamicsSystemComponent, AZ::Component>()->Version(0);
        }
    }

    void VehicleDynamicsSystemComponent::GetProvidedServices(AZ::ComponentDescriptor::DependencyArrayType& provided)
    {
        provided.push_back(AZ_CRC_CE("vehicle_dynamics_gemService"));
    }

    void VehicleDynamicsSystemComponent::GetIncompatibleServices(AZ::ComponentDescriptor::DependencyArrayType& incompatible)
    {
        incompatible.push_back(AZ_CRC_CE("vehicle_dynamics_gemService"));
    }

    void VehicleDynamicsSystemComponent::GetRequiredServices([[maybe_unused]] AZ::ComponentDescriptor::DependencyArrayType& required)
    {
    }

    void VehicleDynamicsSystemComponent::GetDependentServices([[maybe_unused]] AZ::ComponentDescriptor::DependencyArrayType& dependent)
    {
    }

} // namespace VehicleDynamics
