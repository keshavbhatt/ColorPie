cmake_minimum_required(VERSION 3.21)

project(colorpie
    VERSION 2.0
    DESCRIPTION "Color picker and converter"
    HOMEPAGE_URL "https://github.com/keshavbhatt/ColorPie"
    LANGUAGES CXX
)

set(APP_NAME "ColorPie")
set(APP_EXECUTABLE_NAME "colorpie")

configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/src/colorpie.desktop.in"
    "${CMAKE_CURRENT_BINARY_DIR}/${APP_EXECUTABLE_NAME}.desktop"
    @ONLY
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

# QHttpServer::bind(QTcpServer*) needs Qt >= 6.8; developed and shipped
# against Qt 6.11 (kde-qt6-core24-sdk / kf6-core24).
find_package(Qt6 6.8 REQUIRED COMPONENTS
    Core
    Gui
    Widgets
    Network
    HttpServer
    WebEngineWidgets
    WebEngineCore
)

qt_standard_project_setup()

# Dev builds against the kde-qt6-core24-sdk snap on a non-Ubuntu host: some
# imported Qt targets export the SDK's generic /usr/include (Ubuntu glibc
# headers), which shadow the host toolchain's libc headers and break
# libstdc++. Strip that one directory from every imported target; the Qt
# headers proper live under .../include/<arch>/qt6 and are unaffected.
# Harmless in snapcraft builds (the dir then equals the real sysroot's).
if(CMAKE_PREFIX_PATH MATCHES "/snap/kde-qt6-core24-sdk/")
    get_property(_colorpie_imported DIRECTORY PROPERTY IMPORTED_TARGETS)
    foreach(_tgt IN LISTS _colorpie_imported)
        get_target_property(_incs ${_tgt} INTERFACE_INCLUDE_DIRECTORIES)
        if(_incs)
            set(_filtered "")
            foreach(_dir IN LISTS _incs)
                if(NOT _dir MATCHES "/snap/kde-qt6-core24-sdk/[^/]+/usr/include$")
                    list(APPEND _filtered "${_dir}")
                endif()
            endforeach()
            set_property(TARGET ${_tgt} PROPERTY
                INTERFACE_INCLUDE_DIRECTORIES "${_filtered}")
        endif()
    endforeach()
endif()

# Suppress qDebug output in non-debug builds.
add_compile_definitions(
    $<$<OR:$<CONFIG:Release>,$<CONFIG:RelWithDebInfo>,$<CONFIG:MinSizeRel>>:QT_NO_DEBUG_OUTPUT>
)

# Bundled QtColorWidgets library
add_subdirectory(src/lib)

# Plain add_executable (not qt_add_executable): the app has no QML, and
# skipping Qt target finalization avoids running the SDK snap's
# qmlimportscanner, which cannot start on non-Ubuntu dev hosts.
add_executable(${APP_EXECUTABLE_NAME}
    src/main.cpp
    src/mainwindow.cpp
    src/mainwindow.h
    src/mainwindow.ui
    src/manager.cpp
    src/manager.h
    src/manager.ui
    src/supportedinputs.cpp
    src/supportedinputs.h
    src/supportedinputs.ui
    src/utils.cpp
    src/utils.h
    src/resources.qrc
    src/breeze.qrc
)

target_compile_definitions(${APP_EXECUTABLE_NAME} PRIVATE
    APPNAMESTR="${APP_NAME}"
    VERSIONSTR="${PROJECT_VERSION}"
)

target_link_libraries(${APP_EXECUTABLE_NAME} PRIVATE
    colorwidgets
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
    Qt6::Network
    Qt6::HttpServer
    Qt6::WebEngineWidgets
    Qt6::WebEngineCore
)

# Install rules
include(GNUInstallDirs)
install(TARGETS ${APP_EXECUTABLE_NAME} DESTINATION ${CMAKE_INSTALL_BINDIR})

if(NOT WIN32)
    install(FILES src/icons/icon-512.png
        DESTINATION ${CMAKE_INSTALL_DATADIR}/icons/hicolor/512x512/apps
        RENAME ${APP_EXECUTABLE_NAME}.png
    )
    install(FILES "${CMAKE_CURRENT_BINARY_DIR}/${APP_EXECUTABLE_NAME}.desktop"
        DESTINATION ${CMAKE_INSTALL_DATADIR}/applications
    )
endif()
