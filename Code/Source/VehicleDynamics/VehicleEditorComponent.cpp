/*
 * Copyright (c) Contributors to the Open 3D Engine Project.
 * For complete copyright and license terms please see the LICENSE at the root of this distribution.
 *
 * SPDX-License-Identifier: Apache-2.0 OR MIT
 *
 */

#include "VehicleEditorComponent.h"
#include "VehicleController.h"

#include <AzCore/Serialization/EditContext.h>
#include <AzCore/Serialization/SerializeContext.h>
#include <AzToolsFramework/Viewport/ViewportMessages.h>

namespace VehicleDynamics
{
    void VehicleEditorComponent::Reflect(AZ::ReflectContext* context)
    {
        if (AZ::SerializeContext* serialize = azrtti_cast<AZ::SerializeContext*>(context))
        {
            serialize->Class<VehicleEditorComponent, AzToolsFramework::Components::EditorComponentBase>()->Version(0)->Field(
                "Configuration", &VehicleEditorComponent::m_configuration);

            if (AZ::EditContext* editContext = serialize->GetEditContext())
            {
                editContext->Class<VehicleEditorComponent>("VehicleController", "VehicleController")
                    ->ClassElement(AZ::Edit::ClassElements::EditorData, "VehicleController")
                    ->Attribute(AZ::Edit::Attributes::AppearsInAddComponentMenu, AZ_CRC_CE("Game"))
                    ->Attribute(AZ::Edit::Attributes::Category, "VehicleDynamics")
                    ->DataElement(
                        AZ::Edit::UIHandlers::Default,
                        &VehicleEditorComponent::m_configuration,
                        "Vehicle controller configuration",
                        "Vehicle controller configuration");
            }
        }
    }

    void VehicleEditorComponent::GetProvidedServices(AZ::ComponentDescriptor::DependencyArrayType& provided)
    {
        provided.push_back(AZ_CRC_CE("VehicleControllerService"));
    }

    void VehicleEditorComponent::GetIncompatibleServices(AZ::ComponentDescriptor::DependencyArrayType& incompatible)
    {
        incompatible.push_back(AZ_CRC_CE("VehicleControllerService"));
    }

    void VehicleEditorComponent::Activate()
    {
        AzToolsFramework::Components::EditorComponentBase::Activate();
    }

    void VehicleEditorComponent::Deactivate()
    {
        AzToolsFramework::Components::EditorComponentBase::Deactivate();
    }

    void VehicleEditorComponent::BuildGameEntity(AZ::Entity* gameEntity)
    {
        gameEntity->CreateComponent<VehicleController>(m_configuration);
    }

} // namespace VehicleDynamics
