#include "libCIPSI1.h"
void main() {
    IMAGE img1 = lectureImage("Source_images/In_30.pgm");
    STREL v = disk(35, 1);
    IMAGE img2 = whiteTopHat(img1, v);
    IMAGE img3 = convolution(img2, V8());

    sauvegardeImage(img2, "P2", "results/wth.pgm");
    sauvegardeImage(img3, "P2", "results/bth.pgm");

    

    //printf("IoU = %.3f%%, Global Delta = %.3f%%\n", 100*result, 100*GD);
}