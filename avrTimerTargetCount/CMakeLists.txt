cmake_minimum_required(VERSION 2.8)
project(avrTimer)


# Configure various compilers & settings thereof.
if(${MSVC10})
  Message("Microsoft Visual Studio 10")	
elseif(${MSVC11})
  Message("Microsoft Visual Studio 11")

elseif(${APPLE})
  if(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
  	Message("system matches Darwin and __APPLE__ defined in h files") 
        set(CMAKE_XCODE_ATTRIBUTE_GCC_VERSION "com.apple.compilers.llvm.clang.1_0")  # tell XCode to use llvm, not g++
        set(CMAKE_XCODE_ATTRIBUTE_CLANG_CXX_LANGUAGE_STANDARD "c++0x")
        set(CMAKE_XCODE_ATTRIBUTE_CLANG_CXX_LIBRARY "libc++")
        set(CMAKE_CXX_FLAGS "${CMAKE_C_FLAGS} -std=c++11 -stdlib=libc++ -g -Wall")
  endif()

elseif(${CMAKE_COMPILER_IS_GNUCXX})
  Message("g++ compiler (probably linux)")
  set(CMAKE_INCLUDE_PATH "/usr/include/c++/4.9.1")
  set(CMAKE_CXX_COMPILER "/usr/bin/g++")
  set(CMAKE_CXX_FLAGS "${CMAKE_C_FLAGS} -std=c++11 -g -pthread -Wall")
elseif(${CMAKE_COMPILER_IS_INTEL})
  Message("icpc compiler (probably linux)")
  set(CMAKE_CXX_FLAGS "${CMAKE_C_FLAGS} -std=c++11 -g -pthread -Wall")
endif()

# add the binary tree to the search path for include files
include_directories("${PROJECT_BINARY_DIR}")

set (avrTimer_sources avrTimerRegHelper.cpp)

# the main executable
add_executable(avrTimer ${avrTimer_sources})






