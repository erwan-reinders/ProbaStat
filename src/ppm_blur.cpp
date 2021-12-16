// blur.cpp : Floute une image en couleur

#include <stdio.h>
#include "image_ppm.h"

int main(int argc, char* argv[]){
  char cNomImgLue[250], cNomImgEcrite[250];
  int nH, nW, nTaille;
  int som[3];

  if (argc != 3) {
       printf("Usage: ImageIn.ppm ImageOut.ppm\n"); 
       exit (1) ;
    }
   
   sscanf (argv[1],"%s",cNomImgLue) ;
   sscanf (argv[2],"%s",cNomImgEcrite);

   int puissanceflou = 5;

   int intervale = puissanceflou+puissanceflou+1;
   int deno = intervale*intervale;

   OCTET *ImgIn, *ImgOut;
   
   lire_nb_lignes_colonnes_image_ppm(cNomImgLue, &nH, &nW);
   nTaille = nH * nW;
  
   allocation_tableau(ImgIn, OCTET, nTaille*3);
   lire_image_ppm(cNomImgLue, ImgIn, nH * nW);
   allocation_tableau(ImgOut, OCTET, nTaille*3);
	
    //On a trois composantes
    for (int i=0; i < nTaille*3; i++){
        ImgOut[i]= ImgIn[i];
    }

    for (int i = puissanceflou; i < nH-puissanceflou; i++){
        for (int j = puissanceflou; j < nW-puissanceflou; j++){
            for(int comp = 0; comp<3;comp++){
                som[comp] = 0;
                for (int k = - puissanceflou; k <= puissanceflou; k++){
                    for (int l = -puissanceflou; l <= puissanceflou; l++){
                        som[comp] += ImgIn[((i+k)*nW+j+l)*3 + comp];
                    }   
                }
                
                /*
                som[comp]=
		        ImgIn[((i-1)*nW+j-1)*3 + comp]
                +ImgIn[((i-1)*nW+j)*3 + comp]
                +ImgIn[((i-1)*nW+j+1)*3 + comp]
		        
                +ImgIn[(i*nW+j-1)*3 + comp]
                +ImgIn[(i*nW+j)*3 + comp]
                +ImgIn[(i*nW+j+1)*3 + comp]
		        
                +ImgIn[((i+1)*nW+j-1)*3 + comp]
                +ImgIn[((i+1)*nW+j)*3 + comp]
                +ImgIn[((i+1)*nW+j+1)*3 + comp];
                */
            }

            ImgOut[(i*nW+j)*3]=som[0]/deno;
            ImgOut[(i*nW+j)*3+1]=som[1]/deno;
            ImgOut[(i*nW+j)*3+2]=som[2]/deno;
        }
    }

   ecrire_image_ppm(cNomImgEcrite, ImgOut,  nH, nW);
   free(ImgIn);free(ImgOut);
   return 1;
}
