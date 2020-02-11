cmake_minimum_required(VERSION 3.2)

project(TalkingSprites LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(Boost_USE_MULTITHREADED TRUE) # Prevents some miscellaneous CMake warnings
set(JSON_BuildTests OFF CACHE INTERNAL "") # Just in case we need JSON support

if (APPLE)
add_definitions(-g -stdlib=libc++)
elseif (WIN32)
add_compile_definitions(_WIN32_WINNT=0x0A00)
endif()

# Find our libraries
find_package(SFML 2.5 COMPONENTS graphics window system REQUIRED)
find_package(ImGui-SFML REQUIRED)
find_package(Boost 1.72 COMPONENTS program_options REQUIRED)
find_package(Threads)
add_subdirectory(deps/nlohmann_json)

# Add include directories
include_directories("${CMAKE_SOURCE_DIR}/src")
include_directories(SYSTEM ${Boost_INCLUDE_DIR})

# Build the application
add_executable(TalkingSprites
    ${CMAKE_SOURCE_DIR}/src/fwd/sfml.hpp

    ${CMAKE_SOURCE_DIR}/src/tools/string.hpp

    ${CMAKE_SOURCE_DIR}/src/game/tile.hpp
    ${CMAKE_SOURCE_DIR}/src/game/state.hpp
    ${CMAKE_SOURCE_DIR}/src/game/character.hpp

    ${CMAKE_SOURCE_DIR}/src/game/player.cpp ${CMAKE_SOURCE_DIR}/src/game/player.hpp

    ${CMAKE_SOURCE_DIR}/src/web/client.cpp ${CMAKE_SOURCE_DIR}/src/web/client.hpp
    ${CMAKE_SOURCE_DIR}/src/web/server.cpp ${CMAKE_SOURCE_DIR}/src/web/server.hpp

    ${CMAKE_SOURCE_DIR}/src/application.cpp ${CMAKE_SOURCE_DIR}/src/application.hpp

    ${CMAKE_SOURCE_DIR}/src/main.cpp
)

if (APPLE)
    target_compile_options(TalkingSprites PRIVATE -Werror -Wall -Wextra)
endif ()

# Link the application
target_link_libraries(TalkingSprites PRIVATE sfml-graphics)
target_link_libraries(TalkingSprites PRIVATE ImGui-SFML::ImGui-SFML)
target_link_libraries(TalkingSprites PRIVATE nlohmann_json::nlohmann_json)
target_link_libraries(TalkingSprites PRIVATE ${CMAKE_THREAD_LIBS_INIT})
target_link_libraries(TalkingSprites PRIVATE ${Boost_LIBRARIES})