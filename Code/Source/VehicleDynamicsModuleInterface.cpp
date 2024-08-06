/*
 * Copyright (c) Contributors to the Open 3D Engine Project.
 * For complete copyright and license terms please see the LICENSE at the root of this distribution.
 *
 * SPDX-License-Identifier: Apache-2.0 OR MIT
 *
 */

#include "VehicleDynamicsModuleInterface.h"

#include <Clients/VehicleDynamicsSystemComponent.h>
#include <VehicleDynamics/VehicleController.h>
#include <VehicleDynamics/VehicleDynamicsTypeIds.h>
#include <VehicleDynamics/WheelController.h>

#include <AzCore/Memory/Memory.h>

namespace VehicleDynamics
{
    AZ_TYPE_INFO_WITH_NAME_IMPL(VehicleDynamicsModuleInterface, "VehicleDynamicsModuleInterface", VehicleDynamicsModuleInterfaceTypeId);
    AZ_RTTI_NO_TYPE_INFO_IMPL(VehicleDynamicsModuleInterface, AZ::Module);
    AZ_CLASS_ALLOCATOR_IMPL(VehicleDynamicsModuleInterface, AZ::SystemAllocator);

    VehicleDynamicsModuleInterface::VehicleDynamicsModuleInterface()
    {
        // Push results of [MyComponent]::CreateDescriptor() into m_descriptors here.
        // Add ALL components descriptors associated with this gem to m_descriptors.
        // This will associate the AzTypeInfo information for the components with the the SerializeContext, BehaviorContext and EditContext.
        // This happens through the [MyComponent]::Reflect() function.
        m_descriptors.insert(
            m_descriptors.end(),
            {
                VehicleDynamicsSystemComponent::CreateDescriptor(),
                WheelController::CreateDescriptor(),
                VehicleController::CreateDescriptor(),
            });
    }

    AZ::ComponentTypeList VehicleDynamicsModuleInterface::GetRequiredSystemComponents() const
    {
        return AZ::ComponentTypeList{
            azrtti_typeid<VehicleDynamicsSystemComponent>(),
        };
    }
} // namespace VehicleDynamics
