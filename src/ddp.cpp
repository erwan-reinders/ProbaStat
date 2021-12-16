//Fichier permettant de sauvegarder l'histogramme d'une image données dans un fichier correspondant
#include <stdio.h>
#include "image_ppm.h"
#include <iostream>
#include <fstream>
#include <string>

using namespace std;

int main(int argc, char* argv[])
{
  char cNomImgLue[250], cNomDDP[250];
  int nH, nW, nTaille;

  
  if (argc != 3){
       printf("Usage: ImageIn.pgm HISTO\n"); 
       exit (1) ;
  }
   
   sscanf (argv[1],"%s",cNomImgLue);
   sscanf (argv[2],"%s",cNomDDP);


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

    for (int i=0; i < 256; i++){
        nbElementsLus[i] /= (double) nTaille;
    }
    //Ecriture du fichier .bat
    for(int i = 0; i<256; i++){
        cout << nbElementsLus[i] << endl;
    }

    ofstream monFlux(cNomDDP);

    if(monFlux){
        for(int i = 0; i<256; i++){
           monFlux << i << " " << nbElementsLus[i] << endl;
        }
    }
    else
    {
        cout << "ERREUR: Impossible d'ouvrir le fichier." << endl;
    }

   free(ImgIn);
   return 1;
}
