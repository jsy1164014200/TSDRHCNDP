cmake_minimum_required(VERSION 3.9)

project(app)

option(CXX "enable C++ compilation" ON)

if(CXX)
  enable_language(CXX)
  set(CMAKE_CXX_STANDARD 20)
endif()

list(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR})
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE "Release")
endif()
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")

# Visual Studio compiler with static runtime libraries
if(MSVC AND MT)
  set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MT")
  set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MTd")
  set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} /MT")
  set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} /MTd")
endif()

# find_package(GUROBI REQUIRED)

# ${GUROBI_INCLUDE_DIRS} : /Library/gurobi1102/macos_universal2/include
include_directories(/Library/gurobi1102/macos_universal2/include)

# list source files here
file(GLOB_RECURSE SRC_LIST ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)
# set(sources src/main.cpp src/PrimaryGraph.cpp src/utils.cpp )

add_executable(${CMAKE_PROJECT_NAME} ${SRC_LIST})


# ${GUROBI_LIBRARY} : /Library/gurobi1102/macos_universal2/lib/libgurobi110.dylib 
# ${GUROBI_CXX_LIBRARY} : /Library/gurobi1102/macos_universal2/lib/libgurobi_c++.a
if(CXX)
  target_link_libraries(${CMAKE_PROJECT_NAME} optimized /Library/gurobi1102/macos_universal2/lib/libgurobi_c++.a
                        debug /Library/gurobi1102/macos_universal2/lib/libgurobi_c++.a)
endif()

target_link_libraries(${CMAKE_PROJECT_NAME} /Library/gurobi1102/macos_universal2/lib/libgurobi110.dylib)



if(${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_CURRENT_SOURCE_DIR})
  include(FeatureSummary)
  feature_summary(WHAT ALL)
endif()
