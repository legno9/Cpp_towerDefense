cmake_minimum_required(VERSION 3.23)
project(GameTemplate LANGUAGES CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
option(BUILD_SHARED_LIBS "Build shared libraries" OFF)

include(FetchContent)

# Download and add SFML as a dependency
FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 2.6.x)
FetchContent_MakeAvailable(SFML)

# Download and add tmxlite (tiled editor loader) as a dependency
FetchContent_Declare(
    TMXLITE
    GIT_REPOSITORY https://github.com/fallahn/tmxlite.git
    GIT_TAG v1.4.4
    SOURCE_SUBDIR tmxlite)
FetchContent_MakeAvailable(TMXLITE)


set(PROJECT_HEADERS 
    include/Utils/Common.h
    include/Utils/json.hpp

    include/Core/AnimationDataManager.h
    include/Core/AssetManager.h
    include/Core/GameObjectManager.h
    include/Core/JsonManager.h
    include/Core/GameManager.h
    include/Core/MouseManager.h
    include/Core/RenderManager.h
    
    include/Animation/AnimationComponent.h

    include/Gameplay/World.h
    include/Gameplay/Level.h
    include/Gameplay/GameObject.h
    
    include/Gameplay/Enemies/EnemyBase.h
    include/Gameplay/Enemies/EnemyBasic.h

    include/Gameplay/Turrets/TurretBase.h
    include/Gameplay/Turrets/TurretShooter.h

    include/Render/SFMLOrthogonalLayer.h

    include/UI/UIScreenMainMenu.h
)

set(PROJECT_SOURCES 

    src/Core/AnimationDataManager.cpp
    src/Core/AssetManager.cpp
    src/Core/GameObjectManager.cpp
    src/Core/JsonManager.cpp
    src/Core/GameManager.cpp
    src/Core/MouseManager.cpp
    src/Core/RenderManager.cpp
    
    src/Animation/AnimationComponent.cpp

    src/Gameplay/World.cpp
    src/Gameplay/Level.cpp

    src/Gameplay/Enemies/EnemyBase.cpp
    src/Gameplay/Enemies/EnemyBasic.cpp

    src/Gameplay/Turrets/TurretBase.cpp
    src/Gameplay/Turrets/TurretShooter.cpp

    src/Render/SFMLOrthogonalLayer.cpp

    src/main.cpp
)

set(PROJECT_FILES 
  ${PROJECT_HEADERS} 
  ${PROJECT_SOURCES}
)

include_directories(${PROJECT_NAME} PRIVATE ${tmxlite_INCLUDE_DIR} include)

add_executable(${PROJECT_NAME} ${PROJECT_FILES})

set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${PROJECT_NAME})
set_property(TARGET ${PROJECT_NAME} PROPERTY 
    VS_DEBUGGER_WORKING_DIRECTORY 
    "$<IF:$<CONFIG:Debug>,${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Debug,${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Release>")

add_custom_command(
    TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    "${CMAKE_SOURCE_DIR}/data" "$<TARGET_FILE_DIR:${PROJECT_NAME}>/data"
)


target_link_libraries(${PROJECT_NAME} PRIVATE sfml-graphics tmxlite)
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_17)
target_compile_definitions(${PROJECT_NAME} PUBLIC DEBUG_MODE=0)

if(WIN32)
    add_custom_command(
        TARGET ${PROJECT_NAME}
        COMMENT "Copy OpenAL DLL"
        PRE_BUILD COMMAND ${CMAKE_COMMAND} -E copy ${SFML_SOURCE_DIR}/extlibs/bin/$<IF:$<EQUAL:${CMAKE_SIZEOF_VOID_P},8>,x64,x86>/openal32.dll $<TARGET_FILE_DIR:${PROJECT_NAME}>
        VERBATIM)
endif()

# TO-DO 
# - Sistema de torres, 2 tipos de torres. Una que dispara y otra que ralentiza. Colocarlo en el escenario. Estadisticas.
# - Sistema de enemigos, 3 tipos de enemigos. Estadisticas. Recorrido.
# - Sistema de proyectiles que surgen de las torres hacia los enemigos, 2 tipos de proyectiles.

# - Sistema de recursos con los que comprar torres y mejorarlas. Se obtiene de los enemigos.
# - Zona de la base, si un enemigo llega a la base se pierde vida. Sistema de vida.
# - Sistema de puntuacion

# - Sistema de interfaz de usuario con botones para comprar torres y mejorarlas.
# - Pantalla de inicio, pausa, fin de juego.