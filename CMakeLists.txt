cmake_minimum_required (VERSION 3.8)
project (LogonRenderer)

if(NOT CMAKE_BUILD_TYPE)
set(CMAKE_BUILD_TYPE Release)
endif()

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "-Wall -Wextra")
set(CMAKE_CXX_FLAGS_DEBUG "-DDEBUG -O0 -g3")
set(CMAKE_CXX_FLAGS_RELEASE "-DNDEBUG -O3 -g0")

# Add source files
file(GLOB_RECURSE SOURCE_FILES
        ${CMAKE_SOURCE_DIR}/code/*.c
        ${CMAKE_SOURCE_DIR}/code/*.cpp)

# Add header files
file(GLOB_RECURSE HEADER_FILES
	${CMAKE_SOURCE_DIR}/code/*.h
	${CMAKE_SOURCE_DIR}/code/*.hpp)

# Add .lib files
link_directories(${CMAKE_SOURCE_DIR}/lib)

# Define the executable
add_executable(${PROJECT_NAME} ${HEADER_FILES} ${SOURCE_FILES})

set (GLM_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/contrib/glm/0.9.9.0/include/)
set (ASSIMP_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/contrib/assimp/4.1.0/include/)
#set (IMAGEMAGICK_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/contrib/imagemagick/7.0.8-3/include/ImageMagick-7/)

# GLM
#find_package(GLM REQUIRED)
#message(STATUS "GLM included at ${GLM_INCLUDE_DIR}")

# ASSIMP
#find_package(ASSIMP REQUIRED)
#message(STATUS "Found ASSIMP in ${ASSIMP_INCLUDE_DIR}")

# IMAGEMAGICK
#find_package(IMAGEMAGICK REQUIRED)
#message(STATUS "Found IMAGEMAGICK in ${IMAGEMAGICK_INCLUDE_DIR}")

# Put all libraries into a variable
set(LIBS assimp glm)

# Define the include DIRs
include_directories(
    "${CMAKE_SOURCE_DIR}/code"
    ${GLM_INCLUDE_DIR}
    ${ASSIMP_INCLUDE_DIR}
#${IMAGEMAGICK_INCLUDE_DIR}
)

# Define the link libraries
target_link_libraries(${PROJECT_NAME} ${LIBS})
