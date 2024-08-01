/*
 * Copyright (c) Contributors to the Open 3D Engine Project.
 * For complete copyright and license terms please see the LICENSE at the root of this distribution.
 *
 * SPDX-License-Identifier: Apache-2.0 OR MIT
 *
 */

#pragma once

#include <Clients/VehicleDynamicsSystemComponent.h>

#include <AzToolsFramework/API/ToolsApplicationAPI.h>

namespace VehicleDynamics
{
    /// System component for VehicleDynamics editor
    class VehicleDynamicsEditorSystemComponent
        : public VehicleDynamicsSystemComponent
        , protected AzToolsFramework::EditorEvents::Bus::Handler
    {
        using BaseSystemComponent = VehicleDynamicsSystemComponent;

    public:
        AZ_COMPONENT_DECL(VehicleDynamicsEditorSystemComponent);

        static void Reflect(AZ::ReflectContext* context);

        VehicleDynamicsEditorSystemComponent();
        ~VehicleDynamicsEditorSystemComponent();

    private:
        static void GetProvidedServices(AZ::ComponentDescriptor::DependencyArrayType& provided);
        static void GetIncompatibleServices(AZ::ComponentDescriptor::DependencyArrayType& incompatible);
        static void GetRequiredServices(AZ::ComponentDescriptor::DependencyArrayType& required);
        static void GetDependentServices(AZ::ComponentDescriptor::DependencyArrayType& dependent);

        // AZ::Component
        void Activate() override;
        void Deactivate() override;
    };
} // namespace VehicleDynamics
