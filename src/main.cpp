#include <opencv2/opencv.hpp>
#include <iostream>
#include <vector>
#include <cmath>

using namespace cv;
using namespace std;

struct Vertex
{
  int row;
  int col;
  Vec3b color;
  bool is_seed = false;
};

struct Edge
{
  Vertex to;
  int weight;
};

bool is_inside(int row, int col, int rows, int cols)
{
  return row >= 0 && row < rows && col >= 0 && col < cols;
}

int pixel_index(int row, int col, int cols)
{
  return row * cols + col;
}

int main()
{
  // Setings
  string image_path = "./image.jpg";
  int num_seeds = 100;

  Mat image = imread(image_path, IMREAD_COLOR);

  if (image.empty())
  {
    cout << "Error: Could not open or find the image." << endl;
    return -1;
  }

  int rows = image.rows;
  int cols = image.cols;
  int num_vertices = rows * cols;

  vector<Vertex> vertices(num_vertices);

  for (int r = 0; r < rows; ++r)
  {
    for (int c = 0; c < cols; ++c)
    {
      int idx = pixel_index(r, c, cols);
      Vertex v;
      v.row = r;
      v.col = c;
      v.color = image.at<Vec3b>(r, c);
      v.is_seed = (idx % (num_vertices / num_seeds) == 0);
      vertices[idx] = v;
    }
  }

  vector<vector<Edge>> graph(num_vertices);

  // 4-neighborhood offsets
  int directions_row[] = {-1, 1, 0, 0};
  int directions_col[] = {0, 0, -1, 1};

  // graph construction
  for (int r = 0; r < rows; ++r)
  {
    for (int c = 0; c < cols; ++c)
    {
      int current_idx = pixel_index(r, c, cols);
      Vertex &current_vertex = vertices[current_idx];

      for (int i = 0; i < 4; ++i)
      {
        int neighbor_row = r + directions_row[i];
        int neighbor_col = c + directions_col[i];

        if (is_inside(neighbor_row, neighbor_col, rows, cols))
        {
          int neighbor_idx = pixel_index(neighbor_row, neighbor_col, cols);
          Vertex neighbor_vertex = vertices[neighbor_idx];

          // RGB color difference as weight (Manhattan distance)
          int weight = abs(current_vertex.color[0] - neighbor_vertex.color[0]) +
                       abs(current_vertex.color[1] - neighbor_vertex.color[1]) +
                       abs(current_vertex.color[2] - neighbor_vertex.color[2]);

          graph[current_idx].push_back({neighbor_vertex, weight});
        }
      }
    }
  }

  
  // dijkstra with multiple sources
  vector<int> distances(num_vertices, INT_MAX);
  vector<int> nearest_seed(num_vertices, -1);
  priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;

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
      int v = pixel_index(edge.to.row, edge.to.col, cols);
      int new_dist = distances[u] + edge.weight;
      if (new_dist < distances[v])
      {
        distances[v] = new_dist;
        nearest_seed[v] = nearest_seed[u];
        pq.push({new_dist, v});
      }
    }
  }

  Mat output_image = Mat::zeros(image.size(), image.type());
  for (int r = 0; r < rows; ++r)
  {
    for (int c = 0; c < cols; ++c)
    {
      int idx = pixel_index(r, c, cols);
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

  string output_path = "../results/" + image_path.substr(0, image_path.find_last_of('.')) + "_" + to_string(num_seeds) + ".jpg";
  imwrite(output_path, output_image);

  return 0;
}
