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
		int get_size(int x);
		float get_internal_diff(int x);

	private:
		std::vector<int> parent, size;
		std::vector<float> internal_diff;
	};

	float k;

	cv::Mat image;
	cv::Mat output_image;

public:
	MST(const std::string &image_path, float k = 500.0f);

	cv::Mat segment();
	void save_result(const std::string &output_path);
	cv::Mat get_result() const;
};

#endif // __MST_HPP__
