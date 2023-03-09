#include "libCIPSI1.h"
void main() {
    IMAGE img1 = lectureImage("/Ground_truth/In_1.pgm");
    IMAGE img2 = lectureImage("Ground_truth/In_1.pgm");

    float result;

    IMAGE res = IoU(img1, img2, &result);
    sauvegardeImage(res, "P5", "results/test1.pgm");

    printf("%.3f", 100*result);
}