cmake_minimum_required(VERSION 3.10)
project(linux-shell)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add custom modules (including FindReadline.cmake)
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

find_package(Readline REQUIRED)

set(SOURCES
    src/main.cpp
    src/Shell.cpp
    src/BuiltIn.cpp
    src/History.cpp
    src/Alias.cpp
    src/Executor.cpp
)

set(HEADERS
    include/Shell.hpp
    include/BuiltIn.hpp
    include/History.hpp
    include/Alias.hpp
    include/Executor.hpp
)

add_executable(linux-shell ${SOURCES} ${HEADERS})

# Link with Readline using the variables provided by FindReadline.cmake
target_include_directories(linux-shell PRIVATE ${Readline_INCLUDE_DIRS} ${CMAKE_SOURCE_DIR}/include)
target_link_libraries(linux-shell ${Readline_LIBRARIES})

# Installation setup: install binary to /usr/local/bin
install(TARGETS linux-shell RUNTIME DESTINATION /usr/local/bin)

# Uninstall target
add_custom_target(uninstall
  COMMAND xargs rm < install_manifest.txt
  WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
)

