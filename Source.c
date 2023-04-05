#include "libCIPSI1.h"
void main() {
    IMAGE img1 = lectureImage("Source_images/In_30.pgm");
    STREL v = disk(31, 1);
    img1 = convolution(img1, disk(3, 1.0f/5));
    IMAGE img2 = difference(erosion(dilatation(img1, v), v), img1);
    IMAGE img3 = blackTopHat(img1, v);

    sauvegardeImage(img1, "P2", "results/con.pgm");
    sauvegardeImage(img2, "P2", "results/wth.pgm");
    sauvegardeImage(img3, "P2", "results/bth.pgm");

    

    //printf("IoU = %.3f%%, Global Delta = %.3f%%\n", 100*result, 100*GD);
}