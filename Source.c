#include "libCIPSI1.h"
void main() {
    IMAGE img1 = lectureImage("Ground truth/In_1.pgm");
    printf("%d", img1.Nbcol);
}