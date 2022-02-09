cmake_minimum_required(VERSION 3.2)
project(mlx90640)

# file(GLOB src_files CONFIGURE_DEPENDS "functions/*.cpp")
file(GLOB inc_files CONFIGURE_DEPENDS "headers/*.h")
set(src_files
    functions/MLX90640_API.cpp
    functions/MLX90640_I2C_Driver.cpp
)
add_library(${PROJECT_NAME} STATIC ${src_files} ${inc_files})
set_target_properties(mlx90640 PROPERTIES LINKER_LANGUAGE CXX)
target_include_directories(${PROJECT_NAME} PUBLIC headers)
