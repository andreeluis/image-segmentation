cmake_minimum_required(VERSION 3.10)
project(ImageSegmentation)

set(CMAKE_CXX_STANDARD 17)

# Encontrar OpenCV
find_package(OpenCV REQUIRED)

# Incluir diretórios de headers
include_directories(${OpenCV_INCLUDE_DIRS} include)

# Adicionar os arquivos fontes
add_executable(segmenter
    src/main.cpp
    src/ift.cpp
    src/mst.cpp
)

# Linkar com OpenCV
target_link_libraries(segmenter ${OpenCV_LIBS})
