/*
 * Copyright (c) Contributors to the Open 3D Engine Project.
 * For complete copyright and license terms please see the LICENSE at the root of this distribution.
 *
 * SPDX-License-Identifier: Apache-2.0 OR MIT
 *
 */

#pragma once

#include "VehicleDynamicsTypeIds.h"

#include <AzCore/Component/Component.h>
#include <AzCore/EBus/EBus.h>
#include <AzCore/Interface/Interface.h>

namespace VehicleDynamics
{
    class VehicleDynamicsRequests : public AZ::ComponentBus
    {
    public:
        AZ_RTTI(VehicleDynamicsRequests, VehicleDynamicsRequestsTypeId);

        //! Set the steering angle of the vehicle
        //! \param steeringAngle The steering angle in radians
        virtual void SetSteeringAngle(float steeringAngle) = 0;

        //! Set the acceleration of the vehicle
        //! \param steeringAcceleration The acceleration in m/s^2
        virtual void SetAcceleration(float steeringAcceleration) = 0;

        static void Reflect(AZ::ReflectContext* context);
    };

    using VehicleDynamicsRequestBus = AZ::EBus<VehicleDynamicsRequests>;
    using VehicleDynamicsInterface = AZ::Interface<VehicleDynamicsRequests>;

} // namespace VehicleDynamics
