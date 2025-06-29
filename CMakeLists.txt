cmake_minimum_required(VERSION 3.13...3.22)
include(FetchContent)

project(app)

# Set number of parallel build jobs to number of available processors
include(ProcessorCount)
ProcessorCount(N)
if(NOT N EQUAL 0)
  set(CMAKE_BUILD_PARALLEL_LEVEL ${N})
endif()

set(CMAKE_CXX_STANDARD 17)
set(XMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -pthread")
set(RLIB --docroot ../../ -c ../../wt_config.xml --http-address 0.0.0.0 --http-port 9020)
set(SOURCE_DIR ${PROJECT_SOURCE_DIR}/src/)

include_directories(${SOURCE_DIR})


FetchContent_Declare(json URL https://github.com/nlohmann/json/releases/download/v3.12.0/json.tar.xz)
FetchContent_MakeAvailable(json)

FetchContent_Declare(cpr GIT_REPOSITORY https://github.com/libcpr/cpr.git GIT_TAG da40186618909b1a7363d4e4495aa899c6e0eb75) 
FetchContent_MakeAvailable(cpr)

# ./app --docroot ../ –config ./wt_config.xml --http-address 0.0.0.0 --http-port 9020

set(SOURCES
    ${SOURCE_DIR}/main.cpp
    
    ${SOURCE_DIR}/000-Components/Button.cpp

    ${SOURCE_DIR}/001-App/App.cpp
    ${SOURCE_DIR}/001-App/Server.cpp
    ${SOURCE_DIR}/002-Dbo/Session.cpp
    ${SOURCE_DIR}/002-Dbo/Tables/User.cpp
    ${SOURCE_DIR}/002-Dbo/Tables/Permission.cpp
    
    ${SOURCE_DIR}/003-Auth/AuthWidget.cpp
    ${SOURCE_DIR}/003-Auth/RegistrationView.cpp
    ${SOURCE_DIR}/003-Auth/UserDetailsModel.cpp
    
    ${SOURCE_DIR}/004-Theme/Theme.cpp
    ${SOURCE_DIR}/004-Theme/ThemeSwitcher.cpp
    ${SOURCE_DIR}/004-Theme/DarkModeToggle.cpp

    ${SOURCE_DIR}/005-WidgetsDisplay/WidgetsDisplay.cpp
    ${SOURCE_DIR}/006-AboutMe/AboutMe.cpp
    ${SOURCE_DIR}/007-UserSettings/UserSettingsPage.cpp

    ${SOURCE_DIR}/010-TestWidgets/Test.cpp
    ${SOURCE_DIR}/010-TestWidgets/Navigation.cpp
    
    ${SOURCE_DIR}/101-Stylus/Stylus.cpp
    ${SOURCE_DIR}/101-Stylus/000-Utils/StylusState.cpp
    ${SOURCE_DIR}/101-Stylus/000-Utils/XMLFileBrain.cpp
    ${SOURCE_DIR}/101-Stylus/000-Utils/MonacoEditor.cpp
    ${SOURCE_DIR}/101-Stylus/000-Utils/FilesManager.cpp
    ${SOURCE_DIR}/101-Stylus/000-Utils/StylusPanelWrapper.cpp
    ${SOURCE_DIR}/101-Stylus/000-Utils/tinyxml2/tinyxml2.cpp
    ${SOURCE_DIR}/101-Stylus/001-XmlFilesManager/XmlFilesManager.cpp
    ${SOURCE_DIR}/101-Stylus/001-XmlFilesManager/Preview/XMLElemNode.cpp
    ${SOURCE_DIR}/101-Stylus/001-XmlFilesManager/Preview/XMLTreeNode.cpp
    ${SOURCE_DIR}/101-Stylus/001-XmlFilesManager/Preview/ControlCenter.cpp
    ${SOURCE_DIR}/101-Stylus/001-XmlFilesManager/Preview/TemplateVarControlCenter.cpp
    ${SOURCE_DIR}/101-Stylus/002-CssFilesManager/CssFilesManager.cpp
    ${SOURCE_DIR}/101-Stylus/003-JsFilesManager/JsFilesManager.cpp
    ${SOURCE_DIR}/101-Stylus/004-TailwindConfigManager/TailwindConfigManager.cpp
    ${SOURCE_DIR}/101-Stylus/005-Images/ImagesManager.cpp
    ${SOURCE_DIR}/101-Stylus/006-Settings/Settings.cpp

)

add_executable(${PROJECT_NAME} ${SOURCES})

target_link_libraries(${PROJECT_NAME} 
    wthttp
    wt
    wtdbo
    wtdbosqlite3
    # wtdbopostgres
    boost_regex
    cpr::cpr
    nlohmann_json::nlohmann_json
)

add_custom_target(run
    COMMAND ${PROJECT_NAME} ${RLIB}
    DEPENDS ${PROJECT_NAME}
    WORKING_DIRECTORY ${CMAKE_PROJECT_DIR}
)
# add_custom_target(run
#     COMMAND ${PROJECT_NAME} ${RLIB} > ./logs.txt 2>&1
#     DEPENDS ${PROJECT_NAME}
#     WORKING_DIRECTORY ${CMAKE_PROJECT_DIR}
# )
