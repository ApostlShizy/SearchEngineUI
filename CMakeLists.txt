cmake_minimum_required(VERSION 3.19)
project(SearchEngineUi LANGUAGES CXX)

find_package(Qt6 6.5 REQUIRED COMPONENTS Core Widgets)

qt_standard_project_setup()

set(CMAKE_CXX_STANDARD 20)
set(JSON_BuildTests OFF CACHE INTERNAL "")

add_subdirectory(nlohmann_json)

qt_add_executable(SearchEngineUi
    WIN32 MACOSX_BUNDLE
    main.cpp
    optionwindow.cpp
    optionwindow.h
    optionwindow.ui
    searchengine.h searchengine.cpp
    SearchServer.h
    InvertedIndex.h
    ConverterJSON.h
)

target_link_libraries(SearchEngineUi
    PRIVATE
        Qt::Core
        Qt::Widgets
        nlohmann_json::nlohmann_json
)

include(GNUInstallDirs)

install(TARGETS SearchEngineUi
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

qt_generate_deploy_app_script(
    TARGET SearchEngineUi
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${deploy_script})
