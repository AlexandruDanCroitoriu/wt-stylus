cmake_minimum_required(VERSION 3.13...3.22)

project(app)

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
    ${SOURCE_DIR}/002-Dbo/User.cpp
    
    ${SOURCE_DIR}/003-Auth/AuthWidget.cpp
    ${SOURCE_DIR}/003-Auth/RegistrationView.cpp
    ${SOURCE_DIR}/003-Auth/UserDetailsModel.cpp
    
    ${SOURCE_DIR}/010-TestWidgets/Test.cpp
    ${SOURCE_DIR}/010-TestWidgets/DarkModeToggle.cpp
    
    
    ${SOURCE_DIR}101-Stylus/Stylus.cpp
    ${SOURCE_DIR}101-Stylus/StylusState.cpp
    ${SOURCE_DIR}101-Stylus/000-Utils/MonacoEditor.cpp
    ${SOURCE_DIR}101-Stylus/000-Utils/FilesManager.cpp
    ${SOURCE_DIR}101-Stylus/000-Utils/tinyxml2/tinyxml2.cpp
    ${SOURCE_DIR}101-Stylus/001-XmlFilesManager/XmlFilesManager.cpp
    ${SOURCE_DIR}101-Stylus/001-XmlFilesManager/Preview/XMLFileBrain.cpp
    ${SOURCE_DIR}101-Stylus/001-XmlFilesManager/Preview/XMLElemNode.cpp
    ${SOURCE_DIR}101-Stylus/001-XmlFilesManager/Preview/XMLTreeNode.cpp
    ${SOURCE_DIR}101-Stylus/001-XmlFilesManager/Preview/ControlCenter.cpp
    ${SOURCE_DIR}101-Stylus/002-CssFilesManager/CssFilesManager.cpp
    ${SOURCE_DIR}101-Stylus/003-JsFilesManager/JsFilesManager.cpp
    ${SOURCE_DIR}101-Stylus/004-TailwindConfigManager/TailwindConfigManager.cpp

)

add_executable(${PROJECT_NAME} ${SOURCES})

target_link_libraries(${PROJECT_NAME} 
    wthttp
    wt
    wtdbo
    wtdbosqlite3
    # wtdbopostgres
    boost_regex
)

add_custom_target(run
    COMMAND ${PROJECT_NAME} ${RLIB}
    DEPENDS ${PROJECT_NAME}
    WORKING_DIRECTORY ${CMAKE_PROJECT_DIR}
)

