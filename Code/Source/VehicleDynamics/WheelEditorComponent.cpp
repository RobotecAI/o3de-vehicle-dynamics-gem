/*
 * Copyright (c) Contributors to the Open 3D Engine Project.
 * For complete copyright and license terms please see the LICENSE at the root of this distribution.
 *
 * SPDX-License-Identifier: Apache-2.0 OR MIT
 *
 */

#include "WheelEditorComponent.h"
#include "WheelController.h"

#include <AzCore/Serialization/EditContext.h>

namespace VehicleDynamics
{
    void WheelEditorComponent::Reflect(AZ::ReflectContext* context)
    {
        if (AZ::SerializeContext* serialize = azrtti_cast<AZ::SerializeContext*>(context))
        {
            serialize->Class<WheelEditorComponent, AzToolsFramework::Components::EditorComponentBase>()->Version(0)->Field(
                "Configuration", &WheelEditorComponent::m_configuration);

            if (AZ::EditContext* editContext = serialize->GetEditContext())
            {
                editContext->Class<WheelEditorComponent>("WheelController", "WheelController")
                    ->ClassElement(AZ::Edit::ClassElements::EditorData, "WheelController")
                    ->Attribute(AZ::Edit::Attributes::AppearsInAddComponentMenu, AZ_CRC_CE("Game"))
                    ->Attribute(AZ::Edit::Attributes::Category, "VehicleDynamics")
                    ->DataElement(
                        AZ::Edit::UIHandlers::Default,
                        &WheelEditorComponent::m_configuration,
                        "Wheel controller configuration",
                        "Wheel controller configuration");
            }
        }
    }

    void WheelEditorComponent::GetProvidedServices(AZ::ComponentDescriptor::DependencyArrayType& provided)
    {
        provided.push_back(AZ_CRC_CE("WheelControllerService"));
    }

    void WheelEditorComponent::GetIncompatibleServices(AZ::ComponentDescriptor::DependencyArrayType& incompatible)
    {
        incompatible.push_back(AZ_CRC_CE("WheelControllerService"));
    }

    void WheelEditorComponent::Activate()
    {
        AzToolsFramework::Components::EditorComponentBase::Activate();
        AzFramework::EntityDebugDisplayEventBus::Handler::BusConnect(GetEntityId());
    }

    void WheelEditorComponent::Deactivate()
    {
        AzFramework::EntityDebugDisplayEventBus::Handler::BusDisconnect();
        AzToolsFramework::Components::EditorComponentBase::Deactivate();
    }

    void WheelEditorComponent::BuildGameEntity(AZ::Entity* gameEntity)
    {
        gameEntity->CreateComponent<WheelController>(m_configuration);
    }

    void WheelEditorComponent::DisplayEntityViewport(
        [[maybe_unused]] const AzFramework::ViewportInfo& viewportInfo, AzFramework::DebugDisplayRequests& debugDisplay)
    {
        AZ::Transform transform = GetEntity()->GetTransform()->GetWorldTM();

        const AZ::u32 stateBefore = debugDisplay.GetState();
        debugDisplay.CullOff();
        debugDisplay.DepthTestOff();
        debugDisplay.SetLineWidth(2.0f);

        debugDisplay.PushMatrix(transform);

        const AZ::Vector3 attachmentPoint = AZ::Vector3::CreateZero();
        const AZ::Vector3 springAxis = AZ::Vector3::CreateAxisZ();
        const AZ::Vector3 wheelAxis = AZ::Vector3::CreateAxisY();
        const AZ::Vector3 springMin = AZ::Vector3::CreateAxisZ(-m_configuration.m_springRestLength + m_configuration.m_springTravel);
        const AZ::Vector3 springMax = AZ::Vector3::CreateAxisZ(-m_configuration.m_springRestLength - m_configuration.m_springTravel);
        const AZ::Vector3 springMiddle = (springMin + springMax) / 2.0f;
        // draw the spring
        debugDisplay.SetColor(AZ::Colors::White);
        debugDisplay.DrawWireSphere(attachmentPoint, 0.2f);
        debugDisplay.DrawWireCylinder(
            springMin / 2.0f, springAxis, 0.2f, m_configuration.m_springRestLength - m_configuration.m_springTravel);
        debugDisplay.SetColor(AZ::Colors::Yellow);
        debugDisplay.DrawWireCylinder((springMiddle - attachmentPoint) / 2.0f, springAxis, 0.1f, m_configuration.m_springRestLength);

        // draw the wheel
        debugDisplay.SetColor(AZ::Colors::Gray);
        debugDisplay.DrawWireCylinder(springMiddle, wheelAxis, m_configuration.m_wheelRadius, 0.2f);

        // draw wheel travel
        const AZStd::array<AZ::Vector3, 2> arrowsOffset = { AZ::Vector3::CreateAxisX(1.1f * m_configuration.m_wheelRadius),
                                                            AZ::Vector3::CreateAxisX(-1.1f * m_configuration.m_wheelRadius) };
        debugDisplay.SetColor(AZ::Colors::Red);
        for (const auto offset : arrowsOffset)
        {
            debugDisplay.DrawArrow(springMin + offset, springMax + offset, 0.35f, true);
        }
        debugDisplay.PopMatrix();
        debugDisplay.SetState(stateBefore);
    }

} // namespace VehicleDynamics
