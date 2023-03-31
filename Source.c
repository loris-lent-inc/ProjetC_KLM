#include "libCIPSI1.h"
void main() {
    IMAGE img1 = lectureImage("Source_Images/In_30.pgm");
    STREL v = V4;
    IMAGE img2 = fermeture(img1, V4);

    sauvegardeImage(img2, "P5", "results/conv1.pgm");

    

    //printf("IoU = %.3f%%, Global Delta = %.3f%%\n", 100*result, 100*GD);
}