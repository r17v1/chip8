cmake_minimum_required(VERSION 3.10)
project(chip8)

# Set C++ standard
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Download SDL2
include(FetchContent)
FetchContent_Declare(
  SDL2
  GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
  GIT_TAG release-2.30.8  # Use the latest stable release tag
)
FetchContent_MakeAvailable(SDL2)




# Include directories
include_directories(${SDL2_SOURCE_DIR}/include src/headers)

# Add executable
add_executable(chip8 src/main.cpp src/SDLWrapper.cpp src/chip8.cpp)

# Link SDL2
target_link_libraries(chip8 SDL2::SDL2main SDL2::SDL2 )