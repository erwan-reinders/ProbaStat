// noise.cpp : Calcul du bruit d'une image (dif img et flou) en img NDG

#include <stdio.h>
#include "image_ppm.h"

int main(int argc, char* argv[]){
  char cNomImgLue[250], cNomImgBlurred[250], cNomImgOut[250];
  int nH, nW, nTaille;
  int som;

  if (argc != 4){
       printf("Usage: ImageIn.pgm ImageBlured.pgm ImageOut.pgm \n"); 
       exit (1) ;
  }
   
   sscanf(argv[1],"%s",cNomImgLue) ;
   sscanf(argv[2],"%s",cNomImgBlurred);
   sscanf(argv[3],"%s",cNomImgOut);
   

   OCTET *ImgIn, *ImgBlurred, *ImgOut;
   
   lire_nb_lignes_colonnes_image_pgm(cNomImgLue, &nH, &nW);
   nTaille = nH * nW;
  
   allocation_tableau(ImgIn, OCTET, nTaille);
   lire_image_pgm(cNomImgLue, ImgIn, nH * nW);

   allocation_tableau(ImgBlurred, OCTET, nTaille);
   lire_image_pgm(cNomImgBlurred, ImgBlurred, nH * nW);
   
   allocation_tableau(ImgOut, OCTET, nTaille);


   for (int i=0; i < nTaille; i++){
       int difference = ImgIn[i] - ImgBlurred[i];
       difference = (difference<-128)? -128 : (difference>127)? 127 : difference;
       difference+= 128;
       ImgOut[i] = difference;
   }

   ecrire_image_pgm(cNomImgOut, ImgOut,  nH, nW);
   free(ImgIn);free(ImgOut), free(ImgBlurred);
   return 1;
}
