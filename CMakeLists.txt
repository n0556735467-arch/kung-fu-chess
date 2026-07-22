cmake_minimum_required(VERSION 3.16)
project(KungFuChessGUI)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB_RECURSE MODEL_SOURCES "model/*.cpp")
file(GLOB_RECURSE IO_SOURCES "io/*.cpp")
file(GLOB_RECURSE INPUT_SOURCES "input/*.cpp")
file(GLOB_RECURSE ENGINE_SOURCES "engine/*.cpp")
file(GLOB_RECURSE REALTIME_SOURCES "realtime/*.cpp")
file(GLOB_RECURSE RULES_SOURCES "rules/*.cpp")
file(GLOB_RECURSE VIEW_SOURCES "view/*.cpp")

add_executable(${PROJECT_NAME}
    ${MODEL_SOURCES}
    ${IO_SOURCES}
    ${INPUT_SOURCES}
    ${ENGINE_SOURCES}
    ${REALTIME_SOURCES}
    ${RULES_SOURCES}
    ${VIEW_SOURCES}
)

set(OPENCV_DIR "${CMAKE_CURRENT_SOURCE_DIR}/OpenCV_451")
target_include_directories(${PROJECT_NAME} PRIVATE "${OPENCV_DIR}/include")
target_link_directories(${PROJECT_NAME} PRIVATE "${OPENCV_DIR}/bin")

target_link_libraries(${PROJECT_NAME}
    $<$<CONFIG:Debug>:${OPENCV_DIR}/bin/opencv_world451d.lib>
    $<$<CONFIG:Release>:${OPENCV_DIR}/bin/opencv_world451.lib>
)

if(WIN32)
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        $<$<CONFIG:Debug>:${OPENCV_DIR}/bin/opencv_world451d.dll>
        $<$<CONFIG:Release>:${OPENCV_DIR}/bin/opencv_world451.dll>
        $<TARGET_FILE_DIR:${PROJECT_NAME}>)
endif()