#ifndef __MST_HPP__
#define __MST_HPP__

#include <opencv2/opencv.hpp>
#include <vector>
#include <map>
#include <string>

class MST
{
private:
    struct Vertex
    {
        int row, col;
        cv::Vec3b color;
    };

    struct Edge
    {
        int from, to;
        int weight;

        bool operator<(const Edge &e) const
        {
            return weight < e.weight;
        }
    };

    class UnionFind
    {
    public:
        UnionFind(int n);
        int find(int x);
        void unite(int a, int b, float weight);
        int getSize(int x);
        float getInternalDiff(int x);

    private:
        std::vector<int> parent, size;
        std::vector<float> internal_diff;
    };

    float k;

    cv::Mat image;
    cv::Mat output_image;

    bool isInside(int row, int col, int rows, int cols) const;
    int pixelIndex(int row, int col, int cols) const;

public:
    MST(const std::string &image_path, float k = 500.0f);

    cv::Mat segment();
    void saveResult(const std::string &output_path);
    cv::Mat getResult() const;
};

#endif // __MST_HPP__
