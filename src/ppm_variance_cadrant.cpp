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
  char cNomImgLue[250];
  int nH, nW, nTaille;
  
  if (argc != 4){
       printf("Usage: ImageIn.ppm nbCadrants_x nbCadrants_y\n"); 
       exit (1) ;
  }
   
   sscanf(argv[1],"%s",cNomImgLue);
   int nb_cadrant_x = atoi(argv[2]);
   int nb_cadrant_y = atoi(argv[3]);
   
   OCTET *ImgIn;
   
   lire_nb_lignes_colonnes_image_ppm(cNomImgLue, &nH, &nW);
   nTaille = nH * nW;
  
   allocation_tableau(ImgIn, OCTET, nTaille*3);

   lire_image_ppm(cNomImgLue, ImgIn, nH * nW);

    cout << "TAILLE DU FICHIER INITIAL A LIRE : " << nW << "x" << nH << endl;
    cout << "NOMBRE DE CADRANTS : " << nb_cadrant_x*nb_cadrant_y << endl;

   //int nb_cadrant_2 = nb_cadrant/2.;

   //double** nbElementsLus_cadrant = new double*[nb_cadrant];


    int pas_x = floor((float)nW/((float)nb_cadrant_x));
    int pas_y = floor((float)nH/((float)nb_cadrant_y));

    ofstream r_monFlux("r_noise_moy.dat");
    ofstream r_monFlux_b("r_noise_et.dat");
    ofstream g_monFlux("g_noise_moy.dat");
    ofstream g_monFlux_b("g_noise_et.dat");
    ofstream b_monFlux("b_noise_moy.dat");
    ofstream b_monFlux_b("b_noise_et.dat");

    ofstream monFlux("coul_noise_moy.dat");
    ofstream monFlux_b("coul_noise_et.dat");

    if(!r_monFlux || !r_monFlux_b || !g_monFlux || !g_monFlux_b || !b_monFlux || !b_monFlux_b || !monFlux || !monFlux_b){
        cout << "ERREUR: Impossible d'ouvrir le fichier DAT." << endl;
    }

   //Pour chacun des cadrants
   for(int nbc = 0; nbc <nb_cadrant_y; nbc++){ 
       int depart_y = nbc*pas_y;
       int fin_y = (nbc+1)*pas_y;

       for(int nbc_b = 0; nbc_b <nb_cadrant_x; nbc_b++){ 
            //nbElementsLus_cadrant[nbc + nbc_b] = new double[256]{};
            double * r_nbElementsLus_cadrant = new double[256]{};
            double * g_nbElementsLus_cadrant = new double[256]{};
            double * b_nbElementsLus_cadrant = new double[256]{};



            int depart_x = nbc_b*pas_x;
            int fin_x = (nbc_b+1)*pas_x;

            cout << "===== POUR LE CADRANT : x[" << depart_x << ":" << fin_x << "], y[" << depart_y << ":" << fin_y << "] =====" << endl;

            for (int i= depart_y; i < fin_y; i++){
                    for (int j=depart_x; j < fin_x; j++){
                        r_nbElementsLus_cadrant[ImgIn[(i*nW+j)*3]]++;
                        g_nbElementsLus_cadrant[ImgIn[(i*nW+j)*3 +1]]++;
                        b_nbElementsLus_cadrant[ImgIn[(i*nW+j)*3 +2]]++;
                    }
            }

            int tailleCad = abs(fin_y-depart_y) * abs(fin_x-depart_x);
            cout << tailleCad << endl;

            double moyenne_r = 0.;
            double moyenne_g = 0.;
            double moyenne_b = 0.;

            for(int i = 0; i<256;i++){
                moyenne_r += (i*r_nbElementsLus_cadrant[i]);
                moyenne_g += (i*g_nbElementsLus_cadrant[i]);
                moyenne_b += (i*b_nbElementsLus_cadrant[i]);
            }

            moyenne_r/=(double)tailleCad;
            moyenne_g/=(double)tailleCad;
            moyenne_b/=(double)tailleCad;

            cout << "MOYENNE : [r:" << moyenne_r << ",g:" << moyenne_g << ",b:" << moyenne_b << "]" << endl;

            double variance_r = 0.;
            double variance_g = 0.;
            double variance_b = 0.;

            for(int i = 0; i<256;i++){
                variance_r += (i*i*r_nbElementsLus_cadrant[i]);
                variance_g += (i*i*g_nbElementsLus_cadrant[i]);
                variance_b += (i*i*b_nbElementsLus_cadrant[i]);
            }
            variance_r = (variance_r/(double)tailleCad) - (moyenne_r*moyenne_r);
            variance_g = (variance_g/(double)tailleCad) - (moyenne_g*moyenne_g);
            variance_b = (variance_b/(double)tailleCad) - (moyenne_b*moyenne_b);
            
            cout << "VARIANCE : [r:" << variance_r << ",g:" << variance_g << ",b:" << variance_b << "]" << endl;

            double et_r = sqrt(variance_r);
            double et_g = sqrt(variance_g);
            double et_b = sqrt(variance_b);

            cout << "ECART TYPE : [r:" << et_r << ",g:" << et_g << ",b:" << et_b << "]" << endl;

            //Ecriture dans le dat
            r_monFlux << (nbc*nb_cadrant_x + nbc_b) << " " << moyenne_r << endl;
            r_monFlux_b << (nbc*nb_cadrant_x + nbc_b) << " " << et_r << endl;
            g_monFlux << (nbc*nb_cadrant_x + nbc_b) << " " << moyenne_g << endl;
            g_monFlux_b << (nbc*nb_cadrant_x + nbc_b) << " " << et_g << endl;
            b_monFlux << (nbc*nb_cadrant_x + nbc_b) << " " << moyenne_b << endl;
            b_monFlux_b << (nbc*nb_cadrant_x + nbc_b) << " " << et_b << endl;

            monFlux << (nbc*nb_cadrant_x + nbc_b) << " " << (moyenne_r+moyenne_g+moyenne_b)/3 << endl;
            monFlux_b << (nbc*nb_cadrant_x + nbc_b) << " " << (et_r+et_g+et_b)/3 << endl;
       }
   }
   free(ImgIn);
}
