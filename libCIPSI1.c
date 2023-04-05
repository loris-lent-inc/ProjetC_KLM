// libCIPSI1.c�
// Hubert Konik - fili�re IPSI

#pragma once
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#include "libCIPSI1.h"

#define PI 3.1415926535
/* static -> non extern */

typedef struct
{
	int Nblig;
	int Nbcol;
	int *data;
	int **pixel;
} intIMAGE;


//int randInt(int min, int max)
//{
//	return min + ((float)rand() / RAND_MAX) * (max - min);
//}

static intIMAGE allocationIntImage(int Nblig, int Nbcol)
{
	intIMAGE mat = { 0,0,NULL,NULL };
	int i;

	mat.Nblig = Nblig;
	mat.Nbcol = Nbcol;
	mat.data = (int*)calloc(Nblig*Nbcol, sizeof(int)); /* mise � 0 par d�faut, gestion des bords lors de l'agrandissement op�r� dans l'�tiquetage */
	if (mat.data == NULL)
		return(mat);
	mat.pixel = (int**)malloc(Nblig*sizeof(int*));
	if (mat.pixel == NULL) {
		free(mat.data);
		mat.data = NULL;
		return(mat);
	}
	for (i = 0; i<Nblig; i++)
		mat.pixel[i] = &mat.data[i*Nbcol];

	return(mat);
}

static void liberationIntImage(intIMAGE *img)
{
	if (img->data != NULL) {
		free(img->data);
		img->data = NULL;
	}
	if (img->pixel != NULL) {
		free(img->pixel);
		img->pixel = NULL;
	}
}

/* fonctionnalit�s -> exportables */

IMAGE allocationImage(int Nblig, int Nbcol)
{
	IMAGE mat = { 0,0,NULL,NULL };
	int i;

	mat.Nblig = Nblig;
	mat.Nbcol = Nbcol;
	mat.data = (unsigned char*)calloc(Nblig*Nbcol, sizeof(unsigned char));
	if (mat.data == NULL)
		return(mat);
	mat.pixel = (unsigned char**)malloc(Nblig*sizeof(unsigned char*));
	if (mat.pixel == NULL) {
		free(mat.data);
		mat.data = NULL;
		return(mat);
	}
	for (i = 0; i<Nblig; i++)
		mat.pixel[i] = &mat.data[i*Nbcol];

	return(mat);
}

STREL allocationStrel(int Nblig, int Nbcol)
{
	if (!(Nblig % 2) || !(Nbcol % 2)) {
		printf("Taille d'élément structurant invalide: il doit s'agir d'un rectangle de cote impair.\n");
		return;
	}
	;
	STREL strel = { 0,0,NULL,NULL };
	int i;

	strel.Nblig = Nblig;
	strel.Nbcol = Nbcol;
	strel.xradius = 1 + Nblig >> 1;
	strel.yradius = 1 + Nbcol >> 1;
	strel.data = (float*)calloc(Nblig * Nbcol, sizeof(float));
	if (strel.data == NULL)
		return(strel);
	strel.pixel = (float**)malloc(Nblig * sizeof(float*));
	if (strel.pixel == NULL) {
		free(strel.data);
		strel.data = NULL;
		return(strel);
	}
	for (i = 0; i < Nblig; i++)
		strel.pixel[i] = &strel.data[i * Nbcol];

	return(strel);
}

void initialisationAleatoireImage(IMAGE img, int ngMin, int ngMax)
{
	int i, j;
	printf("Dans initialisation : img -> %p img.data -> %p img.pixel -> %p\n", &img, img.data, img.pixel);

	for (i = 0; i<img.Nblig; i++)
		for (j = 0; j<img.Nbcol; j++)
			img.pixel[i][j] = (unsigned char)(ngMin + rand() % (ngMax + 1 - ngMin));

	/* solution avec un seul indice
	for (i=0;i<img.Nblig*img.Nbcol;i++)
	img.data[i] = (unsigned char)(ngMin + rand() % (ngMax + 1 - ngMin));
	*/
}

void liberationImage(IMAGE *img)
{
	if (img->data != NULL) {
		free(img->data);
		img->data = NULL;
	}
	if (img->pixel != NULL) {
		free(img->pixel);
		img->pixel = NULL;
	}
}

IMAGE lectureImage(const char *in)
{
	FILE *F = NULL;
	IMAGE img = { 0,0,NULL };
	int dynamique = 0;

	if ((F = fopen(in, "r")) == NULL)
	{
		printf("Pb image inexistante");
	}
	else
	{
		char type[3];

		fgets(type, 3, F);
		fclose(F);
		/* selon le type ouverture binaire ou texte */
		if (strcmp(type, "P2") == 0)  /* cas ASCII niveaux de gris */
		{
			char buf;

			F = fopen(in, "r");

			/* lecture caract�re apr�s caract�re compte-tenu de la diversit� des ent�tes possibles */
			fscanf(F, "%c", &type[0]);
			fscanf(F, "%c", &type[1]);
			fscanf(F, "%c", &buf); /* caract�re espacement */

			fscanf(F, "%c", &buf);
			if (buf == '#') {
				/* on ignore tout jusqu'� trouver '\n' */
				while (buf != '\n')
					fscanf(F, "%c", &buf);
				fscanf(F, "%c", &buf);
			}
			while (((buf - '0') >= 0) && ((buf - '0') <= 9)) { /* possibilit� d'utiliser �galement isdigit de la librairie standard <ctype.h> */
				img.Nbcol = img.Nbcol * 10 + (buf - '0');
				fscanf(F, "%c", &buf);
			}

			fscanf(F, "%c", &buf);
			if (buf == '#') {
				/* on ignore tout jusqu'� trouver '\n' */
				while (buf != '\n')
					fscanf(F, "%c", &buf);
				fscanf(F, "%c", &buf);
			}
			while (((buf - '0') >= 0) && ((buf - '0') <= 9)) {
				img.Nblig = img.Nblig * 10 + (buf - '0');
				fscanf(F, "%c", &buf);
			}

			fscanf(F, "%c", &buf);
			if (buf == '#') {
				/* on ignore tout jusqu'� trouver '\n' */
				while (buf != '\n')
					fscanf(F, "%c", &buf);
				fscanf(F, "%c", &buf);
			}
			while (((buf - '0') >= 0) && ((buf - '0') <= 9)) {
				dynamique = dynamique * 10 + (buf - '0');
				fscanf(F, "%c", &buf);
			}

			/* d�but des data */

			printf("Lecture image NG type %s avec %d lignes et %d colonnes...\n", type, img.Nblig, img.Nbcol);

			/* taille connue, allocation dynamique possible */
			img = allocationImage(img.Nblig, img.Nbcol);

			/* lecture pixel par pixel */
			{
				int i, j;
				int tmp;

				for (i = 0; i<img.Nblig; i++)
					for (j = 0; j<img.Nbcol; j++)
					{
						fscanf(F, "%d", &tmp);
						img.pixel[i][j] = (unsigned char)tmp;
					}
			}
		}
		else
			if (strcmp(type, "P5") == 0)  /* cas brut niveaux de gris */
			{
				char buf;

				F = fopen(in, "rb");

				/* lecture caract�re apr�s caract�re compte-tenu de la diversit� des ent�tes possibles */
				type[0] = fgetc(F);
				type[1] = fgetc(F);
				buf = fgetc(F); /* caract�re espacement */

				buf = fgetc(F);
				if (buf == '#') {
					/* on ignore tout jusqu'� trouver '\n' */
					while (buf != '\n')
						buf = fgetc(F);
					buf = fgetc(F);
				}
				while (((buf - '0') >= 0) && ((buf - '0') <= 9)) { /* possibilit� d'utiliser �galement isdigit de la librairie standard <ctype.h> */
					img.Nbcol = img.Nbcol * 10 + (buf - '0');
					buf = fgetc(F);
				}

				buf = fgetc(F);
				if (buf == '#') {
					/* on ignore tout jusqu'� trouver '\n' */
					while (buf != '\n')
						buf = fgetc(F);
					buf = fgetc(F);
				}
				while (((buf - '0') >= 0) && ((buf - '0') <= 9)) {
					img.Nblig = img.Nblig * 10 + (buf - '0');
					buf = fgetc(F);
				}

				buf = fgetc(F);
				if (buf == '#') {
					/* on ignore tout jusqu'� trouver '\n' */
					while (buf != '\n')
						buf = fgetc(F);
					buf = fgetc(F);
				}
				while (((buf - '0') >= 0) && ((buf - '0') <= 9)) {
					dynamique = dynamique * 10 + (buf - '0');
					buf = fgetc(F);
				}

				/* d�but des data */

				printf("Lecture image NG type %s avec %d lignes et %d colonnes...\n", type, img.Nblig, img.Nbcol);

				/* taille connue, allocation dynamique possible */
				img = allocationImage(img.Nblig, img.Nbcol);

				/* lecture d'un bloc */
				fread(img.data, sizeof(unsigned char), img.Nbcol*img.Nblig, F);
			}
			else
				printf("Format non supporte pour l'instant...\n");
		fclose(F);
	}
	return img;
}

void sauvegardeImage(IMAGE img, const char *type, const char *out)
{
	FILE *F = NULL;

	if (img.data) /* image non vide */
	{
		printf("Sauvegarde image au type %s avec %d lignes et %d colonnes...\n", type, img.Nblig, img.Nbcol);
		/* selon le type ouverture binaire ou texte */
		if (strcmp(type, "P2") == 0)  /* cas ASCII niveaux de gris */
		{
			F = fopen(out, "w");

			fprintf(F, "%s\n", type);
			fprintf(F, "# Created by TSE FISA IPSI\n");
			fprintf(F, "%d %d %d\n", img.Nbcol, img.Nblig, 255);
			/* sauvegarde pixel par pixel */
			{
				int i, j;

				for (i = 0; i<img.Nblig; i++)
					for (j = 0; j<img.Nbcol; j++)
						fprintf(F, "%d ", img.pixel[i][j]);
			}
			fclose(F);
		}
		else
			if (strcmp(type, "P5") == 0)  /* cas brut niveaux de gris */
			{
				char tmp[255];

				F = fopen(out, "wb");

				fputs("P5\n", F);
				fputs("# Created by TSE FISA IPSI\n", F);
				sprintf(tmp, "%d %d %d\n", img.Nbcol, img.Nblig, 255);
				fputs(tmp, F);
				/* sauvegarde par bloc */
				fwrite(img.data, sizeof(unsigned char), img.Nbcol*img.Nblig, F);
				fclose(F);
			}
	}
	else
		printf("Image vide\n");
}

IMAGERGB allocationImageRGB(int Nblig, int Nbcol)
{
	IMAGERGB mat = { 0,0,NULL,NULL };
	int i;

	mat.Nblig = Nblig;
	mat.Nbcol = Nbcol;
	mat.data = (RGB*)malloc(Nblig*Nbcol*sizeof(RGB));
	if (mat.data == NULL)
		return(mat);
	mat.pixel = (RGB**)malloc(Nblig*sizeof(RGB*));
	if (mat.pixel == NULL) {
		free(mat.data);
		mat.data = NULL;
		return(mat);
	}
	for (i = 0; i<Nblig; i++)
		mat.pixel[i] = &(mat.data[i*Nbcol]);

	return(mat);
}

void liberationImageRGB(IMAGERGB *m)
{
	if (m->data != NULL) {
		free(m->data);
		m->data = NULL;
	}
	if (m->pixel != NULL) {
		free(m->pixel);
		m->pixel = NULL;
	}
}

IMAGERGB lectureImageRGB(const char *in)
{
	FILE *F = NULL;
	IMAGERGB img = { 0,0,NULL };
	int dynamique = 0;

	if ((F = fopen(in, "r")) == NULL)
	{
		printf("Pb image inexistante");
	}
	else
	{
		char type[3];

		fgets(type, 3, F);
		fclose(F);
		/* selon le type ouverture binaire ou texte */
		if (strcmp(type, "P3") == 0)  /* cas ASCII couleur */
		{
			char tmp[255] = "";
			char buf;

			F = fopen(in, "r");

			/* lecture caract�re apr�s caract�re compte-tenu de la diversit� des ent�tes possibles */
			fscanf(F, "%c", &type[0]);
			fscanf(F, "%c", &type[1]);
			fscanf(F, "%c", &buf); /* caract�re espacement */

			fscanf(F, "%c", &buf);
			if (buf == '#') {
				/* on ignore tout jusqu'� trouver '\n' */
				while (buf != '\n')
					fscanf(F, "%c", &buf);
				fscanf(F, "%c", &buf);
			}
			while (((buf - '0') >= 0) && ((buf - '0') <= 9)) { /* possibilit� d'utiliser �galement isdigit de la librairie standard <ctype.h> */
				img.Nbcol = img.Nbcol * 10 + (buf - '0');
				fscanf(F, "%c", &buf);
			}

			fscanf(F, "%c", &buf);
			if (buf == '#') {
				/* on ignore tout jusqu'� trouver '\n' */
				while (buf != '\n')
					fscanf(F, "%c", &buf);
				fscanf(F, "%c", &buf);
			}
			while (((buf - '0') >= 0) && ((buf - '0') <= 9)) {
				img.Nblig = img.Nblig * 10 + (buf - '0');
				fscanf(F, "%c", &buf);
			}

			fscanf(F, "%c", &buf);
			if (buf == '#') {
				/* on ignore tout jusqu'� trouver '\n' */
				while (buf != '\n')
					fscanf(F, "%c", &buf);
				fscanf(F, "%c", &buf);
			}
			while (((buf - '0') >= 0) && ((buf - '0') <= 9)) {
				dynamique = dynamique * 10 + (buf - '0');
				fscanf(F, "%c", &buf);
			}

			/* d�but des data */

			printf("Lecture image RGB type %s avec %d lignes et %d colonnes...\n", type, img.Nblig, img.Nbcol);

			/* taille connue, allocation dynamique possible */
			img = allocationImageRGB(img.Nblig, img.Nbcol);

			/* lecture pixel par pixel */
			{
				int i, j;
				int tmp;

				for (i = 0; i<img.Nblig; i++)
					for (j = 0; j<img.Nbcol; j++)
					{
						fscanf(F, "%d", &tmp);
						img.pixel[i][j].R = (unsigned char)tmp;
						fscanf(F, "%d", &tmp);
						img.pixel[i][j].G = (unsigned char)tmp;
						fscanf(F, "%d", &tmp);
						img.pixel[i][j].B = (unsigned char)tmp;
					}
			}
		}
		else
			if (strcmp(type, "P6") == 0)  /* cas brut couleur */
			{
				char buf;

				F = fopen(in, "rb");

				/* lecture caract�re apr�s caract�re compte-tenu de la diversit� des ent�tes possibles */
				type[0] = fgetc(F);
				type[1] = fgetc(F);
				buf = fgetc(F); /* caract�re espacement */

				buf = fgetc(F);
				if (buf == '#') {
					/* on ignore tout jusqu'� trouver '\n' */
					while (buf != '\n')
						buf = fgetc(F);
					buf = fgetc(F);
				}
				while (((buf - '0') >= 0) && ((buf - '0') <= 9)) { /* possibilit� d'utiliser �galement isdigit de la librairie standard <ctype.h> */
					img.Nbcol = img.Nbcol * 10 + (buf - '0');
					buf = fgetc(F);
				}

				buf = fgetc(F);
				if (buf == '#') {
					/* on ignore tout jusqu'� trouver '\n' */
					while (buf != '\n')
						buf = fgetc(F);
					buf = fgetc(F);
				}
				while (((buf - '0') >= 0) && ((buf - '0') <= 9)) {
					img.Nblig = img.Nblig * 10 + (buf - '0');
					buf = fgetc(F);
				}

				buf = fgetc(F);
				if (buf == '#') {
					/* on ignore tout jusqu'� trouver '\n' */
					while (buf != '\n')
						buf = fgetc(F);
					buf = fgetc(F);
				}
				while (((buf - '0') >= 0) && ((buf - '0') <= 9)) {
					dynamique = dynamique * 10 + (buf - '0');
					buf = fgetc(F);
				}

				/* d�but des data */

				printf("Lecture image RGB type %s avec %d lignes et %d colonnes...\n", type, img.Nblig, img.Nbcol);

				/* taille connue, allocation dynamique possible */
				img = allocationImageRGB(img.Nblig, img.Nbcol);

				/* lecture d'un bloc */
				fread(img.data, sizeof(RGB), img.Nbcol*img.Nblig, F);
			}
			else
				printf("Format non supporte pour l'instant...\n");
		fclose(F);
	}
	return img;
}

void sauvegardeImageRGB(IMAGERGB img, const char *type, const char *out)
{
	FILE *F = NULL;

	if (img.data) /* image non vide */
	{
		printf("Sauvegarde image au type %s avec %d lignes et %d colonnes...\n", type, img.Nblig, img.Nbcol);
		/* selon le type ouverture binaire ou texte */
		if (strcmp(type, "P3") == 0)  /* cas ASCII couleur */
		{
			F = fopen(out, "w");

			fprintf(F, "%s\n", type);
			fprintf(F, "# Created by TSE FISA IPSI\n");
			fprintf(F, "%d %d %d\n", img.Nbcol, img.Nblig, 255);
			/* sauvegarde pixel par pixel */
			{
				int i, j;

				for (i = 0; i<img.Nblig; i++)
					for (j = 0; j<img.Nbcol; j++) {
						fprintf(F, "%d ", img.pixel[i][j].R);
						fprintf(F, "%d ", img.pixel[i][j].G);
						fprintf(F, "%d ", img.pixel[i][j].B);
					}
				fclose(F);
			}
		}
		else
			if (strcmp(type, "P6") == 0)  /* cas brut couleur */
			{
				char tmp[255];

				F = fopen(out, "wb");

				fputs("P6\n", F);
				fputs("# Created by TSE FISA IPSI\n", F);
				sprintf(tmp, "%d %d %d\n", img.Nbcol, img.Nblig, 255);
				fputs(tmp, F);

				/* sauvegarde par bloc */
				fwrite(img.data, sizeof(RGB), img.Nbcol*img.Nblig, F);
				fclose(F);
			}
	}
	else
		printf("Image vide\n");
}

IMAGE inverseImage(IMAGE img)
{
	IMAGE out = { 0,0,NULL,NULL };
	int* lut = NULL;
	int i;

	lut = (int*)calloc(256, sizeof(int)); /* LUT pour optimiser */
	for (i = 0; i<256; i++)
		lut[i] = 255 - i;

	out = allocationImage(img.Nblig, img.Nbcol);
	for (i = 0; i<img.Nbcol*img.Nblig; i++)
		out.data[i] = lut[img.data[i]];

	free(lut);

	return out;
}

IMAGE seuillageImage(IMAGE img, int sInf, int sSup)
{
	IMAGE out = { 0,0,NULL,NULL };
	int* lut = NULL;
	int i;

	lut = (int*)calloc(256, sizeof(int));
	for (i = sInf; i <= sSup; i++)
		lut[i] = 255;

	out = allocationImage(img.Nblig, img.Nbcol);
	for (i = 0; i<img.Nbcol*img.Nblig; i++)
		out.data[i] = lut[img.data[i]];

	free(lut);

	return out;
}


IMAGE planImage(IMAGERGB img, int plan)
{
	IMAGE out = { 0,0,NULL,NULL };
	int i;

	out = allocationImage(img.Nblig, img.Nbcol);
	switch (plan)
	{
	case 0: /* plan rouge */
		for (i = 0; i<out.Nbcol*out.Nblig; i++)
			out.data[i] = img.data[i].R;
		break;
	case 1: /* plan vert */
		for (i = 0; i<out.Nbcol*out.Nblig; i++)
			out.data[i] = img.data[i].G;
		break;
	case 2: /* plan bleu */
		for (i = 0; i<out.Nbcol*out.Nblig; i++)
			out.data[i] = img.data[i].B;
		break;
	}
	return(out);
}

IMAGE luminanceImage(IMAGERGB img, float r, float g, float b)
{
	IMAGE out = { 0,0,NULL,NULL };
	int i;

	out = allocationImage(img.Nblig, img.Nbcol);

	for (i = 0; i<out.Nbcol*out.Nblig; i++)
		out.data[i] = (unsigned char)((float)img.data[i].R*r + (float)img.data[i].G*g + (float)img.data[i].B*b);
	return(out);
}

int* histogrammeImage(IMAGE img, int choix, int n)
{
	int* h = NULL;

	double ratio = 256/n;

	if (img.data)
	{
		int i;

		h = (int*)calloc(n, sizeof(int));

		for (i = 0; i<img.Nbcol*img.Nblig; i++)
			h[(int)(img.data[i]/ratio)] += 1;

		if (choix)
		{
			/* sauvegarde dans histo.csv */
			FILE *F;

			F = fopen("histo.csv", "w");
			int i;
			if (F != NULL) {
				for (i = 0; i < n; i++)
					fprintf(F, "%d ; %d\n", i, h[i]);
				fclose(F);
			}
			else
				printf("Acces a 'histo.csv' refuse.\n");
			
		}
	}

	return h;
}

int* histogrammeImageRGB(IMAGERGB img, int choix, int n)
{
	int* h = NULL; /* RGB pas possible car UC, donc 3xint avec R G B successivement pour chaque niveau */

	if (img.data)
	{
		int i;
		int ln = (int)(log(n) / log(2));
		int bins = 256 >> (8 - ln);

		h = (int*)calloc(3 * bins, sizeof(int));

		for (i = 0; i < img.Nbcol * img.Nblig; i++)
		{
			h[3 * (((int)img.data[i].R) >> (8 - ln))] += 1; /* R */
			h[3 * (((int)img.data[i].G) >> (8 - ln)) + 1] += 1; /* G */
			h[3 * (((int)img.data[i].B) >> (8 - ln)) + 2] += 1; /* B */
		}

		if (choix)
		{
			/* sauvegarde dans histo.csv */
			FILE* F;

			F = fopen("histo.csv", "w");
			int i;
			if (F != NULL) {
				for (i = 0; i < n; i++)
					fprintf(F, "%d ; %d; %d; %d\n", i, h[3 * i], h[3 * i + 1], h[3 * i + 2]);
				fclose(F);
			}
			else
				printf("Acces a 'histo.csv' refuse.\n");
		}
	}

	return h;
}


IMAGE expansionImage(IMAGE img, int outMin, int outMax)
{
	//return img;
	IMAGE out = { 0,0,NULL,NULL };
	int* lut = NULL;
	int i;
	float a, b;
	int mini = 255, maxi = 0;

	for (i = 0; i<img.Nblig*img.Nbcol; i++)
	{
		mini = (img.data[i] < mini) ? img.data[i] : mini;
		maxi = (img.data[i] > maxi) ? img.data[i] : maxi;
	}

	a = (float)(outMax - outMin) / ((float)(maxi - mini));
	b = (float)outMin - a * mini;

	lut = (int*)calloc(256, sizeof(int));
	for (i = mini; i <= maxi; i++)
		lut[i] = (int)(a*i + b);

	out = allocationImage(img.Nblig, img.Nbcol);
	for (i = 0; i<img.Nbcol*img.Nblig; i++)
		out.data[i] = lut[img.data[i]];

	free(lut);

	return out;
}

IMAGE egalisationImage(IMAGE img)
{
	IMAGE out = { 0,0,NULL,NULL };
	int* h = histogrammeImage(img, 0, 256);
	int* hC = NULL; 
	int* lut = NULL;
	int i;

	hC = (int*)calloc(256, sizeof(int));
	hC[0] = h[0];
	for (i = 1; i<256; i++)
		hC[i] = hC[i - 1] + h[i];
	free(h);
	h = NULL;

	lut = (int*)calloc(256, sizeof(int));
	for (i = 0; i<256; i++)
		lut[i] = (int)((255 / (float)(img.Nbcol*img.Nblig)) * hC[i]);
	free(hC);
	hC = NULL;

	out = allocationImage(img.Nblig, img.Nbcol);
	for (i = 0; i<img.Nbcol*img.Nblig; i++)
		out.data[i] = lut[img.data[i]];
	free(lut);

	return(out);
}

SIGNATURES statistiquesImage(IMAGE img)
{
	SIGNATURES sig;
	int i;
	double som = 0, som2 = 0;
	int somme, moitPop = (img.Nbcol*img.Nblig) / 2;
	int* h = histogrammeImage(img, 0, 256);

	for (i = 0; i<img.Nbcol*img.Nblig; i++) {
		som += (double)img.data[i];
		som2 += (double)img.data[i] * (double)img.data[i];
	}

	som /= (double)(img.Nblig*img.Nbcol);
	som2 = sqrt(som2 / (double)(img.Nblig*img.Nbcol) - som*som);

	sig.moyenne = som;
	sig.ecartType = som2;

	i = 0;
	while (h[i] == 0)
		i += 1;
	sig.min = i;

	i = 255;
	while (h[i] == 0)
		i -= 1;
	sig.max = i;

	i = 0;
	somme = h[i];
	while (somme < moitPop) {
		i += 1;
		if (i < 256)
			somme += h[i];
	}
	sig.mediane = i;

	free(h);
	h = NULL;

	return sig;
}

IMAGE seuillageOtsu(IMAGE img)
{
	IMAGE out = { 0, 0, NULL, NULL };
	int* h = histogrammeImage(img, 0, 256);
	int* hC = NULL;

	double *tab = NULL;
	int i, seuil;
	double M1, M2;
	double w1;
	int* lut = NULL;
	int min, max;

	/* recherche min et max */
	i = 0;
	while (h[i] == 0)
		i++;
	min = i;
	i = 255;
	while (h[i] == 0)
		i--;
	max = i;

	hC = (int*)calloc(256, sizeof(int));
	hC[0] = h[0];
	for (i = 1; i < 256; i++)
		hC[i] = hC[i - 1] + h[i];

	tab = (double*)calloc(256, sizeof(double));

	/* parcours entre min et max pour �viter divisions par 0 */
	/* initialisation */
	M1 = min;
	seuil = min;
	w1 = (double)hC[min] / (double)(img.Nbcol*img.Nblig);
	M2 = 0;
	for (int i = min + 1; i <= max; i++)
		M2 += (double)h[i] * i;
	M2 /= (double)(hC[max] - h[min]);
	tab[min] = w1*(1 - w1)*(M1 - M2)*(M1 - M2);

	// parcours
	for (int i = min + 1; i < max; i++) {
		M1 = ((double)hC[i - 1] * M1 + (double)h[i] * i) / hC[i];
		M2 = ((double)(hC[255] - hC[i - 1])*M2 - (double)(h[i] * i)) / (double)(hC[255] - hC[i]);
		w1 = (double)hC[i] / (double)(img.Nbcol*img.Nblig);
		tab[i] = w1*(1 - w1)*(M1 - M2)*(M1 - M2);
		if (tab[i] > tab[seuil])
			seuil = i;
	}

	free(h); h = NULL;
	free(hC); hC = NULL;

	free(tab);

	lut = (int*)calloc(256, sizeof(int));
	for (i = seuil; i <= max; i++)
		lut[i] = 255;

	out = allocationImage(img.Nblig, img.Nbcol);
	for (i = 0; i<img.Nbcol*img.Nblig; i++)
		out.data[i] = lut[img.data[i]];

	free(lut);
	return out;
}

/* �tiquetage V8 */
IMAGE labelImage(IMAGE img, int *nbComp)
{
	IMAGE out = { 0,0,NULL,NULL };

	int i, ii, j, jj;

	int k = 1, kk;
	int eps;
	int predNonNul[5];
	int nbPredNonNuls;

	int nbMaxEtiquettes = img.Nblig * img.Nbcol;
	intIMAGE withBords = { 0,0,NULL,NULL };

	withBords = allocationIntImage(img.Nblig + 2, img.Nbcol + 2);

	for (i = 0, ii = 1; i < img.Nblig; i++, ii++) // pure copie 
		for (j = 0, jj = 1; j < img.Nbcol; j++, jj++)
			withBords.pixel[ii][jj] = (int)img.pixel[i][j];

	int *tableEtiquette = NULL;
	tableEtiquette = (int*)calloc(nbMaxEtiquettes, sizeof(int));

	for (kk = 0; kk < nbMaxEtiquettes; kk++) {
		tableEtiquette[kk] = kk;
	}

	for (i = 1; i < withBords.Nblig - 1; i++)
		for (j = 1; j < withBords.Nbcol - 1; j++) {
			if (withBords.pixel[i][j] != 0) {
				// voisinage V8 
				int pred1 = withBords.pixel[i][j - 1];
				int pred2 = withBords.pixel[i - 1][j - 1];
				int pred3 = withBords.pixel[i - 1][j];
				int pred4 = withBords.pixel[i - 1][j + 1];

				nbPredNonNuls = 0;
				if (pred1) predNonNul[nbPredNonNuls++] = pred1;
				if (pred2) predNonNul[nbPredNonNuls++] = pred2;
				if (pred3) predNonNul[nbPredNonNuls++] = pred3;
				if (pred4) predNonNul[nbPredNonNuls++] = pred4;

				if (nbPredNonNuls == 0) {
					withBords.pixel[i][j] = k;
					k++;
				}
				else {
					eps = 0;
					int minPred = tableEtiquette[predNonNul[0]];
					int maxPred = tableEtiquette[predNonNul[0]];
					while (eps < nbPredNonNuls) {
						if (tableEtiquette[predNonNul[eps]] < minPred) minPred = tableEtiquette[predNonNul[eps]];
						if (tableEtiquette[predNonNul[eps]] > maxPred) maxPred = tableEtiquette[predNonNul[eps]];
						eps++;
					}
					if (minPred == maxPred)
						withBords.pixel[i][j] = minPred;
					else {
						withBords.pixel[i][j] = minPred;
						for (eps = 0; eps < nbPredNonNuls; eps++) {
							int beta = predNonNul[eps];
							while (tableEtiquette[beta] != minPred) {
								int m = tableEtiquette[beta];
								tableEtiquette[beta] = minPred;
								beta = m;
							}
						}
					}
				}
			}
		}


	// actualisation de la table d'�quivalence
	for (int kk = 1; kk < k; kk++) {
		int m = kk;
		while (tableEtiquette[m] != m)
			m = tableEtiquette[m];
		tableEtiquette[kk] = m;
	}

	int* etiquettes = (int*)calloc(k + 1, sizeof(int));

	// histo pour rep�rer les trous
	for (int kk = 1; kk<k; kk++)
		etiquettes[tableEtiquette[kk]]++;

	// on remet � jour les index (etiquetage d�finitif hors trou)
	etiquettes[0] = 0;
	int compt = 1;
	for (int kk = 1; kk<k; kk++) {
		if (etiquettes[kk])
			etiquettes[kk] = compt++;
	}

	for (i = 0; i<withBords.Nblig; i++)
		for (j = 0; j<withBords.Nbcol; j++) {
			withBords.pixel[i][j] = etiquettes[tableEtiquette[withBords.pixel[i][j]]];
		}

	free(etiquettes);
	free(tableEtiquette);

	out = allocationImage(img.Nblig, img.Nbcol);

	*nbComp = compt - 1;
	for (i = 0, ii = 1; i < img.Nblig; i++, ii++)
		for (j = 0, jj = 1; j < img.Nbcol; j++, jj++)
			out.pixel[i][j] = (unsigned char)withBords.pixel[ii][jj];

	liberationIntImage(&withBords);

	return out;
}

int* lectureFicherFausseCouleurs(char * lutFile){
	char ligne[255];
	int temp=0, *table = calloc(256*3, sizeof(int)), i = 0;
	FILE* lut = fopen(lutFile, "r");
	while (fgets(ligne, 255, lut) != NULL)
	{
		//printf("%s", ligne); /* remplacer ici par sscanf */
		sscanf(ligne, "%d\t%d\t%d\t%d\n", &temp, &table[3 * i], &table[3 * i + 1], &table[3 * i + 2]);
		i++;
	}
	fclose(lut);
	return table;
}

IMAGERGB conversionImageFausseCouleur(IMAGE img, char* lutFile){
	int* lut = lectureFicherFausseCouleurs(lutFile), ng = 0;

	IMAGERGB ifc= allocationImageRGB(img.Nblig, img.Nbcol);

	for (int i = 0; i < img.Nbcol * img.Nblig; i++) {
		ng = 3 * img.data[i];
		ifc.data[i].R = lut[ng];
		ifc.data[i].G = lut[ng+1];
		ifc.data[i].B = lut[ng+2];
	}
	return ifc;
}

double distanceEcartTypeImage(IMAGE im1, IMAGE im2)
{
	return abs(statistiquesImage(im1).ecartType - statistiquesImage(im2).ecartType);
}

double distanceHistogrammeImage(IMAGE im1, IMAGE im2){
	int* h1 = histogrammeImage(im1, 0, 256);
	int* h2 = histogrammeImage(im2, 0, 256);
	double d = 0;
	for (int i = 0; i < 256; i++) {
		d += (h1[i] - h2[i]) * (h1[i] - h2[i]);
	}
	return sqrt(d / 256);
}

double distanceHistogrammeImageRGB(IMAGERGB im1, IMAGERGB im2)
{
	int* h1 = histogrammeImageRGB(im1, 0, 256);
	int* h2 = histogrammeImageRGB(im2, 0, 256);
	double d = 0;
	for (int i = 0; i < 3*256; i++) {
		d += (h1[i] - h2[i]) * (h1[i] - h2[i]);
	}
	return sqrt(d / 3*256);
}

void LectureListing(int * nbImages, char ** titres){
	FILE* F = NULL;
	//int nbImages = 0;
	
	//char** titres;
	
	if ((F = fopen("data/listing.txt", "r")) == NULL)
	{
		printf("Pb fichier inexistant");
	}
	else {
		//fgets(nbImages, 1, F);
		fscanf(F, "%d", nbImages);
		//titres = calloc(nbImages, sizeof(char*));
		for (int i = 0; i < *nbImages; i++) {
			titres[i] = calloc(15, sizeof(char));
			fscanf(F, "%s", titres[i]);
		}
		fclose(F);
	}
}

int choixUser(int min, int max)
{
	int choix = min;
	do {
		printf("Choix image [%d : %d] : ", min, max);
		scanf("%i", &choix);
	} while ((choix < min) || (choix > max));

	return choix;
}

IMAGE bruitAleatoireImage(IMAGE img, int amplitude)
{
	IMAGE imBruit = allocationImage(img.Nblig, img.Nbcol);

	for (int pix = 0; pix < img.Nbcol * img.Nblig; pix++) {
		imBruit.data[pix] = img.data[pix] + randInt(-amplitude, amplitude);
	}

	return imBruit;
}

IMAGERGB bruitAleatoireImageRGB(IMAGERGB img, int amplitude)
{
	IMAGERGB imBruit = allocationImageRGB(img.Nblig, img.Nbcol);
	double rand;
	for (int pix = 0; pix < img.Nbcol * img.Nblig; pix++) {
		rand = 1 + randInt(-amplitude, amplitude)/255.0;
		imBruit.data[pix].R = MIN(255, img.data[pix].R * rand);
		imBruit.data[pix].G = MIN(255, img.data[pix].G * rand);
		imBruit.data[pix].B = MIN(255, img.data[pix].B * rand);
	}

	return imBruit;
}

IMAGERGB masqueIMAGE(IMAGE img, IMAGERGB masque)
{
	IMAGERGB RGB = allocationImageRGB(img.Nblig, img.Nbcol);
	int taille = img.Nbcol * img.Nblig;
	for (int pix = 0; pix < taille; pix++) {
		if ((masque.data[pix].R == 0) && (masque.data[pix].G == 0) && (masque.data[pix].B == 0)) {
			RGB.data[pix].R = RGB.data[pix].G = RGB.data[pix].B = img.data[pix];
		}
		else {
			RGB.data[pix] = masque.data[pix];
		}
	}

	return RGB;
}

IMAGE convolution(IMAGE img, STREL strel)
{
	IMAGE result = allocationImage(img.Nblig, img.Nbcol);
	float sum;
	int i, j, itemp, jtemp, x, y;
	for ( i = 0; i < img.Nblig; i++) {
		for ( j = 0; j < img.Nbcol; j++) {
			sum = 0;
			for ( y = 0; y < strel.Nblig; y++) {
				for ( x = 0; x < strel.Nbcol; x++) {
					itemp = y + i - strel.xradius + 1;
					jtemp = x + j - strel.yradius + 1;
					if ((itemp >= 0) && (itemp < img.Nblig) && (jtemp >= 0) && (jtemp < img.Nbcol))
						sum += (img.pixel[itemp][jtemp] * strel.pixel[y][x]);
				}
			}
			result.pixel[i][j] = (char) sum;
		}
	}
	return result;
}

IMAGE erosion(IMAGE img,  STREL strel)
{
	IMAGE erod = allocationImage(img.Nblig, img.Nbcol);
	VOISINAGE V;
	
	for (int i = 0; i < img.Nblig; i++)
		for (int j = 0; j < img.Nbcol; j++)
			if (img.pixel[i][j] != 0)
				erod.pixel[i][j] = getVal(voisinage(img, i, j, strel), "min");

	/*
	if (voisinage == 4) {
		for (int i = 0; i < img.Nblig; i++) {
			for (int j = 0; j < img.Nbcol; j++) {
				if (i == 0 || i == img.Nblig - 1 || j == 0 || j == img.Nbcol - 1)
					erod.pixel[i][j] = 0;
				else
					erod.pixel[i][j] = MIN(MIN(MIN(img.pixel[i][j], img.pixel[i - 1][j]), MIN(img.pixel[i][j - 1], img.pixel[i + 1][j])), img.pixel[i][j + 1]);
			}
		}
	}
	else if (voisinage == 8) {
		for (int i = 1; i < img.Nblig - 1; i++) {
			for (int j = 1; j < img.Nbcol - 1; j++) {
				if (i == 0 || i == img.Nblig - 1 || j == 0 || j == img.Nbcol - 1)
					erod.pixel[i][j] = 0;
				else
					erod.pixel[i][j] = MIN(MIN(MIN(img.pixel[i - 1][j], img.pixel[i - 1][j - 1]), MIN(img.pixel[i][j - 1], img.pixel[i + 1][j - 1])), MIN(MIN(img.pixel[i + 1][j], img.pixel[i + 1][j + 1]), MIN(img.pixel[i][j + 1], img.pixel[i - 1][j + 1])));
				
			}
		}
	}
	else printf("Voisinage invalide (4 ou 8)\n");*/

	return erod;
}

IMAGE dilatation(IMAGE img, STREL strel)
{
	IMAGE dilat = allocationImage(img.Nblig, img.Nbcol);
	VOISINAGE V;

	for (int i = 0; i < img.Nblig; i++)
		for (int j = 0; j < img.Nbcol; j++)
			if (img.pixel[i][j] != 0) 
				dilat.pixel[i][j] = getVal(voisinage(img, i, j, strel), "max");

	
	/*if (voisinage == 4) {
		for (int i = 0; i < img.Nblig; i++) {
			for (int j = 0; j < img.Nbcol; j++) {
				if (i == 0 || i == img.Nblig - 1 || j == 0 || j == img.Nbcol - 1)
					dilat.pixel[i][j] = 0;
				else
					dilat.pixel[i][j] = MAX(MAX(MAX(img.pixel[i][j], img.pixel[i - 1][j]), MAX(img.pixel[i][j - 1], img.pixel[i + 1][j])), img.pixel[i][j + 1]);
			}
		}
	}
	else if (voisinage == 8) {
		for (int i = 1; i < img.Nblig - 1; i++) {
			for (int j = 1; j < img.Nbcol - 1; j++) {
				if (i == 0 || i == img.Nblig - 1 || j == 0 || j == img.Nbcol - 1)
					dilat.pixel[i][j] = 0;
				else
					dilat.pixel[i][j] = MAX(MAX(MAX(img.pixel[i - 1][j], img.pixel[i - 1][j - 1]), MAX(img.pixel[i][j - 1], img.pixel[i + 1][j - 1])), MAX(MAX(img.pixel[i + 1][j], img.pixel[i + 1][j + 1]), MAX(img.pixel[i][j + 1], img.pixel[i - 1][j + 1])));

			}
		}
	}
	else printf("Voisinage invalide (4 ou 8)\n");*/

	return dilat;
}

IMAGE difference(IMAGE img1, IMAGE img2) {
	IMAGE res = allocationImage(img1.Nblig, img1.Nbcol);
	int diff = 0;
	for (int pix = 0; pix < res.Nbcol * res.Nblig; pix++) {
		diff = 0;
		if(img1.data[pix] > img2.data[pix]) diff = img1.data[pix] - img2.data[pix];
		
		/*if (diff) {
			printf("ha");
		}*/
		res.data[pix] = diff;
	}
	return res;
}

IMAGE copy(IMAGE img) {
	IMAGE img2 = allocationImage(img.Nblig, img.Nbcol);
	for (int i = 0; i < img.Nblig; i++) {
		for (int j = 0; j < img.Nbcol; j++) {
			img2.pixel[i][j] = img.pixel[i][j];
		}
	}

	return img2;
}

float perimetre(IMAGE img, int sig)
{
	IMAGE ref = copy(img);
	
	int cnt1 = 0, cnt2 = 0;
	//int m = ref.data[0];
	int i, j;
	int fnd = 0;

	//while(fnd != 0){
		//fnd = 0;
		for (i = 1; (i < ref.Nblig - 1) && (fnd ==0); i++) {
			for (j = 1; (j < ref.Nbcol - 1) && (fnd ==0); j++) {
				if (ref.pixel[i][j] == sig) {
					//m = ref.pixel[i][j];
					fnd = 1;
					break;
				}
			}
		}

		if (fnd == 0) return 0;
		i--;
		int found = 1;
		int c_neighbor = 0, min_i, min_j;
		while (found) {
			found = 0;
			for (int k = -1; k <= 1; k++) {
				for (int n = 1; n >= -1; n--) {
					if (i + k < 0) return 0;
					else if (i + k >= ref.Nblig) return 0;
					else if (j + n < 0) return 0;
					else if (j + n >= ref.Nbcol) return 0;

					else if ((ref.pixel[i + k][j + n] - sig) == 0) {
						ref.pixel[i][j] = 0;
						if ((n != 0 ) != (k != 0)) cnt1++;
						else if((n != 0) && (k != 0))  cnt2++;
						i += k;
						j += n;
						found = 1;
					}
				}
			}
		}
		return cnt1 + 1.4142 * cnt2;
	//}

}

void rechercheImageProche(int indexIm, char **titles, int nbi, int *indexMinET, int *indexMinHS, int *minET, int *minHS){
	IMAGE ref = lectureImage(titles[indexIm]);

	IMAGE comp = lectureImage(titles[0]);

	int deltaET = distanceEcartTypeImage(ref, comp), deltaHS = distanceHistogrammeImage(ref, comp); //minET, minHS, indexMinET, indexMinHS;
	*minET = deltaET;
	*minHS = deltaHS;
	for (int i = 0; i < nbi; i++) {
		if (i != indexIm) {
			comp = lectureImage(titles[i]);
			deltaET = distanceEcartTypeImage(ref, comp);
			deltaHS = distanceHistogrammeImage(ref, comp);

			if (deltaET < *minET) {
				*indexMinET = i;
				*minET = deltaET;
			}
			if (deltaHS < *minHS) {
				*indexMinHS = i;
				*minHS = deltaHS;
			}
		}
	}

}

IMAGE applicateurLUTVal(IMAGE img, int* LUT) {
	IMAGE res = allocationImage(img.Nblig, img.Nbcol);
	sauvegardeImage(img, "P5", "test/preLUT.pgm");
	for (int k = 0; k < img.Nbcol * img.Nblig; k++) {
		res.data[k] = LUT[img.data[k]];
	}
	return res;
}

void applicateurLUTRef(IMAGE *img, int* LUT) {
	for (int k = 0; k < img->Nbcol * img->Nblig; k++)
		img->data[k] = LUT[img->data[k]];
}

SIGNATURE_COMPOSANTE_CONNEXE* signaturesImage(IMAGE img, int nbComp) {
	SIGNATURE_COMPOSANTE_CONNEXE* sign = (SIGNATURE_COMPOSANTE_CONNEXE*)malloc((nbComp+1) * sizeof(SIGNATURE_COMPOSANTE_CONNEXE));
	int* LUTBords = (int*)malloc((nbComp + 1)* sizeof(int));
	
	for (int i = 0; i < nbComp+1; i++) {
		LUTBords[i] = i;
		sign[i].bord = 0;
		sign[i].perimetre = 0;
		sign[i].surface = 0;
		sign[i].CG.x = 0;
		sign[i].CG.y = 0;
	}
	int max = 0;
	
	IMAGE er = erosion(img, V4());
	IMAGE cnt = difference(img, er);
	//sauvegardeImageRGB(conversionImageFausseCouleur(img, "data/ipsi.txt"), "P6", "test/img.ppm");
	//sauvegardeImageRGB(conversionImageFausseCouleur(er, "data/ipsi.txt"), "P6", "test/er.ppm");
	//sauvegardeImageRGB(conversionImageFausseCouleur(cnt, "data/ipsi.txt"), "P6", "test/cnt.ppm");

	int i = 0;
	int j = 0;
	//sauvegardeImageRGB(conversionImageFausseCouleur(cnt, "data/ipsi.txt"), "P6", "test/labelO.ppm");

	//float* peri = (float*)malloc(nbComp, sizeof(float));


	for (int k = 0; k < img.Nbcol * img.Nblig; k++) {
		sign[img.data[k]].surface++;
		//sign[cnt.data[k]].perimetre += 1;
		int i = k % img.Nbcol;
		int j = k / img.Nblig;
		sign[img.data[k]].CG.x += i;
		sign[img.data[k]].CG.y += j;
	}
	for (int i = 1; i < nbComp + 1; i++) {
		sign[i].perimetre = perimetre(cnt, i);
		sign[i].compacite = pow(sign[i].perimetre, 2) / (4 * PI * sign[i].surface);
		sign[i].rayon = sqrt(sign[i].surface) / PI;
		sign[i].CG.x /= sign[i].surface;
		sign[i].CG.y /= sign[i].surface;
	}

	for (int k = 0; k < img.Nbcol; k++) {
		LUTBords[img.pixel[k][0]] = 0;
		LUTBords[img.pixel[k][img.Nbcol-1]] = 0;
		sign[img.pixel[k][0]].bord = 1;
		sign[img.pixel[k][img.Nbcol - 1]].bord = 1;
	}
	for(int k = 0; k < img.Nblig; k++) {
		LUTBords[img.pixel[0][k]] = 0;
		LUTBords[img.pixel[img.Nblig-1][k]] = 0;
		sign[img.pixel[0][k]].bord = 1;
		sign[img.pixel[img.Nblig - 1][k]].bord = 1;
	}

	applicateurLUTRef(&img, LUTBords);
	//sauvegardeImageRGB(conversionImageFausseCouleur(img, "data/ipsi.txt"), "P6", "test/img.ppm");

	return sign;
}

void sauvegardeSignaturesImage(SIGNATURE_COMPOSANTE_CONNEXE* sig, int nbComp, const char* fichier)
{
	FILE* F = NULL;

	F = fopen(fichier, "w");

	for (int i = 0; i < nbComp; i++)
		fprintf(F, "%d ; %d ; %d ; %d ; %d ; %d ;\n", sig[i].perimetre, sig[i].surface, sig[i].rayon, sig[i].compacite, sig[i].CG.x, sig[i].CG.y);


	fclose(F);
}

int** rechercheTrou(SIGNATURE_COMPOSANTE_CONNEXE* signObjet, SIGNATURE_COMPOSANTE_CONNEXE* signTrou, int nbObjets, int NbTrou, int *NbGroupes)
{
	int** ObjTrou = (int**)calloc(nbObjets + 1, sizeof(int*));
	int* TabTrou = (int*)calloc(nbObjets + 1, sizeof(int));
	float dist;
	for (int k = 0; k < nbObjets; k++) {
		if (!signObjet[k].bord) {
			int* temp = (int*)calloc(NbTrou+1, sizeof(int));
			for (int m = 0; m < NbTrou; m++) {
				if (signTrou[m].rayon > signObjet[k].rayon)
					continue;
				dist = pow(signObjet[k].CG.x - signTrou[m].CG.x, 2);
				dist += pow(signObjet[k].CG.y - signTrou[m].CG.y, 2);
				dist = sqrt(dist);
				dist -= signObjet[k].rayon;
				if (dist < 0) {
					temp[++TabTrou[k]] = m;
				}
			}
			temp[0] = TabTrou[k];
			ObjTrou[k] = realloc(temp, (TabTrou[k]+1) * sizeof(int));
		}
		else {
			TabTrou[k] = -1;
			ObjTrou[k] = NULL;
		}
	}
	int** temp = (int**)calloc(nbObjets + 1, sizeof(int*));
	int* indexs = (int*)calloc(nbObjets + 1, sizeof(int));
	int max = 0;
	for (int i = 0; i < nbObjets; i++) {
		if (ObjTrou[i] != NULL) {
			if (ObjTrou[i][0] > 0) {
				int nbTrou = ObjTrou[i][0];
				max = MAX(max, nbTrou);
				if(temp[nbTrou] == NULL)
					temp[nbTrou] = (int*)calloc(nbObjets + 1, sizeof(int));
				temp[nbTrou][indexs[nbTrou]++] = i;
			}
		}
	}
	free(ObjTrou);
	ObjTrou = (int**)calloc(max+2, sizeof(int*));
	for (int i = 0; i <= max; i++) {
		if (temp[i] != NULL) {
			ObjTrou[i] = realloc(temp[i], (indexs[i] + 1) * sizeof(int*));
			ObjTrou[i][indexs[i]] = -1;
		}
	}
	*NbGroupes = max;
	return ObjTrou;
}

void discriminationTrous(IMAGE img, IMAGE* res, int** configTrous, int NbObjets, int *NbGroup) {
	int **LUT = (int**)calloc(*NbGroup +1, sizeof(int*));
	int* luts = (int*)calloc(*NbGroup +1, sizeof(int));
	int cmp = 0;

	for (int i = 0; i <= *NbGroup; i++) {
		if(configTrous[i]){
			luts[cmp] = i;
			int c = 0;
			while (configTrous[i][c++] > 0);
			LUT[cmp] = (int*)calloc(NbObjets+1, sizeof(int));
			c = 0;
			while (configTrous[i][c] > 0) {
				LUT[cmp][configTrous[i][c]] = configTrous[i][c++];
			}
			cmp++;
		}
	}
	luts[cmp] = -1;
	int i = 0;
	*NbGroup = cmp;
	//res = (IMAGE*)calloc(cmp, sizeof(IMAGE));
	while(luts[i] > -1){
		res[i] = allocationImage(img.Nblig, img.Nbcol);
		res[i] = applicateurLUTVal(img, LUT[i]);
		i++;
	}

	//printf("hola");




	/*for (int i = 0; i < NbObjets; i++) {
		int* temp = (int*)calloc(NbObjets, sizeof(int));
		LUT[i] = temp;
		if (configTrous[i] > 0) {
			LUT[i][0] = 1; cmp++;
			LUT[i][configTrous[i][0]] = configTrous[i];
		}
	}
	int** temp = (int**)calloc(cmp, sizeof(int*));
	int t = 0;	
	for (int i = 0; i < NbObjets; i++) {
		if (LUT[i][0]) {
			temp[t] = LUT[i];
			temp[t][0] = i;
			t++;
		}
	}
	free(LUT); LUT = temp;
	
	
	for (int i = 0; i < NbObjets; i++) {
		IMAGE* img = &(res[LUT[i][0]]);
		applicateurLUTRef((res+LUT[i][0]), LUT[i], 1);
	}*/
	
}

int randInt(int min, int max)
{
	return min + (max - min) * ((float)rand() / RAND_MAX);
}

POINT* imageVersPoints(IMAGE img, int* n, char axe)
{
	POINT* pts = (POINT*)calloc(img.Nbcol * img.Nblig, sizeof(POINT));
	*n = 0;
	for (int i = 0; i < img.Nblig; i++) {
		for (int j = 0; j < img.Nbcol; j++) {
			if (img.pixel[i][j]) {
				if (axe == 'i') {
					pts[(*n)].x = i;
					pts[(*n)].y = j;
				}
				else if (axe == 'j') {
					pts[(*n)].x = j;
					pts[(*n)].y = i;
				}
				(*n)++;
			}
		}
	}
	pts = (POINT*)realloc(pts, (*n) * sizeof(POINT));
}

void regression(POINT* tab, int n, float* a, float* b)
{
	float somme_x, somme_y, somme_x2, somme_xy;
	int i;

	somme_x = 0.;
	somme_y = 0.;
	somme_x2 = 0.;
	somme_xy = 0.;

	for (i = 0; i < n; i++) {
		somme_x += tab[i].x;
		somme_y += tab[i].y;
		somme_x2 += tab[i].x * tab[i].x;
		somme_xy += (tab[i].x * tab[i].y);
	}

	*a = (somme_xy - ((somme_x * somme_y) / (float)n)) / (somme_x2 - ((somme_x * somme_x) / (float)n));
	*b = (somme_y - ((*a) * somme_x)) / (float)n;
}

IMAGE imageSortieRegression(IMAGE img, char axe)
{
	IMAGE result = allocationImage(img.Nblig, img.Nbcol);
	int n; float a, b;
	POINT* tab4 = imageVersPoints(img, &n, axe);
	regression(tab4, n, &a, &b);

	for (int i = 0; i < img.Nblig; i++) {
		int j_reg = a * i + b;
		for(int j = 0; j < img.Nbcol; j++){
			if(j == j_reg)
				result.pixel[i][j] = 2;

			else
				result.pixel[i][j] = img.pixel[i][j];
		}
		
	}
	char* titre = (char*)calloc(20, sizeof(char));
	sprintf(titre, "data/result_%c.ppm", axe);
	sauvegardeImageRGB(conversionImageFausseCouleur(result, "data/ipsi.txt"), "P6", titre);

	return result;
}
/*commencement projet image en C */
IMAGE IoU(IMAGE i1, IMAGE i2, float *IoU, float *GlobalDelta){
    IMAGE diff = allocationImage(i1.Nblig, i1.Nbcol);	// Image de differenciation
    unsigned int intersect = 0, uni = 0;	// nombre de pixels dans l'intersection et l'union
    for(unsigned int i = 0; i < i1.Nbcol * i1.Nblig; i++)
	{
        
		if (i1.data[i] == i2.data[i])		// pixels identiques -> 0 dans diff
			diff.data[i] = 0;
		else								// pixels differents -> 1 dans diff
			diff.data[i] = 255;
        
		if((i1.data[i] != 0) || (i2.data[i] != 0))
            uni++;										// Si une des images a un defaut
        if((i1.data[i] != 0) && (i2.data[i] != 0))
            intersect++;								// Si les deux images ont un defaut
    }
    *IoU = (float)intersect/uni;
	*GlobalDelta = 1 - ((float)(uni - intersect) / (diff.Nblig * diff.Nbcol));
    return diff;
}

STREL diamond(int taille, float valeur)
{
	STREL diamond = allocationStrel(taille, taille);
	int mid = taille >> 1 ;
	for (int i = 0; i < taille; i++) {
		for (int j = 0; j < taille; j++) {
			int d = abs(i - mid) + abs(j - mid);
			if (d < diamond.xradius) {
				diamond.pixel[i][j] = valeur;
			}
		}
	}
	return diamond;
}

STREL disk(int taille, float valeur)
{
	STREL disk = allocationStrel(taille, taille);
	//int disk.xradius = taille >> 1;
	for (int i = 0; i < taille; i++) {
		for (int j = 0; j < taille; j++) {
			int d = sqrt(pow(abs(i - disk.xradius), 2) + pow(abs(j - disk.xradius), 2));
			if (d < taille) {
				disk.pixel[i][j] = valeur;
			}
		}
	}
	return disk;
}

STREL full(int taille, float valeur)
{
	STREL full = allocationStrel(taille, taille);
	for (int i = 0; i < 3; i++)
		for (int j = 0; j < 3; j++)
			full.pixel[i][j] = valeur;

	return full;
}

STREL V4()
{
	STREL v4 = diamond(3, 1.0f/4);
	v4.pixel[1][1] = 0;
	return v4;
}

STREL V8()
{
	STREL v8 = full(3, 1.0f/8);
	v8.pixel[1][1] = 0;
	return v8;
}

VOISINAGE allocVois(STREL strel)
{
	VOISINAGE v;
	v.nb = 0;
	v.pixels = (unsigned char*)malloc(strel.Nbcol * strel.Nblig * sizeof(unsigned char));
	return v;
}



VOISINAGE voisinage(IMAGE img, int y, int x, STREL strel)
{
	VOISINAGE v = allocVois(strel);
	int imin = y - strel.yradius + 1, imax = imin + strel.Nblig;
	int jmin = x - strel.xradius + 1, jmax = jmin + strel.Nbcol;
	unsigned char temp;
	for (int i = imin; i < imax; i++) {
		if (i < 0 || i >= img.Nblig)
			continue;
		for (int j = jmin; j < jmax; j++) {
			if (j < 0 || j >= img.Nbcol)
				continue;
			if (strel.pixel[i - imin][j - jmin] == 0)
				continue;
			if (i == y && j == x)
				continue;

			temp = img.pixel[i][j]; //*strel.pixel[i - x][j - y];
			
			//if (temp <= 0)
				//continue;
			
			v.pixels[v.nb++] = temp;
		}
	}
	return v;
}

unsigned char getVal(VOISINAGE v, char* type)
{
	unsigned char res = 0;
	
	if (v.nb == 0)
		return 0;

	else if (type == "min" || type == "MIN") {
		res = v.pixels[0];
		for (int i = 1; i < v.nb; i++) {
			res = MIN(res, v.pixels[i]);
		}
	}
	else if (type == "max" || type == "MAX") {
		res = v.pixels[0];
		for (int i = 1; i < v.nb; i++) {
			res = MAX(res, v.pixels[i]);
		}
	}
	else if (type == "mean" || type == "MEAN" || type == "moyenne" || type == "MOYENNE" || type == "average" || type == "AVERAGE") {
		for (int i = 0; i < v.nb; i++) {
			res += v.pixels[i];
		}
		res /= v.nb;
	}
	return res;
}

IMAGE filter_repeat(IMAGE (*application)(IMAGE img, STREL strel), IMAGE img, STREL strel, int iterations)
{
	IMAGE result = img;
	for (int i = 0; i < iterations; i++) {
		result = application(result, strel);
	}
	return result;
}

