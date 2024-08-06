/*
 * Copyright (c) Contributors to the Open 3D Engine Project.
 * For complete copyright and license terms please see the LICENSE at the root of this distribution.
 *
 * SPDX-License-Identifier: Apache-2.0 OR MIT
 *
 */

#pragma once

#include <VehicleDynamics/VehicleDynamicsBus.h>

#include <AzCore/Component/Component.h>
#include <AzCore/Component/TickBus.h>

namespace VehicleDynamics
{
    class VehicleDynamicsSystemComponent : public AZ::Component
    {
    public:
        AZ_COMPONENT_DECL(VehicleDynamicsSystemComponent);

        static void Reflect(AZ::ReflectContext* context);

        static void GetProvidedServices(AZ::ComponentDescriptor::DependencyArrayType& provided);
        static void GetIncompatibleServices(AZ::ComponentDescriptor::DependencyArrayType& incompatible);
        static void GetRequiredServices(AZ::ComponentDescriptor::DependencyArrayType& required);
        static void GetDependentServices(AZ::ComponentDescriptor::DependencyArrayType& dependent);

        VehicleDynamicsSystemComponent() = default;
        ~VehicleDynamicsSystemComponent() = default;

    protected:
        ////////////////////////////////////////////////////////////////////////

        ////////////////////////////////////////////////////////////////////////
        // AZ::Component interface implementation
        void Init() override{};
        void Activate() override{};
        void Deactivate() override{};
        ////////////////////////////////////////////////////////////////////////
    };

} // namespace VehicleDynamics
