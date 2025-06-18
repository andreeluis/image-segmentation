#include <opencv2/opencv.hpp>
#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <numeric>
#include <map>
#include <random>

using namespace cv;
using namespace std;

struct Vertex
{
  int row, col;
  Vec3b color;
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

struct UnionFind
{
  vector<int> parent, size;
  vector<float> internal_diff;

  UnionFind(int n) : parent(n), size(n, 1), internal_diff(n, 0.0f)
  {
    iota(parent.begin(), parent.end(), 0);
  }

  int find(int x)
  {
    if (parent[x] != x)
      parent[x] = find(parent[x]);
    return parent[x];
  }

  void unite(int a, int b, float weight)
  {
    a = find(a);
    b = find(b);
    if (a == b)
      return;

    if (size[a] < size[b])
      swap(a, b);

    parent[b] = a;
    size[a] += size[b];
    internal_diff[a] = max({internal_diff[a], internal_diff[b], weight});
  }

  int get_size(int x)
  {
    return size[find(x)];
  }

  float get_internal_diff(int x)
  {
    return internal_diff[find(x)];
  }
};

int pixel_index(int row, int col, int cols)
{
  return row * cols + col;
}

bool is_inside(int row, int col, int rows, int cols)
{
  return row >= 0 && row < rows && col >= 0 && col < cols;
}

Mat segment_fh(const Mat &image, float k)
{
  int rows = image.rows, cols = image.cols;
  int num_vertices = rows * cols;
  vector<Vertex> vertices(num_vertices);

  // Inicializar vértices
  for (int r = 0; r < rows; ++r)
    for (int c = 0; c < cols; ++c)
    {
      int idx = pixel_index(r, c, cols);
      vertices[idx] = {r, c, image.at<Vec3b>(r, c)};
    }

  // Construir arestas (4-vizinhança)
  vector<Edge> edges;
  int dr[] = {-1, 1, 0, 0};
  int dc[] = {0, 0, -1, 1};

  for (int r = 0; r < rows; ++r)
    for (int c = 0; c < cols; ++c)
    {
      int u = pixel_index(r, c, cols);
      Vec3b cu = image.at<Vec3b>(r, c);

      for (int i = 0; i < 4; ++i)
      {
        int nr = r + dr[i], nc = c + dc[i];
        if (!is_inside(nr, nc, rows, cols))
          continue;

        int v = pixel_index(nr, nc, cols);
        Vec3b cv = image.at<Vec3b>(nr, nc);

        int diff = abs(cu[0] - cv[0]) + abs(cu[1] - cv[1]) + abs(cu[2] - cv[2]);

        edges.push_back({u, v, diff});
      }
    }

  // Ordenar arestas por peso
  sort(edges.begin(), edges.end());

  // Inicializar Union-Find
  UnionFind uf(num_vertices);

  // Função τ(C) = k / |C|
  auto threshold = [&](int size)
  {
    return k / size;
  };

  // Kruskal modificado (Felzenszwalb-Huttenlocher)
  for (const Edge &e : edges)
  {
    int a = uf.find(e.from);
    int b = uf.find(e.to);

    if (a == b)
      continue;

    float MIntA = uf.get_internal_diff(a) + threshold(uf.get_size(a));
    float MIntB = uf.get_internal_diff(b) + threshold(uf.get_size(b));

    if (e.weight <= min(MIntA, MIntB))
    {
      uf.unite(a, b, e.weight);
    }
  }

  // Atribuir cor por componente
  map<int, Vec3b> comp_color;
  RNG rng(12345);
  Mat output = Mat::zeros(image.size(), image.type());

  for (int r = 0; r < rows; ++r)
    for (int c = 0; c < cols; ++c)
    {
      int idx = pixel_index(r, c, cols);
      int root = uf.find(idx);

      if (comp_color.count(root) == 0)
      {
        comp_color[root] = Vec3b(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));
      }

      output.at<Vec3b>(r, c) = comp_color[root];
    }

  return output;
}

int main()
{
  // Carregar imagem
  Mat image = imread("cat.jpg");
  if (image.empty())
  {
    cerr << "Erro ao carregar a imagem!" << endl;
    return -1;
  }

  // Parâmetro k
  float k = 10000.0f;

  // Segmentação usando Felzenszwalb-Huttenlocher
  Mat segmented_image = segment_fh(image, k);

  // write the segmented image
  imwrite("segmented_image.jpg", segmented_image);

  return 0;
}