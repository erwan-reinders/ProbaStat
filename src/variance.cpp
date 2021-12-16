//Fichier permettant de sauvegarder l'histogramme d'une image données dans un fichier correspondant
#include <stdio.h>
#include "image_ppm.h"
#include <iostream>
#include <fstream>
#include <string>
#include <math.h>

using namespace std;

int main(int argc, char* argv[])
{
  char cNomImgLue[250];
  int nH, nW, nTaille;

  
  if (argc != 2){
       printf("Usage: ImageIn.pgm\n"); 
       exit (1) ;
  }
   
   sscanf (argv[1],"%s",cNomImgLue);

   OCTET *ImgIn;
   
   lire_nb_lignes_colonnes_image_pgm(cNomImgLue, &nH, &nW);
   nTaille = nH * nW;
  
   allocation_tableau(ImgIn, OCTET, nTaille);
   lire_image_pgm(cNomImgLue, ImgIn, nH * nW);


    double* nbElementsLus = new double[256]{};


    for (int i=0; i < nH; i++){
        for (int j=0; j < nW; j++){
            nbElementsLus[ImgIn[i*nW+j]]++;
        }
    }

    double moyenne = 0.;
    for(int i = 0; i<256;i++){
        moyenne += (i*nbElementsLus[i]);
    }
    moyenne/=(double)nTaille;

    cout << "MOYENNE : " << moyenne << endl;

    double variance = 0.;
    for(int i = 0; i<256;i++){
        variance += (i*i*nbElementsLus[i]);
    }
    variance = (variance/(double)nTaille) - (moyenne*moyenne);
    cout << "VARIANCE : " << variance << endl;
    cout << "ECART TYPE : " << sqrt(variance) << endl;

   free(ImgIn);
   return 1;
}
