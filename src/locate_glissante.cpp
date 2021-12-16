//Fichier permettant de sauvegarder l'histogramme d'une image données dans un fichier correspondant
#include <stdio.h>
#include "image_ppm.h"
#include <iostream>
#include <fstream>
#include <string>
#include <math.h>
#include <cstdlib>
#include <vector>

using namespace std;

int main(int argc, char* argv[])
{
  char cNomImgLue[250], cNomImgsortie[250];
  int nH, nW, nTaille;
  
  if (argc != 5){
       printf("Usage: ImageIn.pgm largeur_fenetre hauteur_fenetre image_sortie.pgm\n"); 
       exit (1) ;
  }
   
   sscanf(argv[1],"%s",cNomImgLue);
   sscanf(argv[4],"%s",cNomImgsortie);
   int largeur_fenetre = atoi(argv[2]);
   int hauteur_fenetre = atoi(argv[3]);


   OCTET *ImgIn,*ImgOut;
   lire_nb_lignes_colonnes_image_pgm(cNomImgLue, &nH, &nW);
   nTaille = nH * nW;
  
   allocation_tableau(ImgIn, OCTET, nTaille);
   lire_image_pgm(cNomImgLue, ImgIn, nH * nW);
   allocation_tableau(ImgOut, OCTET, nTaille);

    cout << "TAILLE DU FICHIER INITIAL A LIRE : " << nW << "x" << nH << endl;
    cout << "FENETRE : " << largeur_fenetre*hauteur_fenetre << endl;


    int pas_x = floor((float)largeur_fenetre/4.);
    int pas_y = floor((float)hauteur_fenetre/4.);
    
    cout << "PAS DE X : " << pas_x << endl;
    cout << "PAS DE Y : " << pas_y << endl;

    double etmax = -1;

    vector<double> ecart_type;
    int y = 0;
    
    int nbcad_x = 0;
    int nbcad_y = 0;
    cout << "DEBUT REMPLISSAGE ECART TYPE + ETMAX" << endl;
    while(y+hauteur_fenetre<nH){
        int nouv_y = y+pas_y;
        int x = 0;
        while(x+largeur_fenetre<nW){
            if(y == 0) nbcad_x++;
            if(x == 0) nbcad_y++;

            int nouv_x = x+pas_x;

            double * nbElementsLus_cadrant = new double[256]{};
            //On doit faire une moyenne des cadrants sur chacun des pixels
            for(int i = 0; i<largeur_fenetre; i++){
                for (int j = 0; j < hauteur_fenetre; j++){
                    nbElementsLus_cadrant[ImgIn[x+i+(y+j)*nW]]++; 
                }
            }

            double moyenne = 0.;
            for(int i = 0; i<256;i++){
                moyenne += (i*nbElementsLus_cadrant[i]);
            }
            moyenne/=(double)(largeur_fenetre*hauteur_fenetre);

            //cout << "MOYENNE : " << moyenne << endl;

            double variance = 0.;
            for(int i = 0; i<256;i++){
                variance += (i*i*nbElementsLus_cadrant[i]);
            }
            variance = (variance/(double)(largeur_fenetre*hauteur_fenetre)) - (moyenne*moyenne);
            //cout << "VARIANCE : " << variance << endl;
            double et = sqrt(variance);
            //cout << "ECART TYPE : " << et << endl;
            ecart_type.push_back(et);
            if(et>etmax){
                etmax = et;
            }
            x = nouv_x;
        }

        {
            //On refait pareil mais sur des largeurs plus faibles
            double * nbElementsLus_cadrant = new double[256]{};    
            //Pour les x restants
            for(int i=x;i<nW;i++){
                //On doit faire une moyenne des cadrants sur chacun des pixels
                for (int j = 0; j < hauteur_fenetre; j++){
                    //LA (plus x)
                    nbElementsLus_cadrant[ImgIn[i+(y+j)*nW]]++; 
                }
            }

            double moyenne = 0.;
            for(int i = 0; i<256;i++){
                moyenne += (i*nbElementsLus_cadrant[i]);
            }
            
            int taille_fenetre = abs(nW - x);
            taille_fenetre *= hauteur_fenetre;

            moyenne/=(double)(taille_fenetre);

            //cout << "MOYENNE : " << moyenne << endl;

            double variance = 0.;
            for(int i = 0; i<256;i++){
                variance += (i*i*nbElementsLus_cadrant[i]);
            }
            variance = (variance/(double)(taille_fenetre)) - (moyenne*moyenne);
            //cout << "VARIANCE : " << variance << endl;
            double et = sqrt(variance);
            //cout << "ECART TYPE : " << et << endl;
            ecart_type.push_back(et);
            if(et>etmax){
                etmax = et;
            }
        }
        y = nouv_y;
    }

    //On refait pareil mais sur des hauteurs plus faibles
    //Pour les y restants
    cout << "DEBUT REMPLISSAGE ECART TYPE + ETMAX SUR HAUTEURS RESTANTES" << endl;
    cout << "   VALEUR DE y : " << y << endl;
    int x = 0;
    while(x+largeur_fenetre<nW){
        int nouv_x = x+pas_x;
        double * nbElementsLus_cadrant = new double[256]{};
        //On doit faire une moyenne des cadrants sur chacun des pixels
        for(int i = 0; i<largeur_fenetre; i++){
            for (int j = y; j < nH; j++){
                nbElementsLus_cadrant[ImgIn[x+i+j*nW]]++; 
            }
        }

        double moyenne = 0.;
        for(int i = 0; i<256;i++){
            moyenne += (i*nbElementsLus_cadrant[i]);
        }

        int taille_fenetre = abs(nH - y);
        taille_fenetre *= largeur_fenetre;

        moyenne/=(double)(taille_fenetre);

        //cout << "MOYENNE : " << moyenne << endl;

        double variance = 0.;
        for(int i = 0; i<256;i++){
            variance += (i*i*nbElementsLus_cadrant[i]);
        }
        variance = (variance/(double)(taille_fenetre)) - (moyenne*moyenne);
        //cout << "VARIANCE : " << variance << endl;
        double et = sqrt(variance);
        //cout << "ECART TYPE : " << et << endl;
        ecart_type.push_back(et);
        if(et>etmax){
            etmax = et;
        }
        x = nouv_x;
    }

    {
        cout << "FIN CALCUL ECART TYPE" << endl;
        cout << "   VALEUR DE x : " << x << endl;
        cout << "   VALEUR DE y : " << y << endl;

        //On refait pareil mais sur des largeurs plus faibles
        double * nbElementsLus_cadrant = new double[256]{};    
        //Pour les x restants
        for(int i=x;i<nW;i++){
            //On doit faire une moyenne des cadrants sur chacun des pixels
            for (int j = y; j < nH; j++){
                nbElementsLus_cadrant[ImgIn[i+j*nW]]++; 
            }
        }

        double moyenne = 0.;
        for(int i = 0; i<256;i++){
            moyenne += (i*nbElementsLus_cadrant[i]);
        }
        int taille_fenetre = abs(nW - x);
        taille_fenetre *= abs(nH - y);

        moyenne/=(double)(taille_fenetre);

        //cout << "MOYENNE : " << moyenne << endl;

        double variance = 0.;
        for(int i = 0; i<256;i++){
            variance += (i*i*nbElementsLus_cadrant[i]);
        }
        
        variance = (variance/(double)(taille_fenetre)) - (moyenne*moyenne);
        //cout << "VARIANCE : " << variance << endl;
        double et = sqrt(variance);
        //cout << "ECART TYPE : " << et << endl;
        ecart_type.push_back(et);
        if(et>etmax){
            etmax = et;
        }
    }



//=========== ON ECRIT SUR LIMAGE FINALE
    cout << "DEBUT ECRITURE IMG FINALE (INIT)" << endl;
    for (int i = 0; i < nTaille; i++){
       ImgOut[i] = 0.;
   }
   

    cout << "   ETMAX : " << etmax << endl;
    cout << "   NOMBRE DE CADRANS EN LARGEUR : " << nbcad_x << endl;
    cout << "   NOMBRE DE CADRANS EN HAUTEUR : " << nbcad_y << endl;

    nbcad_x++;

    //Ecriture de l'image sortie
    y = 0;
    int ind_ycad = 0;
    int * nbaddcad = new int[nTaille]{};

    cout << "ECRITURE IMG FINALE" << endl;
    while(y+hauteur_fenetre<nH){
        int nouv_y = y + pas_y;
        int ind_xcad = 0;
        int x = 0;
        while(x+largeur_fenetre<nW){
            int nouv_x = x+pas_x;
            double valcad = (ecart_type[ind_xcad + ind_ycad*nbcad_x]<0)? 0 : ecart_type[ind_xcad + ind_ycad*nbcad_x]/etmax * 255;
            //On ajoute une val
            for(int i = 0; i<largeur_fenetre; i++){
                for (int j = 0; j < hauteur_fenetre; j++){
                    ImgOut[x+i+(y+j)*nW] += valcad; 
                    nbaddcad[x+i+(y+j)*nW]++;
                }
            }
            x = nouv_x;
            ind_xcad++;
        }

        {
            //On refait pareil mais sur des largeurs plus faibles
            double valcad = (ecart_type[ind_xcad + ind_ycad*nbcad_x]<0)? 0 : ecart_type[ind_xcad + ind_ycad*nbcad_x]/etmax * 255;
            //Pour les x restants
            for(int i=x;i<nW;i++){
                //On doit faire une moyenne des cadrants sur chacun des pixels
                for (int j = 0; j < hauteur_fenetre; j++){
                    ImgOut[i+(y+j)*nW] += valcad; 
                    nbaddcad[i+(y+j)*nW]++;
                }
            }
        }
        y = nouv_y;
        ind_ycad++;
    }

    cout << "   nb cadrants y deja explores : " << ind_ycad << endl;

    //On refait pareil mais sur des hauteurs plus faibles
    //Pour les y restants
    x = 0;
    int ind_xcad = 0;
    cout << "ECRITURE IMG FINALE HAUTEUR RESTANTES" << endl;
    while(x+largeur_fenetre<nW){
        int nouv_x = x+pas_x;
        
        double valcad = (ecart_type[ind_xcad + ind_ycad*nbcad_x]<0)? 0 : ecart_type[ind_xcad + ind_ycad*nbcad_x]/etmax * 255;

        //On doit faire une moyenne des cadrants sur chacun des pixels
        for(int i = 0; i<largeur_fenetre; i++){
            for (int j = y; j < nH; j++){
                ImgOut[x+i+j*nW] += valcad; 
                nbaddcad[x+i+j*nW]++;
            }
        }

        ind_xcad++;
        x = nouv_x;
    }

    {
        cout << "FIN ECRITURE IMG FINALE" << endl;
        cout << "   VALEUR DE x : " << x << endl;
        cout << "   VALEUR DE y : " << y << endl;

        //On refait pareil mais sur des largeurs plus faibles
        double valcad = (ecart_type[ind_xcad + ind_ycad*nbcad_x]<0)? 0 : ecart_type[ind_xcad + ind_ycad*nbcad_x]/etmax * 255;
        //Pour les x restants
        for(int i=x;i<nW;i++){
            //On doit faire une moyenne des cadrants sur chacun des pixels
            for (int j = y; j < nH; j++){
                ImgOut[x+j*nW] += valcad; 
                nbaddcad[x+j*nW]++;
            }
        }
    }

    //Enfin, on normalize car on a pu écrire plusieurs valeurs dans chacun des pixels de l'image finale
    cout << "FIN ECRITURE IMG FINALE (normalisation)" << endl;
    /*for (int i = 0; i < nTaille; i++){
        //cout << (int) ImgOut[i] << "/" << nbaddcad[i] << " - ";
        if(nbaddcad[i]>0) ImgOut[i] /= nbaddcad[i];
        //else
            //cout << (int)ImgOut[i] << "-";
    }*/
    

    ecrire_image_pgm(cNomImgsortie, ImgOut,  nH, nW);
    free(ImgIn);
    free(ImgOut);
    return 1;
}
