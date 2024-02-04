cmake_minimum_required(VERSION 3.16)

project(Autoclicker VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 6.5 REQUIRED COMPONENTS Quick)
find_package(Qt6 6.5 REQUIRED COMPONENTS Network)
find_package(Qt6 6.5 REQUIRED COMPONENTS QuickControls2)

qt_standard_project_setup(REQUIRES 6.5)

qt_add_executable(appAutoclicker
    src/main.cpp
    src/autoclicker.h src/autoclicker.cpp
    src/abstractjob.h src/abstractjob.cpp
    src/clickerjob.h src/clickerjob.cpp
    src/sleepjob.h src/sleepjob.cpp
    src/mousewheeljob.h src/mousewheeljob.cpp
)
if(UNIX)
qt_add_executable(deviceProgram
    srcInputLinux/mainProgram.cpp
    srcInputLinux/inputevent.h
    srcInputLinux/inputevent.cpp


)
endif()
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src)
qt_add_qml_module(appAutoclicker
    URI Autoclicker
    VERSION 1.0
    QML_FILES qml/Main.qml
    QML_FILES qml/ClickDelegate.qml
    QML_FILES qml/SleepDelegate.qml
    QML_FILES qml/AbstractDelegate.qml
    QML_FILES qml/MouseWheelDelegate.qml
)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
set_target_properties(appAutoclicker PROPERTIES
#    MACOSX_BUNDLE_GUI_IDENTIFIER com.example.appAutoclicker
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)
if(UNIX)
set_target_properties(deviceProgram PROPERTIES
#    MACOSX_BUNDLE_GUI_IDENTIFIER com.example.appAutoclicker
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE

)
endif()
target_link_libraries(appAutoclicker
    PRIVATE Qt6::Quick
    PRIVATE Qt6::Network
    PRIVATE Qt6::QuickControls2

)
if(UNIX)
target_link_libraries(deviceProgram
    PRIVATE Qt6::Network
)
endif()
include(GNUInstallDirs)
install(TARGETS appAutoclicker
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
