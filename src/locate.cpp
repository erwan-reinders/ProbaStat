//Fichier permettant de sauvegarder l'histogramme d'une image données dans un fichier correspondant
#include <stdio.h>
#include "image_ppm.h"
#include <iostream>
#include <fstream>
#include <string>
#include <math.h>
#include <cstdlib>

using namespace std;

int main(int argc, char* argv[])
{
  char cNomImgLue[250], cNomImgsortie[250];
  int nH, nW, nTaille;
  
  if (argc != 5){
       printf("Usage: ImageIn.pgm nbCadrants_x nbCadrants_y image_sortie\n"); 
       exit (1) ;
  }
   
   sscanf(argv[1],"%s",cNomImgLue);
   sscanf(argv[4],"%s",cNomImgsortie);
   int nb_cadrant_x = atoi(argv[2]);
   int nb_cadrant_y = atoi(argv[3]);
   
   OCTET *ImgIn,*ImgOut;
   
   lire_nb_lignes_colonnes_image_pgm(cNomImgLue, &nH, &nW);
   nTaille = nH * nW;
  
   allocation_tableau(ImgIn, OCTET, nTaille);
   lire_image_pgm(cNomImgLue, ImgIn, nH * nW);
   allocation_tableau(ImgOut, OCTET, nTaille);

    cout << "TAILLE DU FICHIER INITIAL A LIRE : " << nW << "x" << nH << endl;
    cout << "NOMBRE DE CADRANTS : " << nb_cadrant_x*nb_cadrant_y << endl;


   double* et_cadrants = new double[nb_cadrant_x*nb_cadrant_y]{};


    int pas_x = floor((float)nW/((float)nb_cadrant_x));
    int pas_y = floor((float)nH/((float)nb_cadrant_y));

    double etmax = -1;
   //Pour chacun des cadrants
   for(int nbc = 0; nbc <nb_cadrant_y; nbc++){ 
       int depart_y = nbc*pas_y;
       int fin_y = (nbc+1)*pas_y;
       for(int nbc_b = 0; nbc_b <nb_cadrant_x; nbc_b++){ 
            double * nbElementsLus_cadrant = new double[256]{};

            int depart_x = nbc_b*pas_x;
            int fin_x = (nbc_b+1)*pas_x;

            cout << "===== POUR LE CADRANT : x[" << depart_x << ":" << fin_x << "], y[" << depart_y << ":" << fin_y << "] =====" << endl;

            for (int i= depart_y; i < fin_y; i++){
                    for (int j=depart_x; j < fin_x; j++){
                        nbElementsLus_cadrant[ImgIn[i*nW+j]]++;
                    }
            }

            int tailleCady = abs(fin_y-depart_y);
            int tailleCadx = abs(fin_x-depart_x);
            cout << tailleCady << "x" << tailleCadx << endl;
            int tailleCad = tailleCady*tailleCadx;

            
            double moyenne = 0.;
            for(int i = 0; i<256;i++){
                moyenne += (i*nbElementsLus_cadrant[i]);
            }
            moyenne/=(double)tailleCad;

            //cout << "MOYENNE : " << moyenne << endl;

            double variance = 0.;
            for(int i = 0; i<256;i++){
                variance += (i*i*nbElementsLus_cadrant[i]);
            }
            variance = (variance/(double)tailleCad) - (moyenne*moyenne);
            //cout << "VARIANCE : " << variance << endl;
            double et = sqrt(variance);
            //cout << "ECART TYPE : " << et << endl;

            //Sauvegarde et
            et_cadrants[nbc_b + nbc*nb_cadrant_x] = et;
            if(et>etmax){
                etmax = et;
            }
       }
   }

   for (int i = 0; i < nTaille; i++){
       ImgOut[i] = 0.;
   }
   

    cout << "ETMAX : " << etmax << endl;
   //Ecriture de l'image sortie
   for(int nbc = 0; nbc <nb_cadrant_y; nbc++){ 
       int depart_y = nbc*pas_y;
       int fin_y = (nbc+1)*pas_y;
       for(int nbc_b = 0; nbc_b <nb_cadrant_x; nbc_b++){ 
            double * nbElementsLus_cadrant = new double[256]{};

            int depart_x = nbc_b*pas_x;
            int fin_x = (nbc_b+1)*pas_x;

            double valcad = (et_cadrants[nbc_b + nbc*nb_cadrant_x]<0)? 0 : et_cadrants[nbc_b + nbc*nb_cadrant_x]/etmax * 255;

            for (int i= depart_y; i < fin_y; i++){
                for (int j=depart_x; j < fin_x; j++){
                        ImgOut[i*nW+j] = valcad;
                }
            }
       }
   }

   ecrire_image_pgm(cNomImgsortie, ImgOut,  nH, nW);
   free(ImgIn);
   free(ImgOut);
   return 1;
}
