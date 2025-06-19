#include "..include/ift.hpp"
#include <iostream>
#include <filesystem>

using namespace cv;
using namespace std;

IFT::IFT(const string &image_path, int num_seeds)
    : num_seeds(num_seeds)
{
    image = imread(image_path, IMREAD_COLOR);

    if (image.empty())
    {
        throw runtime_error("Error: Could not open or find the image.");
    }

    rows = image.rows;
    cols = image.cols;
    num_vertices = rows * cols;

    vertices.resize(num_vertices);
    graph.resize(num_vertices);

    for (int r = 0; r < rows; ++r)
    {
        for (int c = 0; c < cols; ++c)
        {
            int idx = pixelIndex(r, c);
            Vertex v;
            v.row = r;
            v.col = c;
            v.color = image.at<Vec3b>(r, c);
            v.is_seed = (idx % (num_vertices / num_seeds) == 0);
            vertices[idx] = v;
        }
    }
}

bool IFT::isInside(int row, int col) const
{
    return row >= 0 && row < rows && col >= 0 && col < cols;
}

int IFT::pixelIndex(int row, int col) const
{
    return row * cols + col;
}

void IFT::buildGraph()
{
    const int directions_row[] = {-1, 1, 0, 0};
    const int directions_col[] = {0, 0, -1, 1};

    for (int r = 0; r < rows; ++r)
    {
        for (int c = 0; c < cols; ++c)
        {
            int current_idx = pixelIndex(r, c);
            const Vertex &current_vertex = vertices[current_idx];

            for (int i = 0; i < 4; ++i)
            {
                int neighbor_row = r + directions_row[i];
                int neighbor_col = c + directions_col[i];

                if (isInside(neighbor_row, neighbor_col))
                {
                    int neighbor_idx = pixelIndex(neighbor_row, neighbor_col);
                    const Vertex &neighbor_vertex = vertices[neighbor_idx];

                    int weight = abs(current_vertex.color[0] - neighbor_vertex.color[0]) +
                                 abs(current_vertex.color[1] - neighbor_vertex.color[1]) +
                                 abs(current_vertex.color[2] - neighbor_vertex.color[2]);

                    graph[current_idx].push_back({neighbor_vertex, weight});
                }
            }
        }
    }
}

void IFT::computeSegmentation()
{
    distances.assign(num_vertices, INT_MAX);
    nearest_seed.assign(num_vertices, -1);

    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<>> pq;

    for (int i = 0; i < num_vertices; ++i)
    {
        if (vertices[i].is_seed)
        {
            distances[i] = 0;
            nearest_seed[i] = i;
            pq.push({0, i});
        }
    }

    while (!pq.empty())
    {
        auto [dist, u] = pq.top();
        pq.pop();

        if (dist > distances[u]) continue;

        for (const auto &edge : graph[u])
        {
            int v = pixelIndex(edge.to.row, edge.to.col);
            int new_dist = distances[u] + edge.weight;

            if (new_dist < distances[v])
            {
                distances[v] = new_dist;
                nearest_seed[v] = nearest_seed[u];
                pq.push({new_dist, v});
            }
        }
    }
}

void IFT::run()
{
    buildGraph();
    computeSegmentation();

    output_image = Mat::zeros(image.size(), image.type());

    for (int r = 0; r < rows; ++r)
    {
        for (int c = 0; c < cols; ++c)
        {
            int idx = pixelIndex(r, c);
            if (nearest_seed[idx] != -1)
            {
                output_image.at<Vec3b>(r, c) = vertices[nearest_seed[idx]].color;
            }
            else
            {
                output_image.at<Vec3b>(r, c) = Vec3b(0, 0, 0);
            }
        }
    }
}

void IFT::saveResult(const string &output_path)
{
    namespace fs = std::filesystem;
    fs::create_directories(fs::path(output_path).parent_path());
    imwrite(output_path, output_image);
}

Mat IFT::getResult() const
{
    return output_image;
}
