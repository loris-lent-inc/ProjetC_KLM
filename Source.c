 #include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "libCIPSI1.h"

 void readFilenames(const char *filename, FileList *fileList) {
     FILE *file = fopen(filename, "r");
     if (file == NULL) {
         printf("Error opening file %s\n", filename);
         exit(1);
     }

     char line[100];

     fileList->count = 0;
     fileList->filenames = NULL;

     while (fgets(line, 100, file) != NULL) {
         line[strcspn(line, "\n")] = '\0';

         fileList->count++;

         // Use a temporary pointer for reallocation
         char **temp = (char **)realloc(fileList->filenames, fileList->count * sizeof(char *));

         if (temp == NULL) {
             // Handle reallocation failure
             // Free memory allocated so far and handle the error
             for (int i = 0; i < fileList->count - 1; i++) {
                 free(fileList->filenames[i]);
             }
             free(fileList->filenames);
             fclose(file);
             printf("Error allocating memory\n");
             exit(1);
         }

         fileList->filenames = temp;

         fileList->filenames[fileList->count - 1] = (char *)malloc((strlen(line) + 1) * sizeof(char));
         strcpy(fileList->filenames[fileList->count - 1], line);
     }

     fclose(file);
 }

void process_images(const char *folder1, const char *folder2, const char *result_folder, const FileList *fileList) {
    const int maxPathLength = 100;
    float total_iou = 0;
    float total_vinet = 0;

    for (int i = 0; i < fileList->count; ++i) {
        char *path1 = malloc(maxPathLength * sizeof(char));
        char *path2 = malloc(maxPathLength * sizeof(char));
        char *result_path = malloc(maxPathLength * sizeof(char));

        float result, GD;

        snprintf(path1, maxPathLength, "%s%s", folder1, fileList->filenames[i]);
        snprintf(path2, maxPathLength, "%s%s", folder2, fileList->filenames[i]);
        snprintf(result_path, maxPathLength, "%s%s", result_folder, fileList->filenames[i]);

        printf("______________________________________________________________\n");

        // Operations sur les images
        printf("%s\n", fileList->filenames[i]);
        IMAGE img1 = lectureImage(path1);
        IMAGE img2 = lectureImage(path2);

        STREL strel = disk(3, 1);

        // Morphologique
        img1 = ouverture(dilatation(img1, strel), strel);

        IMAGE img_black_top_hat = blackTopHat(img1, disk(17, 1));
        IMAGE img_white_top_hat = whiteTopHat(img1, disk(17, 1));

        double white_cor = correlation_croisee_normalisee(img_white_top_hat, img2);
        double black_cor = correlation_croisee_normalisee(img_black_top_hat, img2);

        IMAGE img_tophat;

        if (white_cor > black_cor) {
            img_tophat = img_white_top_hat;
            printf("White top hat\n");
        }else {
            img_tophat = img_black_top_hat;
            printf("Black top hat\n");
        }

        // Seuillage Ostu
        IMAGE img_otsu = seuillageOtsu(img_tophat);

        //labelisation
        int nb_labels = 0;
        IMAGE img_label = labelImage(img_otsu, &nb_labels);

        // Supprimer les petits objets en dessous de 23 de surface
        SIGNATURE_COMPOSANTE_CONNEXE *signatures = signaturesImage(img_label, nb_labels);
        for (int k = 1; k <= nb_labels; ++k) {
            //printf("Surface composante %d : %d\n", k, signatures[k].surface);
            if ((signatures[k].surface < 23)) {
                //printf("Suppression composante %d\n", k);
                img_label = supprimerComposanteConnexe(img_label, k);
            }
        }
        free(signatures);

        img_label = labelToBinary(img_label, nb_labels);

        IoU(img_label, img2, &result, &GD);
        float vinet = 0;

        char result_filename[100];
        char results[100];
        snprintf(result_filename, maxPathLength, "%s%s", result_folder, fileList->filenames[i]);
        snprintf(results, maxPathLength, "%s%s", "./results/", fileList->filenames[i]);
        sauvegardeImage(img_otsu, "P5",result_filename);
        sauvegardeImage(img_label, "P5",results);

        printf("IoU = %.3f%%, vinet = %.3f%%\n", 100 * result, 100 * vinet);

        total_iou += result;
        total_vinet += vinet;

        free(path1);
        free(path2);
        free(result_path);
    }
    float avg_iou = total_iou / (float)fileList->count;
    float avg_vinet = total_vinet / (float)fileList->count;

    printf("______________________________________________________________\n");
    printf("Score total:\n");
    printf("IoU = %.3f%%, Vinet = %.3f%%\n", 100 * avg_iou, 100 * avg_vinet);

}

void freeFileList(FileList *fileList) {
    for (int i = 0; i < fileList->count; i++) {
        free(fileList->filenames[i]);
    }
    free(fileList->filenames);
    fileList->count = 0;
}

int main() {
    const char *folder1 = "./Source_Images/";
    const char *folder2 = "./Ground_truth/";
    const char *result_folder = "./results/test/";
    const char *filename = "file_list.txt";

    FileList fileList;
    fileList.count = 0;

    readFilenames(filename, &fileList);
    process_images(folder1, folder2, result_folder, &fileList);
    freeFileList(&fileList);

    return 0;
}