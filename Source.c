 #include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "libCIPSI1.h"

int main() {
    const char *folder1 = "./Source_Images/";
    const char *folder2 = "./Ground_truth/";
    const char *result_folder = "./results/test/";
    const char *filename = "file_list.txt";

    FileList fileList;
    fileList.count = 0;
    fileList.countIN = 0;
    fileList.countSc = 0;

    readFilenames(filename, &fileList);
    process_images(folder1, folder2, result_folder, &fileList);
    freeFileList(&fileList);

    return 0;
}