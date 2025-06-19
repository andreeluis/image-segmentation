<h1 align="center">
	Graph-Based Image Segmentation
</h1>

<p align="center">
	<img alt="C++ Badge" src="https://img.shields.io/badge/C++-%2300599c?style=for-the-badge&logo=cplusplus&logoColor=white">
  <img alt="Computer Vision Badge" src="https://img.shields.io/badge/Computer%20Vision-%230d1117?style=for-the-badge">
</p>

This project provides image segmentation using **graph-based algorithms**. Two state-of-the-art methods are implemented: one based on **Minimum Spanning Tree (MST)**, following the approach of Felzenszwalb and Huttenlocher (2004), and another based on **Shortest Path**, inspired by the Image Foresting Transform (IFT) proposed by Falcão et al. (2004). Both methods support grayscale and color images.

## Quick Start

> [!WARNING]
> This project requires **CMake** and **OpenCV** to be installed on your system.

For a quick start, you can clone the repository and run the provided CMake build system.

```bash
git clone https://github.com/andreeluis/image-segmentation
cd image-segmentation
make all
```
> [!TIP]
> If you prefer not to use CMake, you can compile the code directly with `g++` as shown below.
> ```bash
> g++ -std=c++17 src/*.cpp -Iinclude `pkg-config --cflags --libs opencv4` -o main
> ```

### Usage

```bash
# For IFT segmentation with 250 seeds
./main ift images/painted_cat.png 250

# For MST segmentation with k=8000
./main mst images/bw_cat.png 8000
```
## Project Structure

```
/image-segmentation
 ├── Makefile
 ├── src/
 │   ├── include/
 │   │   ├── ift.hpp
 │   │   ├── mst.hpp
 │   │   └── utils.hpp
 │   ├── ift.cpp
 │   ├── mst.cpp
 │   ├── utils.cpp
 │   └── main.cpp
 ├── images/
 │    ├── bw_cat.png
 │    └── painted_cat.png
 ├── results/
```