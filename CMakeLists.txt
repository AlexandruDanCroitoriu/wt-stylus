cmake_minimum_required(VERSION 3.13...3.22)

project(app-base)

set(CMAKE_CXX_STANDARD 17)
set(XMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -pthread")
set(RLIB --docroot ../ -c ../wt_config.xml --http-address 0.0.0.0 --http-port 9020)
# set(RLIB --docroot ../ --http-address 0.0.0.0 --http-port 9020)
set(SOURCE_DIR ${PROJECT_SOURCE_DIR}/src/)

include_directories(${SOURCE_DIR})


set(SOURCES
    ${SOURCE_DIR}/main.cpp
    ${SOURCE_DIR}/001-App/App.cpp
    ${SOURCE_DIR}/002-Dbo/Session.cpp
    # ${SOURCE_DIR}/003-Tailwind/TVariable.cpp
    # ${SOURCE_DIR}/003-Tailwind/WTConfig.cpp
    # ${SOURCE_DIR}/003-Tailwind/WTVariable.cpp
    # ${SOURCE_DIR}/003-Tailwind/WTVariableText.cpp
    # ${SOURCE_DIR}/004-FilesManager/FilesManager.cpp
    # ${SOURCE_DIR}/004-FilesManager/MonacoEdditor.cpp
    # ${SOURCE_DIR}/005-TailwindConfigCenter/TailwindConfigCenter.cpp
    # ${SOURCE_DIR}/006-TemplatesManager/TemplatesManager.cpp
    
    ${SOURCE_DIR}/010-TestWidgets/Test.cpp
    ${SOURCE_DIR}/010-TestWidgets/DarkModeToggle.cpp
    
    
    ${SOURCE_DIR}101-Stylus/Stylus.cpp
    ${SOURCE_DIR}101-Stylus/StylusState.cpp
    ${SOURCE_DIR}101-Stylus/000-Utils/MonacoEditor.cpp
    ${SOURCE_DIR}101-Stylus/000-Utils/FilesManager.cpp
    ${SOURCE_DIR}101-Stylus/000-Utils/tinyxml2/tinyxml2.cpp
    ${SOURCE_DIR}101-Stylus/001-XmlFilesManager/XmlFilesManager.cpp
    ${SOURCE_DIR}101-Stylus/002-CssFilesManager/CssFilesManager.cpp
    ${SOURCE_DIR}101-Stylus/003-JsFilesManager/JsFilesManager.cpp
    ${SOURCE_DIR}101-Stylus/004-TailwindConfigManager/TailwindConfigManager.cpp
    # ${SOURCE_DIR}101-Stylus/004-TailwindConfigManager/WTConfig.cpp
    # ${SOURCE_DIR}101-Stylus/004-TailwindConfigManager/TVariable.cpp
    # ${SOURCE_DIR}101-Stylus/004-TailwindConfigManager/WTVariable.cpp
    # ${SOURCE_DIR}101-Stylus/004-TailwindConfigManager/WTVariableText.cpp

)

add_executable(${PROJECT_NAME} ${SOURCES})

target_link_libraries(${PROJECT_NAME} 
    wthttp
    wt
    wtdbo
    wtdbosqlite3
    wtdbopostgres
    boost_regex
)

add_custom_target(run
    COMMAND ${PROJECT_NAME} ${RLIB}
    DEPENDS ${PROJECT_NAME}
    WORKING_DIRECTORY ${CMAKE_PROJECT_DIR}
)

