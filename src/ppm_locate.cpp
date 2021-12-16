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
       printf("Usage: ImageIn.ppm nbCadrants_x nbCadrants_y image_sortie\n"); 
       exit (1) ;
  }
   
   sscanf(argv[1],"%s",cNomImgLue);
   sscanf(argv[4],"%s",cNomImgsortie);
   int nb_cadrant_x = atoi(argv[2]);
   int nb_cadrant_y = atoi(argv[3]);
   
   OCTET *ImgIn,*ImgOut;
   
   lire_nb_lignes_colonnes_image_ppm(cNomImgLue, &nH, &nW);
   nTaille = nH * nW;
  
   allocation_tableau(ImgIn, OCTET, nTaille*3);
   lire_image_ppm(cNomImgLue, ImgIn, nH * nW);
   allocation_tableau(ImgOut, OCTET, nTaille*3);

    cout << "TAILLE DU FICHIER INITIAL A LIRE : " << nW << "x" << nH << endl;
    cout << "NOMBRE DE CADRANTS : " << nb_cadrant_x*nb_cadrant_y << endl;


   double* et_cadrants_r = new double[nb_cadrant_x*nb_cadrant_y]{};
   double* et_cadrants_g = new double[nb_cadrant_x*nb_cadrant_y]{};
   double* et_cadrants_b = new double[nb_cadrant_x*nb_cadrant_y]{};


    int pas_x = floor((float)nW/((float)nb_cadrant_x));
    int pas_y = floor((float)nH/((float)nb_cadrant_y));

    double etmax_r = -1;
    double etmax_g = -1;
    double etmax_b = -1;

   //Pour chacun des cadrants
   for(int nbc = 0; nbc <nb_cadrant_y; nbc++){ 
       int depart_y = nbc*pas_y;
       int fin_y = (nbc+1)*pas_y;
       for(int nbc_b = 0; nbc_b <nb_cadrant_x; nbc_b++){ 
            double * nbElementsLus_cadrant_r = new double[256]{};
            double * nbElementsLus_cadrant_g = new double[256]{};
            double * nbElementsLus_cadrant_b = new double[256]{};

            int depart_x = nbc_b*pas_x;
            int fin_x = (nbc_b+1)*pas_x;

            cout << "===== POUR LE CADRANT : x[" << depart_x << ":" << fin_x << "], y[" << depart_y << ":" << fin_y << "] =====" << endl;

            for (int i= depart_y; i < fin_y; i++){
                    for (int j=depart_x; j < fin_x; j++){
                        nbElementsLus_cadrant_r[ImgIn[(i*nW+j)*3]]++;
                        nbElementsLus_cadrant_g[ImgIn[(i*nW+j)*3+1]]++;
                        nbElementsLus_cadrant_b[ImgIn[(i*nW+j)*3+2]]++;
                    }
            }

            int tailleCady = abs(fin_y-depart_y);
            int tailleCadx = abs(fin_x-depart_x);

            cout << tailleCady << "x" << tailleCadx << endl;
            int tailleCad = tailleCady*tailleCadx;
            
            double moyenne_r = 0.;
            double moyenne_g = 0.;
            double moyenne_b = 0.;
            for(int i = 0; i<256;i++){
                moyenne_r += (i*nbElementsLus_cadrant_r[i]);
                moyenne_g += (i*nbElementsLus_cadrant_g[i]);
                moyenne_b += (i*nbElementsLus_cadrant_b[i]);
            }

            moyenne_r/=(double)tailleCad;
            moyenne_g/=(double)tailleCad;
            moyenne_b/=(double)tailleCad;

            //cout << "MOYENNE : " << moyenne << endl;

            double variance_r = 0.;
            double variance_g = 0.;
            double variance_b = 0.;

            for(int i = 0; i<256;i++){
                variance_r += (i*i*nbElementsLus_cadrant_r[i]);
                variance_g += (i*i*nbElementsLus_cadrant_g[i]);
                variance_b += (i*i*nbElementsLus_cadrant_b[i]);
            }
            variance_r = (variance_r/(double)tailleCad) - (moyenne_r*moyenne_r);
            variance_g = (variance_g/(double)tailleCad) - (moyenne_g*moyenne_g);
            variance_b = (variance_b/(double)tailleCad) - (moyenne_b*moyenne_b);
            //cout << "VARIANCE : " << variance << endl;
            //cout << "ECART TYPE : " << et << endl;

            //Sauvegarde et
            double et_r = sqrt(variance_r);
            double et_g = sqrt(variance_g);
            double et_b = sqrt(variance_b);

            et_cadrants_r[nbc_b + nbc*nb_cadrant_x] = et_r;
            et_cadrants_g[nbc_b + nbc*nb_cadrant_x] = et_g;
            et_cadrants_b[nbc_b + nbc*nb_cadrant_x] = et_b;
            
            if(et_r>etmax_r){
                etmax_r = et_r;
            }
            if(et_g>etmax_g){
                etmax_g = et_g;
            }
            if(et_b>etmax_b){
                etmax_b = et_b;
            }
       }
   }

   for (int i = 0; i < nTaille*3; i++){
       ImgOut[i] = 0.;
   }
   

    cout << "ETMAX R : " << etmax_r << endl;
    cout << "ETMAX G : " << etmax_g << endl;
    cout << "ETMAX B : " << etmax_b << endl;

   //Ecriture de l'image sortie
   for(int nbc = 0; nbc <nb_cadrant_y; nbc++){ 
       int depart_y = nbc*pas_y;
       int fin_y = (nbc+1)*pas_y;
       for(int nbc_b = 0; nbc_b <nb_cadrant_x; nbc_b++){ 
            int depart_x = nbc_b*pas_x;
            int fin_x = (nbc_b+1)*pas_x;

            double valcad_r = (et_cadrants_r[nbc_b + nbc*nb_cadrant_x]<0)? 0 : et_cadrants_r[nbc_b + nbc*nb_cadrant_x]/etmax_r * 255;
            double valcad_g = (et_cadrants_g[nbc_b + nbc*nb_cadrant_x]<0)? 0 : et_cadrants_g[nbc_b + nbc*nb_cadrant_x]/etmax_g * 255;
            double valcad_b = (et_cadrants_b[nbc_b + nbc*nb_cadrant_x]<0)? 0 : et_cadrants_b[nbc_b + nbc*nb_cadrant_x]/etmax_b * 255;

            for (int i= depart_y; i < fin_y; i++){
                for (int j=depart_x; j < fin_x; j++){
                    ImgOut[(i*nW+j)*3] = valcad_r;
                    ImgOut[(i*nW+j)*3+1] = valcad_g;
                    ImgOut[(i*nW+j)*3+2] = valcad_b;
                }
            }
       }
   }

   ecrire_image_ppm(cNomImgsortie, ImgOut,  nH, nW);
   free(ImgIn);
   free(ImgOut);
   return 1;
}
