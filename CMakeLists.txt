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
    server/bus.cpp
    server/snapshot_codec.cpp
)

target_compile_definitions(${PROJECT_NAME} PRIVATE
    ASSETS_DIR=\"${CMAKE_CURRENT_SOURCE_DIR}/assets\"
)

set(OPENCV_DIR "${CMAKE_CURRENT_SOURCE_DIR}/OpenCV_451")
target_include_directories(${PROJECT_NAME} PRIVATE "${OPENCV_DIR}/include")
target_link_directories(${PROJECT_NAME} PRIVATE "${OPENCV_DIR}/bin")

target_link_libraries(${PROJECT_NAME}
    $<$<CONFIG:Debug>:${OPENCV_DIR}/bin/opencv_world451d.lib>
    $<$<CONFIG:Release>:${OPENCV_DIR}/bin/opencv_world451.lib>
    ixwebsocket
)

if(WIN32)
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        $<$<CONFIG:Debug>:${OPENCV_DIR}/bin/opencv_world451d.dll>
        $<$<CONFIG:Release>:${OPENCV_DIR}/bin/opencv_world451.dll>
        $<TARGET_FILE_DIR:${PROJECT_NAME}>)
endif()

# --- OpenCV-dependent unit tests (same MSVC/OpenCV toolchain as the GUI) ---
add_executable(TestJsonAnimationConfigProvider
    tests/unit/test_json_animation_config_provider.cpp
    view/json_animation_config_provider.cpp
)

target_include_directories(TestJsonAnimationConfigProvider PRIVATE
    "${OPENCV_DIR}/include"
    tests/unit
)

target_link_directories(TestJsonAnimationConfigProvider PRIVATE "${OPENCV_DIR}/bin")

target_link_libraries(TestJsonAnimationConfigProvider
    $<$<CONFIG:Debug>:${OPENCV_DIR}/bin/opencv_world451d.lib>
    $<$<CONFIG:Release>:${OPENCV_DIR}/bin/opencv_world451.lib>
)

if(WIN32)
    add_custom_command(TARGET TestJsonAnimationConfigProvider POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        $<$<CONFIG:Debug>:${OPENCV_DIR}/bin/opencv_world451d.dll>
        $<$<CONFIG:Release>:${OPENCV_DIR}/bin/opencv_world451.dll>
        $<TARGET_FILE_DIR:TestJsonAnimationConfigProvider>)
endif()

include(FetchContent)
FetchContent_Declare(
    ixwebsocket
    GIT_REPOSITORY https://github.com/machinezone/IXWebSocket.git
    GIT_TAG v11.4.5
)
set(USE_TLS OFF CACHE BOOL "" FORCE)

set(USE_TLS OFF CACHE BOOL "" FORCE)
set(USE_ZLIB OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(ixwebsocket)

add_executable(EchoServer server/echo_server_main.cpp)
target_link_libraries(EchoServer PRIVATE ixwebsocket)

add_executable(EchoClient server/echo_client_main.cpp)
target_link_libraries(EchoClient PRIVATE ixwebsocket)


add_executable(ChessServer
    server/chess_server_main.cpp
    server/game_session.cpp
    server/role_assigner.cpp
    server/command_parser.cpp
    server/snapshot_codec.cpp
    server/player_registry.cpp
    server/bus.cpp
    engine/game_engine.cpp
    io/board_parser.cpp
    io/board_printer.cpp
    model/board.cpp
    model/piece.cpp
    model/position.cpp
    rules/rule_engine.cpp
    rules/piece_rules.cpp
    realtime/real_time_arbiter.cpp
    realtime/motion.cpp
    realtime/jump.cpp
)
    
target_link_libraries(ChessServer PRIVATE ixwebsocket)