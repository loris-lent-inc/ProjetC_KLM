#pragma once
#include <stdio.h>
//#include "libOutils.h"

/* librairie IPSI1 */

#define MAX(a, b) a > b ? a : b
#define MIN(a, b) a > b ? b : a
#define maxPathLength 100

//#define V4 diamond(3)
//#define V8 full(3, 1.0f/9)

typedef struct point {
	float x;
	float y;
}POINT;

typedef struct image {
	int   Nblig;
	int   Nbcol;
	unsigned char*  data;
	unsigned char** pixel;
} IMAGE;

typedef struct rgb {
	unsigned char R;
	unsigned char G;
	unsigned char B;
} RGB;

typedef struct imageRGB {
	int   Nblig;
	int   Nbcol;
	RGB*  data;
	RGB** pixel;
} IMAGERGB;

typedef struct signatures {
	int		min;
	int		max;
	double	moyenne;
	double	ecartType;
	int		mediane;
} SIGNATURES;

typedef struct region {
	int x;
	int y;
	int width;
	int height;
} REGION;

typedef struct signature_composantes_connexes {
	int surface;
	float perimetre;
	float compacite;
	int bord;
	POINT CG;
	float rayon;
	REGION region;
} SIGNATURE_COMPOSANTE_CONNEXE;

typedef struct element_structurant {
	int Nblig;
	int Nbcol;
	int xradius;
	int yradius;
	float* data;
	float** pixel;
}STREL;

typedef struct voisinage {
	int nb;
	unsigned char* pixels;
}VOISINAGE;

typedef struct {
    char **filenames;
    int count;
    int countIN;
    int countSc;
} FileList;

int randInt(int min, int max);

POINT* imageVersPoints(IMAGE img, int* n, char axe);

void regression(POINT* tab, int n, float* a, float* b);

IMAGE imageSortieRegression(IMAGE img, char axe);

IMAGE allocationImage(int Nblig, int Nbcol);
STREL allocationStrel(int Nblig, int Nbcol);

void initialisationAleatoireImage(IMAGE img, int ngMin, int ngMax);

void sauvegardeImage(IMAGE img, const char *type, const char *out);
void liberationImage(IMAGE *im);

void sauvegerdeSTREL(STREL strel, const char *out);

IMAGE lectureImage(const char *nom);

IMAGERGB allocationImageRGB(int Nblig, int Nbcol);
IMAGERGB lectureImageRGB(const char *nom);

void sauvegardeImageRGB(IMAGERGB img, const char *type, const char *out);
void liberationImageRGB(IMAGERGB *im);

IMAGE planImage(IMAGERGB img, int plan);
IMAGE luminanceImage(IMAGERGB img, float r, float g, float b);
IMAGE inverseImage(IMAGE img);
IMAGE seuillageImage(IMAGE img, int sInf, int sSup);

int* histogrammeImage(IMAGE img, int choix, int n);
int* histogrammeImageRGB(IMAGERGB img, int choix, int n);
IMAGE expansionImage(IMAGE img, int outMin, int outMax);
IMAGE egalisationImage(IMAGE img);
SIGNATURES statistiquesImage(IMAGE img);

IMAGE seuillageOtsu(IMAGE img);

IMAGE labelImage(IMAGE img, int *nbComp); /* image label + nb composantes connexes */

int* lectureFicherFausseCouleurs(char * lutFile);

IMAGERGB conversionImageFausseCouleur(IMAGE img, char* lutFile);

double distanceEcartTypeImage(IMAGE im1, IMAGE im2);

double distanceHistogrammeImage(IMAGE im1, IMAGE im2);
double distanceHistogrammeImageRGB(IMAGERGB im1, IMAGERGB im2);

void LectureListing(int* nbImages, char** titres);

int choixUser(int min, int max);

IMAGE bruitAleatoireImage(IMAGE img, int amplitude);
IMAGERGB bruitAleatoireImageRGB(IMAGERGB img, int amplitude);

IMAGERGB masqueIMAGE(IMAGE img, IMAGERGB masque);

STREL diamond(int taille, float valeur);
STREL disk(int taille, float valeur);
STREL full(int taille, float valeur);
STREL V4();
STREL V8();

IMAGE convolution(IMAGE img, STREL strel);
IMAGE erosion(IMAGE img, STREL strel);
IMAGE dilatation(IMAGE img, STREL strel);
IMAGE mediane(IMAGE img, STREL strel);
inline IMAGE fermeture(IMAGE img, STREL strel){ return erosion(dilatation(img, strel), strel); }
inline IMAGE ouverture(IMAGE img, STREL strel){ return dilatation(erosion(img, strel), strel); }
IMAGE difference(IMAGE img1, IMAGE img2);
inline IMAGE whiteTopHat(IMAGE img, STREL strel) { return convolution(difference(img, dilatation(erosion(img, strel), strel)), V8()); }
inline IMAGE blackTopHat(IMAGE img, STREL strel) { return convolution(difference(erosion(dilatation(img, strel), strel), img), V8()); }

float perimetre(IMAGE img, int sig);

void rechercheImageProche(int indexIm, char** titles, int nbi, int* indexM11inET, int* indexMinHS, int* minET, int* minHS);

IMAGE applicateurLUTVal(IMAGE img, int* LUT);
void applicateurLUTRef(IMAGE* img, int* LUT);

SIGNATURE_COMPOSANTE_CONNEXE* signaturesImage(IMAGE img, int nbComp);
IMAGE supprimerComposanteConnexe(IMAGE img, int numComp);
IMAGE labelToBinary(IMAGE img, int numComp);
void sauvegardeSignaturesImage(SIGNATURE_COMPOSANTE_CONNEXE* sig, int nbComp, const char* fichier);

int** rechercheTrou(SIGNATURE_COMPOSANTE_CONNEXE* signObjet, SIGNATURE_COMPOSANTE_CONNEXE* signTrou, int nbObjets, int NbTrou, int *NbGroupes);

void discriminationTrous(IMAGE img, IMAGE* res, int** configTrous, int NbObjets, int* NbGroups);

inline float distanceSQ(POINT p1, POINT p2) {
	return (p1.x - p2.x) * (p1.x - p2.x) + (p1.y - p2.y) * (p1.y - p2.y);
}

inline char belongTo(POINT p, REGION reg) {
	if (p.x > reg.x && p.y > reg.y && p.x < reg.x + reg.width && p.y < reg.y + reg.height) {
		return 1;
	}
	return 0;
};

// Fonction de notation de resultat
// retourne une image avec les différences et 2 mesures de ressemblance (une locale, une globale)
float IoU(IMAGE i1, IMAGE i2);

float localIoU(IMAGE test, IMAGE ref, REGION reg);
void sort(VOISINAGE* v);
unsigned char medianeVoisinage(VOISINAGE v);
float Vinet(IMAGE i1, IMAGE i2);


// Alloue le pointeur pour le voisinage
VOISINAGE allocVois(STREL strel);

// Cherche les pixels faisant partie du voisinage du pixel (x, y) dans l'img
VOISINAGE voisinage(IMAGE img, int x, int y, STREL strel);

// Retourne la valeur correspondant au type (min, max, average, ...) dans le voisinage associé 
unsigned char getVal(VOISINAGE v, char* type);

double correlation_croisee_normalisee(IMAGE img, IMAGE imgRef);
double calculateSSIM(IMAGE img1, IMAGE img2);

//
void readFilenames(const char* filename, FileList* fileList);
void freeFileList(FileList* fileList);
void process_images(const char* folder1, const char* folder2, const char* result_folder, FileList* fileList);
IMAGE traitement_image(IMAGE img1, IMAGE img2, float* iou, float* vinet);

//
void menu();