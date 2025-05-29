cmake_minimum_required(VERSION 3.10)
project(GndG3.0)

set(CMAKE_CXX_STANDARD 17)

# === GLAD ===
add_library(glad STATIC external/glad/src/glad.c)
target_include_directories(glad PUBLIC external/glad/include)

# === GLFW ===
add_subdirectory(external/glfw)

# === TinyGLTF ===
add_library(tinygltf STATIC external/tinygltf/tiny_gltf.cc)
target_include_directories(tinygltf PUBLIC external/tinygltf)

# === Executable ===
add_executable(GndG3.0
    src/main.cpp
    src/loader.cpp
    src/shader_utils.cpp
)

# === Include directories ===
target_include_directories(GndG3.0 PRIVATE
    include
    external/glad/include
    external/glfw/include
    external/tinygltf
    external/glm
)

# === Link libraries ===
target_link_libraries(GndG3.0 PRIVATE
    glad
    glfw
    tinygltf
    ${CMAKE_DL_LIBS}
)

# === Optional: copy shader files on build ===
file(GLOB SHADERS "shaders/*.vert" "shaders/*.frag")
foreach(shader ${SHADERS})
    configure_file(${shader} ${CMAKE_CURRENT_BINARY_DIR}/shaders COPYONLY)
endforeach()
