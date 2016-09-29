
/*

This is an naive exhaustive O(n**2) algorithm for finding optimal patch correspondences between two images. This is mostly used to compare against PatchMatch, which in our experience is much more effective. 

TODO: 

*/

#include <chrono>
#include <cmath>
#include "naive_patch_corresopndence.h"
#include "array.h"
#include "pfm.h"

using std::cout;
using std::endl;
using std::string;

void usage() {
    fprintf(stderr, "\n"
                    "usage: main <input_image_a>.png <input_image_b>.png <output_file>.pfm <options>\n"
                    "\n"
                    "This is a naive patch correspondence algorithm. It is optimal and exhaustive. It takes O(n**2) time. This is mostly used as a benchmark for comparison. For applications that need patch correspondences between images, we recommend using a more time efficient algorithm or an approximation algorithm, e.g. PatchMatch or Propagation-Assisted KD-Trees. \n"
                    "\n"
                    "\n"
                    "Required Parameters: \n"
                    "\n"
                    "    <input_image_a>.png: This is the file name of input image A. Must be a .png file. \n"
                    "\n"
                    "    <input_image_b>.png: This is the file name of input image B. Must be a .png file. \n"
                    "\n"
                    "    <output_file>.pfm: This string is the name of the desired output file. This will be a .pfm file containing the nearest neighbor field. The output file will be 3 dimensional. It will have the same X and Y dimensions as input image A (both minus the size of patch_dim). It's Z dimension will have length 3 (to describe the X and Y coordinates of input image b that represents the nearest neighbor patch as well as the patch distance). Patches of size patch_dim by patch_dim will be referred to by their upper left coordinate. In order to get the coordinates (x_b,y_b) in input image b that correspond to the coordinates (x_a,y_a) in input image A using this output file, we will have to use a pfm reader to extract the values at output_file[y_a,x_a,0] to get x_b and output_file[y_a,x_a,1] to get y_b. \n"
                    "\n"
                    "\n"
                    "Options: \n"
                    "\n"
                    "    -patch_dim <patch_dim>: This int value determines the size of the patches. The patches will be size <patch_dim> by <patch_dim>. \n"
                    "\n"
                    "\n"
           );
    exit(1);
}

int main(int argc, char* argv[]) {
    
    if (argc<4) {
        usage();
    }

    std::srand( std::time(NULL) );

    static const char* A_name = argv[1];
    static const char* B_name = argv[2];
    static const char* output_name = argv[3];
    static const int patch_dim = atoi(get_command_line_param_val_default_val(argc, argv, "-patch_dim", "5"));
    
    static const png::image< png::rgba_pixel > A_image(A_name); 
    static const png::image< png::rgba_pixel > B_image(B_name); 
    static const int A_height = A_image.get_height();
    static const int A_width = A_image.get_width();
    static const int B_height = B_image.get_height();
    static const int B_width = B_image.get_width();
    static const int Ann_height = A_height-patch_dim+1;
    static const int Ann_width = A_width-patch_dim+1;
    static const Array<byte> A(A_image);
    static const Array<byte> B(B_image);
    Array<int> Ann(vector<int>{Ann_height, Ann_width, 3});
    
    NEWLINE;
    PRINT("Parameter Values");
    TEST(A_height);
    TEST(A_width);
    TEST(B_height);
    TEST(B_width);
    TEST(Ann_height);
    TEST(Ann_width);
    TEST(patch_dim);
    
    long total_patch_distance;
    double mean_patch_distance;
    
    naive_patch_corresopndence(A, B, Ann, A_height, A_width, B_height, B_width, Ann_height, Ann_width, patch_dim, total_patch_distance, mean_patch_distance);
    
    // Write output to .pfm file
    
    float *depth = new float[Ann_height*Ann_width*3];
    for (int y = 0; y < Ann_height; y++) {
        for (int x = 0; x < Ann_width; x++) {
            int i = (Ann_height-1-y)*Ann_width*3+x*3;
            
            depth[i] = FLOAT(Ann(y,x,X_COORD));
            depth[i+1] = FLOAT(Ann(y,x,Y_COORD));
            depth[i+2] = FLOAT(Ann(y,x,D_COORD));
        }
    }
    write_pfm_file3(output_name, depth, Ann_width, Ann_height);
    
    NEWLINE;
    cout << "Final Total Patch Distance: " << total_patch_distance << endl;
    cout << "Final Mean Patch Distance:  " << mean_patch_distance << endl;
    
    return 0;
}

