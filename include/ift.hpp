#ifndef __IFT_HPP__
#define __IFT_HPP__

#include <opencv2/opencv.hpp>
#include <vector>
#include <queue>
#include <string>
#include <climits>
#include <cmath>

class IFT
{
private:
    struct Vertex
    {
        int row, col;
        cv::Vec3b color;
        bool is_seed = false;
    };

    struct Edge
    {
        Vertex to;
        int weight;
    };

    // Atributos principais
    cv::Mat image;
    cv::Mat output_image;
    int rows, cols;
    int num_vertices;
    int num_seeds;

    std::vector<Vertex> vertices;
    std::vector<std::vector<Edge>> graph;
    std::vector<int> distances;
    std::vector<int> nearest_seed;

    void buildGraph();
    void computeSegmentation();
    bool isInside(int row, int col) const;
    int pixelIndex(int row, int col) const;

public:
    IFT(const std::string &image_path, int num_seeds = 100);

    // Processa a segmentação
    void run();

    // Salva a imagem segmentada
    void saveResult(const std::string &output_path);

    // Retorna a imagem segmentada
    cv::Mat getResult() const;
};

#endif // __IFT_HPP__
