cmake_minimum_required (VERSION 3.14)
set(CRYENGINE_DIR "C:/Users/Maxwell/AppData/Local/Programs/CRYENGINE Launcher/Engines/crytek/cryengine-57-lts/5.7.1" CACHE STRING "CRYENGINE root directory.")
set(TOOLS_CMAKE_DIR "${CRYENGINE_DIR}/Tools/CMake")

set(PROJECT_BUILD 1)
set(PROJECT_DIR "${CMAKE_SOURCE_DIR}/../")

include("${TOOLS_CMAKE_DIR}/InitialSetup.cmake")
include("${TOOLS_CMAKE_DIR}/CommonOptions.cmake")

add_subdirectory("${CRYENGINE_DIR}" "${CMAKE_CURRENT_BINARY_DIR}/CRYENGINE")

include("${TOOLS_CMAKE_DIR}/Configure.cmake")
start_sources()

sources_platform(ALL)
add_sources("Code_uber.cpp"
    PROJECTS WarCompany
    SOURCE_GROUP "Root"
		"GamePlugin.cpp"
		"StdAfx.cpp"
		"GamePlugin.h"
		"StdAfx.h"
)

set(COMPANY_UNIT_SYS_DIR "TroopUnitSystem")
set(TROOP_UNIT_DIR "${COMPANY_UNIT_SYS_DIR}/TroopUnit")
set(TROOP_COMPANY_DIR "${COMPANY_UNIT_SYS_DIR}/TroopCompany")
add_sources("TroopUnitSystem_uber.cpp"
    PROJECTS WarCompany
    SOURCE_GROUP "Troop Unit System"
        "${COMPANY_UNIT_SYS_DIR}/ITroopUnitSystem.h"
        "${COMPANY_UNIT_SYS_DIR}/TroopUnitSystem.h"
        "${COMPANY_UNIT_SYS_DIR}/TroopUnitSystem.cpp"
        "${COMPANY_UNIT_SYS_DIR}/CompanyUnit.h"
        "${COMPANY_UNIT_SYS_DIR}/CompanyUnit.cpp"
    SOURCE_GROUP "Troop Unit System/Troop Unit"
        "${TROOP_UNIT_DIR}/ITroopUnitBasicTypes.h"
        "${TROOP_UNIT_DIR}/ITroopUnit.h"
        "${TROOP_UNIT_DIR}/TroopUnit.h"
        "${TROOP_UNIT_DIR}/TroopUnit.cpp"
        "${TROOP_UNIT_DIR}/ITroopUnitClass.h"
        "${TROOP_UNIT_DIR}/TroopUnitClass.h"
        "${TROOP_UNIT_DIR}/TroopUnitClass.cpp"
    SOURCE_GROUP "Troop Unit System/Troop Company"
        "${TROOP_COMPANY_DIR}/ITroopCompany.h"
        "${TROOP_COMPANY_DIR}/TroopCompany.h"
        "${TROOP_COMPANY_DIR}/TroopCompany.cpp"
        "${TROOP_COMPANY_DIR}/ITroopCompanyClass.h"
        "${TROOP_COMPANY_DIR}/TroopCompanyClass.h"
        "${TROOP_COMPANY_DIR}/TroopCompanyClass.cpp"
)

set(BATTLE_FORMATION_DIR "BattleFormation")
set(FORMATION_SPLINE_DIR "${BATTLE_FORMATION_DIR}/FormationSpline")
add_sources("BattleFormation_uber.cpp"
    PROJECTS WarCompany
    SOURCE_GROUP "Battle Formation"
        "${BATTLE_FORMATION_DIR}/IFormationUnit.h"
        "${BATTLE_FORMATION_DIR}/IFormationSlot.h"
        "${BATTLE_FORMATION_DIR}/FormationSlot.cpp"
        "${BATTLE_FORMATION_DIR}/FormationSlot.h"
        "${BATTLE_FORMATION_DIR}/BattleFormation.cpp"
        "${BATTLE_FORMATION_DIR}/BattleFormation.h"
    SOURCE_GROUP "Battle Formation/Formation Builder"
        "${BATTLE_FORMATION_DIR}/FormationBuilder/FormationBuilder.h"
        "${BATTLE_FORMATION_DIR}/FormationBuilder/FormationBuilder.cpp"
        "${BATTLE_FORMATION_DIR}/FormationBuilder/FormationBuildGroup.h"
        "${BATTLE_FORMATION_DIR}/FormationBuilder/FormationBuildGroup.cpp"
    SOURCE_GROUP "Battle Formation/Formation Spline"
        "${FORMATION_SPLINE_DIR}/AnchorPoint.cpp"
        "${FORMATION_SPLINE_DIR}/AnchorPoint.h"
		"${FORMATION_SPLINE_DIR}/BattleLineSpline.cpp"
		"${FORMATION_SPLINE_DIR}/BattleLineSpline.h"
		"${FORMATION_SPLINE_DIR}/IBattleLineSpline.h"
		"${FORMATION_SPLINE_DIR}/VertexPoint.cpp"
		"${FORMATION_SPLINE_DIR}/VertexPoint.h"
		"${FORMATION_SPLINE_DIR}/IVertexPoint.h"
)

add_sources("Components_uber.cpp"
    PROJECTS WarCompany
    SOURCE_GROUP "Components"
		"Components/Player.cpp"
		"Components/Player.h"
        "Components/StdComponent.cpp"
        "Components/StdComponent.h"
)

if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/CVarOverrides.h")
    add_sources("NoUberFile"
        PROJECTS WarCompany
        SOURCE_GROUP "Root"
            "CVarOverrides.h"
    )
endif()

if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/EngineDefineOverrides.h")
    add_sources("NoUberFile"
        PROJECTS WarCompany
        SOURCE_GROUP "Root"
            "EngineDefineOverrides.h"
    )
endif()
end_sources()

CryGameModule(WarCompany FORCE_SHARED PCH "StdAfx.cpp" SOLUTION_FOLDER "Project")

target_include_directories(${THIS_PROJECT}
PRIVATE
    "${CRYENGINE_DIR}/Code/CryEngine/CryCommon"
    "${CRYENGINE_DIR}/Code/CryEngine/CryAction"
    "${CRYENGINE_DIR}/Code/CryEngine/CrySchematyc/Core/Interface"
    "${CRYENGINE_DIR}/Code/CryPlugins/CryDefaultEntities/Module"
)

if(CMAKE_CXX_COMPILER_ID MATCHES "[Cc]lang")
    target_compile_options(${THIS_PROJECT} PRIVATE
        -Wno-unused-variable
        -Wno-reorder
        -Wno-unknown-pragmas
        -Wno-parentheses
        -Wno-switch
        -Wno-format
        -Wno-dynamic-class-memaccess
        -Wno-unused-private-field
        -Wno-unused-value
        -Wno-invalid-offsetof
        -Wno-multichar
        -Wno-char-subscripts
        -Wno-null-conversion
        -Wno-empty-body
        -Wno-unused-lambda-capture
        -Wno-unused-function
        -Wno-tautological-constant-out-of-range-compare
    )
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    target_compile_options(${THIS_PROJECT} PRIVATE
        -Wno-unused-function
        -Wno-unused-value
        -Wno-unused-variable
        -Wno-aligned-new
        -Wno-int-in-bool-context
        -Wno-invalid-offsetof
        -Wno-comment
        -Wno-sign-compare
        -Wno-unused-but-set-variable
        -Wno-maybe-uninitialized
        -Wno-misleading-indentation
        -Wno-unused-result
        -Wno-unknown-pragmas
        -Wno-multichar
        -Wno-strict-aliasing
        -Wno-char-subscripts
        -Wno-conversion-null
        -Wno-reorder
        -Wno-parentheses
        -Wno-format
        -Wno-switch
    )
endif()


if(OPTION_ENGINE)
    if(NOT EXISTS "${CRYENGINE_DIR}/Code/Sandbox/EditorQt")
        add_library(Editor STATIC "${CRYENGINE_DIR}/Code/CryEngine/CryCommon/CryCore/Platform/platform.h")
        set_target_properties(Editor PROPERTIES LINKER_LANGUAGE CXX)
        if (WIN32)
            set_visual_studio_debugger_command(Editor "${CRYENGINE_DIR}/bin/win_x64/Sandbox.exe" "-project \"${PROJECT_DIR}/WarCompany.cryproject\"")
        endif()
    endif()
else()
    add_library(GameLauncher STATIC "${CRYENGINE_DIR}/Code/CryEngine/CryCommon/CryCore/Platform/platform.h")
    set_target_properties(GameLauncher PROPERTIES LINKER_LANGUAGE CXX)
    if (WIN32)
        set_visual_studio_debugger_command(GameLauncher "${CRYENGINE_DIR}/bin/win_x64/GameLauncher.exe" "-project \"${PROJECT_DIR}/WarCompany.cryproject\"")
    endif()

    add_library(Editor STATIC "${CRYENGINE_DIR}/Code/CryEngine/CryCommon/CryCore/Platform/platform.h")
    set_target_properties(Editor PROPERTIES LINKER_LANGUAGE CXX)
    if (WIN32)
        set_visual_studio_debugger_command(Editor "${CRYENGINE_DIR}/bin/win_x64/Sandbox.exe" "-project \"${PROJECT_DIR}/WarCompany.cryproject\"")
    endif()

    add_library(GameServer STATIC "${CRYENGINE_DIR}/Code/CryEngine/CryCommon/CryCore/Platform/platform.h")
    set_target_properties(GameServer PROPERTIES LINKER_LANGUAGE CXX)
    if (WIN32)
        set_visual_studio_debugger_command(GameServer "${CRYENGINE_DIR}/bin/win_x64/Game_Server.exe" "-project \"${PROJECT_DIR}/WarCompany.cryproject\"")
    endif()
endif()

# Set StartUp project in Visual Studio
set_solution_startup_target(GameLauncher)

if (WIN32)
    set_visual_studio_debugger_command( ${THIS_PROJECT} "${CRYENGINE_DIR}/bin/win_x64/GameLauncher.exe" "-project \"${PROJECT_DIR}/WarCompany.cryproject\"" )
endif()

#BEGIN-CUSTOM
# Make any custom changes here, modifications outside of the block will be discarded on regeneration.
#END-CUSTOM