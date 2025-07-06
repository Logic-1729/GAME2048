cmake_minimum_required(VERSION 3.16)
project(Game2048)

# 设置C++标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 设置输出目录到bin文件夹
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin")

# MinGW交叉编译配置（如果在WSL/Linux环境下）
if(UNIX AND NOT APPLE)
    find_program(MINGW_GCC x86_64-w64-mingw32-gcc)
    find_program(MINGW_GXX x86_64-w64-mingw32-g++)
    
    if(MINGW_GCC AND MINGW_GXX)
        set(CMAKE_SYSTEM_NAME Windows)
        set(CMAKE_C_COMPILER ${MINGW_GCC})
        set(CMAKE_CXX_COMPILER ${MINGW_GXX})
        set(WIN32 TRUE)
        # 设置静态链接选项
        set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static -static-libgcc -static-libstdc++")
    endif()
endif()

# 编译选项
if(WIN32)
    add_compile_options(-mwindows)
    add_compile_definitions(WIN32_LEAN_AND_MEAN NOMINMAX)
    # 静态链接标准库，避免运行时依赖
    add_link_options(-static-libgcc -static-libstdc++)
endif()

# 源文件
set(SOURCES
    src/main.cpp
    src/Game2048.cpp
    src/Game2048_Logic.cpp
    src/Game2048_UI.cpp
    src/Game2048_Data.cpp
    src/Game2048_Messages.cpp
)

# 创建可执行文件
add_executable(Game2048 WIN32 ${SOURCES})

# 头文件目录
target_include_directories(Game2048 PRIVATE include)

# 链接库
target_link_libraries(Game2048 PRIVATE 
    user32 gdi32 kernel32 shell32 comctl32 version)

# 设置输出名称
set_target_properties(Game2048 PROPERTIES OUTPUT_NAME "2048")
