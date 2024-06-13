cmake_minimum_required(VERSION 2.8)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O2 -std=gnu11 ")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2 -std=gnu++11 ")

include_directories(${CMAKE_SOURCE_DIR})
include_directories(${CMAKE_SOURCE_DIR}/include)
include_directories(${CMAKE_SOURCE_DIR}/yangpush)
include_directories(${CMAKE_SOURCE_DIR}/include/yangpush)
include_directories(${CMAKE_SOURCE_DIR}/include/yangutil)
include_directories(${CMAKE_SOURCE_DIR}/thirdparty/include)
include_directories(${CMAKE_SOURCE_DIR}/thirdparty/user_include)
include_directories(${CMAKE_SOURCE_DIR}/thirdparty/user_include/ffmpeg)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -L${CMAKE_SOURCE_DIR}/lib")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -L${CMAKE_SOURCE_DIR}/thirdparty/lib")

file(GLOB SRC_FILE "${CMAKE_SOURCE_DIR}/*.cpp")
file(GLOB PUSH_FILE "${CMAKE_SOURCE_DIR}/yangpush/*.cpp")

# add_library(push-lib
#             STATIC
#             ${SRC_FILE}
#             ${PUSH_FILE}
# )

add_executable(rtcApp ${SRC_FILE} ${PUSH_FILE})

target_link_libraries(
            rtcApp
            
            -Wl,--start-group
            ssl2
            usrsctp
            srtp2
            crypto2
            pthread
            dl
            yuv
            opus
            speexdsp
            openh264
            x264
            asound

            metartccore7
            metartc7
            -Wl,--end-group    
)  