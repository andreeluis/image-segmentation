#include "include/ift.hpp"
#include "include/mst.hpp"

#include <iostream>

int main(int argc, char* argv[])
{
	int ret = 0;
    if (argc < 4)
    {
        std::cerr << "Usage: " << argv[0] << " <method> <image_path> <value>\n";
        std::cerr << "Methods:\n";
        std::cerr << "  ift : Image Foresting Transform\n";
        std::cerr << "  mst : Minimum Spanning Tree\n";
        std::cerr << "Value:\n";
        std::cerr << "  For IFT : number of seeds (integer)\n";
        std::cerr << "  For MST : k value (float)\n";
		ret = -1;
    }
	else
	{
		std::string method = argv[1];
		std::string image_path = argv[2];

		try
		{
			if (method == "ift")
			{
				int num_seeds = std::stoi(argv[3]);
				IFT ift(image_path, num_seeds);
				ift.run();
				std::string output_path = "results/ift_segmented.jpg";
				ift.saveResult(output_path);
				cv::imshow("IFT Segmented Image", ift.getResult());
				cv::waitKey(0);
			}
			else if (method == "mst")
			{
				float k = std::stof(argv[3]);
				MST mst(image_path, k);
				mst.segment();
				std::string output_path = "results/mst_segmented.jpg";
				mst.saveResult(output_path);
				cv::imshow("MST Segmented Image", mst.getResult());
				cv::waitKey(0);
			}
			else
			{
				std::cerr << "Unknown method: " << method << "\n";
				ret = -1;
			}
		}
		catch (const std::exception& e)
		{
			std::cerr << "Error: " << e.what() << "\n";
			ret = -1;
		}
	}

    return ret;
}
