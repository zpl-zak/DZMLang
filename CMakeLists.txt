cmake_minimum_required(VERSION 3.2)
project(DZMLang)

if(WIN32)
  
else()
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-unused-result -Wno-unused-parameter -std=gnu++11 -Wall -Wextra -Werror")
endif()

set(SOURCE_FILES
    app.cpp)

#set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

add_executable(dzm ${SOURCE_FILES})

if(WIN32)

else()
target_link_libraries(dzm pthread readline m)
endif()
