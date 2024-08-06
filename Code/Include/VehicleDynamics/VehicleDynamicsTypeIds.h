/*
 * Copyright (c) Contributors to the Open 3D Engine Project.
 * For complete copyright and license terms please see the LICENSE at the root of this distribution.
 *
 * SPDX-License-Identifier: Apache-2.0 OR MIT
 *
 */

#pragma once

namespace VehicleDynamics
{
    // System Component TypeIds
    inline constexpr const char* VehicleDynamicsSystemComponentTypeId = "{B6ECDCCF-4B1E-47E7-813C-13E55D867B18}";
    inline constexpr const char* VehicleDynamicsEditorSystemComponentTypeId = "{AF2A05B0-07B9-4A0B-962D-9BDB918CFD01}";

    // Module derived classes TypeIds
    inline constexpr const char* VehicleDynamicsModuleInterfaceTypeId = "{55CC48D9-4546-446F-A220-03C1CCF1FADD}";
    inline constexpr const char* VehicleDynamicsModuleTypeId = "{D4D015B5-1A61-4465-A23D-653371ACEE5F}";
    // The Editor Module by default is mutually exclusive with the Client Module
    // so they use the Same TypeId
    inline constexpr const char* VehicleDynamicsEditorModuleTypeId = VehicleDynamicsModuleTypeId;

    // Interface TypeIds
    inline constexpr const char* VehicleDynamicsRequestsTypeId = "{897AA56C-B024-4114-8A0C-327A10A38F88}";

    // Components TypeIds
    inline constexpr const char* SimpleVehicleControllerTypeId = "{688EDC5B-04C8-4AC8-AA4F-DB43D389CAB9}";
    inline constexpr const char* SimpleVehicleConfigurationTypeId = "{BC125E68-1906-4384-B0B5-E77A2F6F8A3F}";
    inline constexpr const char* SimpleVehicleEditorComponentTypeId = "{DBB05966-6D98-45E6-B17C-3E99F170E220}";
    inline constexpr const char* SimpleWheelConfigurationTypeId = "{51955F0C-C312-4732-B58B-494956E52AC5}";
    inline constexpr const char* SimpleWheelControllerTypeId = "{8BBDAAF3-5A86-4345-8EF9-7EC99103C1B8}";
    inline constexpr const char* SimpleWheelEditorComponentTypeId = "{B6C9C871-EDBA-45BE-AFD4-DA57D5F82165}";

    inline constexpr const char* VehicleControllerTypeId = "{EE1AD1D5-413C-493B-9FF7-D4F7ECA0D003}";
    inline constexpr const char* VehicleConfigurationTypeId = "{EEEF81A6-0FE7-4F1C-9B50-45E9E809C970}";
    inline constexpr const char* VehicleEditorComponentTypeId = "{A09F4D45-07D3-4F07-B65E-59A537282199}";
    inline constexpr const char* WheelConfigurationTypeId = "{02C676E9-9A56-4C7E-B32C-BBBA3F6ED68C}";
    inline constexpr const char* WheelControllerTypeId = "{D8D81612-18CE-4492-9058-1891D9789A7B}";
    inline constexpr const char* WheelEditorComponentTypeId = "{EDE68C46-F5DE-49AB-8F5D-15F51F05E96F}";

} // namespace VehicleDynamics
