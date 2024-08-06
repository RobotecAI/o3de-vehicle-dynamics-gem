/*
 * Copyright (c) Contributors to the Open 3D Engine Project.
 * For complete copyright and license terms please see the LICENSE at the root of this distribution.
 *
 * SPDX-License-Identifier: Apache-2.0 OR MIT
 *
 */

#pragma once

#include "VehicleConfiguration.h"

#include <VehicleDynamics/VehicleDynamicsTypeIds.h>

#include <AzToolsFramework/ToolsComponents/EditorComponentBase.h>

namespace VehicleDynamics
{
    class VehicleEditorComponent : public AzToolsFramework::Components::EditorComponentBase
    {
    public:
        AZ_EDITOR_COMPONENT(VehicleEditorComponent, VehicleEditorComponentTypeId);

        static void Reflect(AZ::ReflectContext* context);

        static void GetProvidedServices(AZ::ComponentDescriptor::DependencyArrayType& provided);
        static void GetIncompatibleServices(AZ::ComponentDescriptor::DependencyArrayType& incompatible);

        // AzToolsFramework::Components::EditorComponentBase overrides
        void Activate() override;
        void Deactivate() override;
        void BuildGameEntity(AZ::Entity* gameEntity) override;

    private:
        VehicleConfiguration m_configuration;
    };

} // namespace VehicleDynamics
