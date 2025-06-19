#include "include/ift.hpp"
#include "include/mst.hpp"
#include <iostream>

using namespace std;
using namespace cv;

int main(int argc, char *argv[])
{
	int ret = 0;
	if (argc < 4)
	{
		cerr << "Usage: " << argv[0] << " <method> <image_path> <value>\n";
		cerr << "Methods:\n";
		cerr << "  ift : Image Foresting Transform\n";
		cerr << "  mst : Minimum Spanning Tree\n";
		cerr << "Value:\n";
		cerr << "  For IFT : number of seeds (integer)\n";
		cerr << "  For MST : k value (float)\n";
		ret = -1;
	}
	else
	{
		string method = argv[1];
		transform(method.begin(), method.end(), method.begin(), ::tolower);
		if (method != "ift" && method != "mst")
		{
			cerr << "Invalid method: " << method << "\n";
			cerr << "Valid methods are: ift, mst\n";
			return -1;
		}
		
		string image_path = argv[2];		
		string output_path = "results/" + method + "/" + image_path.substr(image_path.find_last_of("/\\") + 1, image_path.find_last_of('.') - image_path.find_last_of("/\\") - 1) + "_" + argv[3] + ".png";

		try
		{
			if (method == "ift")
			{
				int num_seeds = stoi(argv[3]);
				IFT ift(image_path, num_seeds);
				ift.run();
				ift.save_result(output_path);
			}
			else if (method == "mst")
			{
				float k = stof(argv[3]);
				MST mst(image_path, k);
				mst.segment();
				mst.save_result(output_path);
			}
			else
			{
				cerr << "Unknown method: " << method << "\n";
				ret = -1;
			}
		}
		catch (const exception &e)
		{
			cerr << "Error: " << e.what() << "\n";
			ret = -1;
		}
	}

	return ret;
}
