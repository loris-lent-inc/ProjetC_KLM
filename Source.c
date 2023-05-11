#include "libCIPSI1.h"
void main() {
    int n;
    IMAGE img1 = lectureImage("Ground_truth/In_1.pgm");
    IMAGE img2 = lectureImage("Ground_truth/In_7.pgm");
    IMAGE img3 = lectureImage("Source_images/In_3.pgm");
    IMAGE img4 = mediane(img3, V8());
    
    float iou, gd;
    IoU(img1, img2, &iou, &gd);

    printf("Score Vinet : %.3f%% de corresondance.\n\n", 100*Vinet(img1, img2));
    printf("IoU = %.3f%%, Global Delta = %.3f%%\n", 100*iou, 100*gd);

    sauvegardeImage(img4, "P2", "results/con.pgm");
}