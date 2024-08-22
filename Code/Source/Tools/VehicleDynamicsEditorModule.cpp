/*
 * Copyright (c) Contributors to the Open 3D Engine Project.
 * For complete copyright and license terms please see the LICENSE at the root of this distribution.
 *
 * SPDX-License-Identifier: Apache-2.0 OR MIT
 *
 */

#include "VehicleDynamicsEditorSystemComponent.h"

#include <VehicleDynamics/VehicleDynamicsTypeIds.h>
#include <VehicleDynamics/VehicleEditorComponent.h>
#include <VehicleDynamics/WheelEditorComponent.h>
#include <VehicleDynamicsModuleInterface.h>

namespace VehicleDynamics
{
    class VehicleDynamicsEditorModule : public VehicleDynamicsModuleInterface
    {
    public:
        AZ_RTTI(VehicleDynamicsEditorModule, VehicleDynamicsEditorModuleTypeId, VehicleDynamicsModuleInterface);
        AZ_CLASS_ALLOCATOR(VehicleDynamicsEditorModule, AZ::SystemAllocator);

        VehicleDynamicsEditorModule()
        {
            // Push results of [MyComponent]::CreateDescriptor() into m_descriptors here.
            // Add ALL components descriptors associated with this gem to m_descriptors.
            // This will associate the AzTypeInfo information for the components with the the SerializeContext, BehaviorContext and
            // EditContext. This happens through the [MyComponent]::Reflect() function.
            m_descriptors.insert(
                m_descriptors.end(),
                {
                    VehicleDynamicsEditorSystemComponent::CreateDescriptor(),
                    WheelEditorComponent::CreateDescriptor(),
                    VehicleEditorComponent::CreateDescriptor(),
                });
        }

        /**
         * Add required SystemComponents to the SystemEntity.
         * Non-SystemComponents should not be added here
         */
        AZ::ComponentTypeList GetRequiredSystemComponents() const override
        {
            return AZ::ComponentTypeList{
                azrtti_typeid<VehicleDynamicsEditorSystemComponent>(),
            };
        }
    };
} // namespace VehicleDynamics

AZ_DECLARE_MODULE_CLASS(Gem_vehicle_dynamics_gem, VehicleDynamics::VehicleDynamicsEditorModule)
