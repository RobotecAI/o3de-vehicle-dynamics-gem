/*
 * Copyright (c) Contributors to the Open 3D Engine Project.
 * For complete copyright and license terms please see the LICENSE at the root of this distribution.
 *
 * SPDX-License-Identifier: Apache-2.0 OR MIT
 *
 */

#include "VehicleDynamicsEditorSystemComponent.h"

#include <VehicleDynamics/VehicleDynamicsTypeIds.h>

#include <AzCore/Serialization/SerializeContext.h>

namespace VehicleDynamics
{
    AZ_COMPONENT_IMPL(
        VehicleDynamicsEditorSystemComponent,
        "VehicleDynamicsEditorSystemComponent",
        VehicleDynamicsEditorSystemComponentTypeId,
        BaseSystemComponent);

    void VehicleDynamicsEditorSystemComponent::Reflect(AZ::ReflectContext* context)
    {
        if (auto serializeContext = azrtti_cast<AZ::SerializeContext*>(context))
        {
            serializeContext->Class<VehicleDynamicsEditorSystemComponent, VehicleDynamicsSystemComponent>()->Version(0);
        }
    }

    VehicleDynamicsEditorSystemComponent::VehicleDynamicsEditorSystemComponent() = default;

    VehicleDynamicsEditorSystemComponent::~VehicleDynamicsEditorSystemComponent() = default;

    void VehicleDynamicsEditorSystemComponent::GetProvidedServices(AZ::ComponentDescriptor::DependencyArrayType& provided)
    {
        BaseSystemComponent::GetProvidedServices(provided);
        provided.push_back(AZ_CRC_CE("vehicle_dynamics_gemEditorService"));
    }

    void VehicleDynamicsEditorSystemComponent::GetIncompatibleServices(AZ::ComponentDescriptor::DependencyArrayType& incompatible)
    {
        BaseSystemComponent::GetIncompatibleServices(incompatible);
        incompatible.push_back(AZ_CRC_CE("vehicle_dynamics_gemEditorService"));
    }

    void VehicleDynamicsEditorSystemComponent::GetRequiredServices([[maybe_unused]] AZ::ComponentDescriptor::DependencyArrayType& required)
    {
        BaseSystemComponent::GetRequiredServices(required);
    }

    void VehicleDynamicsEditorSystemComponent::GetDependentServices(
        [[maybe_unused]] AZ::ComponentDescriptor::DependencyArrayType& dependent)
    {
        BaseSystemComponent::GetDependentServices(dependent);
    }

    void VehicleDynamicsEditorSystemComponent::Activate()
    {
        VehicleDynamicsSystemComponent::Activate();
        AzToolsFramework::EditorEvents::Bus::Handler::BusConnect();
    }

    void VehicleDynamicsEditorSystemComponent::Deactivate()
    {
        AzToolsFramework::EditorEvents::Bus::Handler::BusDisconnect();
        VehicleDynamicsSystemComponent::Deactivate();
    }

} // namespace VehicleDynamics
