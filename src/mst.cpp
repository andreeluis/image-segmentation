#include "include/mst.hpp"
#include "include/utils.hpp"
#include <iostream>
#include <filesystem>
#include <algorithm>
#include <numeric>
#include <random>

using namespace cv;
using namespace std;

// ----------------- Union Find -----------------
MST::UnionFind::UnionFind(int n) : parent(n), size(n, 1), internal_diff(n, 0.0f)
{
	iota(parent.begin(), parent.end(), 0);
}

int MST::UnionFind::find(int x)
{
	if (parent[x] != x)
		parent[x] = find(parent[x]);
	return parent[x];
}

void MST::UnionFind::unite(int a, int b, float weight)
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

int MST::UnionFind::get_size(int x)
{
	return size[find(x)];
}

float MST::UnionFind::get_internal_diff(int x)
{
	return internal_diff[find(x)];
}

// ----------------- MST -----------------
MST::MST(const string &image_path, float k) : k(k)
{
	image = imread(image_path, IMREAD_COLOR);

	if (image.empty())
	{
		throw runtime_error("Error: Could not open or find the image.");
	}
}

Mat MST::segment()
{
	int rows = image.rows;
	int cols = image.cols;
	int num_vertices = rows * cols;

	vector<Vertex> vertices(num_vertices);
	vector<Edge> edges;

	// Inicializar vértices
	for (int r = 0; r < rows; ++r)
		for (int c = 0; c < cols; ++c)
		{
			int idx = pixel_index(r, c, cols);
			vertices[idx] = {r, c, image.at<Vec3b>(r, c)};
		}

	// Construir arestas (4-vizinhança)
	const int dr[] = {-1, 1, 0, 0};
	const int dc[] = {0, 0, -1, 1};

	for (int r = 0; r < rows; ++r)
	{
		for (int c = 0; c < cols; ++c)
		{
			int u = pixel_index(r, c, cols);
			Vec3b cu = image.at<Vec3b>(r, c);

			for (int i = 0; i < 4; ++i)
			{
				int nr = r + dr[i];
				int nc = c + dc[i];

				if (!is_inside(nr, nc, rows, cols))
					continue;

				int v = pixel_index(nr, nc, cols);
				Vec3b cv = image.at<Vec3b>(nr, nc);

				int diff = abs(cu[0] - cv[0]) +
									 abs(cu[1] - cv[1]) +
									 abs(cu[2] - cv[2]);

				edges.push_back({u, v, diff});
			}
		}
	}

	sort(edges.begin(), edges.end());

	UnionFind uf(num_vertices);

	auto threshold = [&](int size)
	{
		return k / size;
	};

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
	output_image = Mat::zeros(image.size(), image.type());

	for (int r = 0; r < rows; ++r)
	{
		for (int c = 0; c < cols; ++c)
		{
			int idx = pixel_index(r, c, cols);
			int root = uf.find(idx);

			if (comp_color.count(root) == 0)
			{
				comp_color[root] = Vec3b(
						rng.uniform(0, 255),
						rng.uniform(0, 255),
						rng.uniform(0, 255));
			}

			output_image.at<Vec3b>(r, c) = comp_color[root];
		}
	}

	return output_image;
}

void MST::save_result(const string &output_path)
{
	namespace fs = std::filesystem;
	fs::create_directories(fs::path(output_path).parent_path());
	imwrite(output_path, output_image);
}

Mat MST::get_result() const
{
	return output_image;
}
