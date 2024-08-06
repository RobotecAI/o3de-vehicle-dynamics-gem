/*
 * Copyright (c) Contributors to the Open 3D Engine Project.
 * For complete copyright and license terms please see the LICENSE at the root of this distribution.
 *
 * SPDX-License-Identifier: Apache-2.0 OR MIT
 *
 */

#include "VehicleDynamicsSystemComponent.h"
#include <VehicleDynamics/VehicleDynamicsTypeIds.h>
#include <VehicleDynamicsModuleInterface.h>

namespace VehicleDynamics
{
    class VehicleDynamicsModule : public VehicleDynamicsModuleInterface
    {
    public:
        AZ_RTTI(VehicleDynamicsModule, VehicleDynamicsModuleTypeId, VehicleDynamicsModuleInterface);
        AZ_CLASS_ALLOCATOR(VehicleDynamicsModule, AZ::SystemAllocator);
    };
} // namespace VehicleDynamics

AZ_DECLARE_MODULE_CLASS(Gem_vehicle_dynamics_gem, VehicleDynamics::VehicleDynamicsModule)
