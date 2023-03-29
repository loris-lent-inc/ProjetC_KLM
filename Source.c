#include "libCIPSI1.h"
void main() {
    IMAGE img1 = lectureImage("Ground_truth/In_30.pgm");
    IMAGE img2 = lectureImage("Ground_truth/In_21.pgm");

    float result, GD;

    IMAGE res = IoU(img1, img2, &result, &GD);
    sauvegardeImage(res, "P5", "results/test1.pgm");

    //printf("IoU = %.3f%%, Global Delta = %.3f%%\n", 100*result, 100*GD);
}