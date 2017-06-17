cmake_minimum_required(VERSION 3.0)
project(SteamWorkshopUploader)

set(CMAKE_CXX_FLAGS_DEBUG -DDEBUG)

# wxWidgets API
set(wxWidgets_CONFIGURATION mswu)
find_package(wxWidgets COMPONENTS core base REQUIRED)
include(${wxWidgets_USE_FILE})
# End wxWidgets API

# Steam API
include_directories("${STEAM_INCLUDE}")
add_library(steam_api SHARED IMPORTED)
set_target_properties(steam_api PROPERTIES BUILD_WITH_INSTALL_RPATH
    TRUE IMPORTED_LOCATION libsteam_api.so)
cmake_policy(SET CMP0015 NEW)
link_directories(.)
# End Steam API

add_executable(SteamWorkshopUploader main.cpp gui_frames.cpp
    gui_frames.h steam.cpp steam.h)

target_link_libraries(SteamWorkshopUploader steam_api
    ${wxWidgets_LIBRARIES})
