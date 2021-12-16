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
       printf("Usage: ImageIn.ppm largeur_fenetre hauteur_fenetre image_sortie.ppm\n"); 
       exit (1) ;
  }
   
   sscanf(argv[1],"%s",cNomImgLue);
   sscanf(argv[4],"%s",cNomImgsortie);
   int largeur_fenetre = atoi(argv[2]);
   int hauteur_fenetre = atoi(argv[3]);


   OCTET *ImgIn,*ImgOut;
   lire_nb_lignes_colonnes_image_ppm(cNomImgLue, &nH, &nW);
   nTaille = nH * nW;
  
   allocation_tableau(ImgIn, OCTET, nTaille*3);
   lire_image_ppm(cNomImgLue, ImgIn, nH * nW);
   allocation_tableau(ImgOut, OCTET, nTaille*3);

    cout << "TAILLE DU FICHIER INITIAL A LIRE : " << nW << "x" << nH << endl;
    cout << "FENETRE : " << largeur_fenetre*hauteur_fenetre << endl;


    int pas_x = floor((float)largeur_fenetre/4.);
    int pas_y = floor((float)hauteur_fenetre/4.);
    
    cout << "PAS DE X : " << pas_x << endl;
    cout << "PAS DE Y : " << pas_y << endl;


    vector<double> ecart_type_r;
    vector<double> ecart_type_g;
    vector<double> ecart_type_b;

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

            double * nbElementsLus_cadrant_r = new double[256]{};
            double * nbElementsLus_cadrant_g = new double[256]{};
            double * nbElementsLus_cadrant_b = new double[256]{};

            //On doit faire une moyenne des cadrants sur chacun des pixels
            for(int i = 0; i<largeur_fenetre; i++){
                for (int j = 0; j < hauteur_fenetre; j++){
                    nbElementsLus_cadrant_r[ImgIn[(x+i+(y+j)*nW)*3]]++;
                    nbElementsLus_cadrant_g[ImgIn[(x+i+(y+j)*nW)*3+1]]++;
                    nbElementsLus_cadrant_b[ImgIn[(x+i+(y+j)*nW)*3+2]]++; 
                }
            }

            double moyenne_r = 0.;
            double moyenne_g = 0.;
            double moyenne_b = 0.;
            
            for(int i = 0; i<256;i++){
                moyenne_r += (i*nbElementsLus_cadrant_r[i]);
                moyenne_g += (i*nbElementsLus_cadrant_g[i]);
                moyenne_b += (i*nbElementsLus_cadrant_b[i]);
            }

            moyenne_r/=(double)(largeur_fenetre*hauteur_fenetre);
            moyenne_g/=(double)(largeur_fenetre*hauteur_fenetre);
            moyenne_b/=(double)(largeur_fenetre*hauteur_fenetre);

            //cout << "MOYENNE : " << moyenne << endl;

            double variance_r = 0.;
            double variance_g = 0.;
            double variance_b = 0.;

            for(int i = 0; i<256;i++){
                variance_r += (i*i*nbElementsLus_cadrant_r[i]);
                variance_g += (i*i*nbElementsLus_cadrant_g[i]);
                variance_b += (i*i*nbElementsLus_cadrant_b[i]);
            }
            variance_r = (variance_r/(double)(largeur_fenetre*hauteur_fenetre)) - (moyenne_r*moyenne_r);
            variance_g = (variance_g/(double)(largeur_fenetre*hauteur_fenetre)) - (moyenne_g*moyenne_g);
            variance_b = (variance_b/(double)(largeur_fenetre*hauteur_fenetre)) - (moyenne_b*moyenne_b);

            //cout << "VARIANCE : " << variance << endl;
            //double et = sqrt(variance);
            //cout << "ECART TYPE : " << et << endl;
            ecart_type_r.push_back(sqrt(variance_r));
            ecart_type_g.push_back(sqrt(variance_g));
            ecart_type_b.push_back(sqrt(variance_b));

            x = nouv_x;
        }

        {
            //On refait pareil mais sur des largeurs plus faibles
            double * nbElementsLus_cadrant_r = new double[256]{};
            double * nbElementsLus_cadrant_g = new double[256]{};
            double * nbElementsLus_cadrant_b = new double[256]{};

            //Pour les x restants
            for(int i=x;i<nW;i++){
                //On doit faire une moyenne des cadrants sur chacun des pixels
                for (int j = 0; j < hauteur_fenetre; j++){
                    //LA (plus x)
                    nbElementsLus_cadrant_r[ImgIn[(i+(y+j)*nW)*3]]++; 
                    nbElementsLus_cadrant_g[ImgIn[(i+(y+j)*nW)*3 +1]]++;
                    nbElementsLus_cadrant_b[ImgIn[(i+(y+j)*nW)*3 +2]]++;
                }
            }

            double moyenne_r = 0.;
            double moyenne_g = 0.;
            double moyenne_b = 0.;

            for(int i = 0; i<256;i++){
                moyenne_r += (i*nbElementsLus_cadrant_r[i]);
                moyenne_g += (i*nbElementsLus_cadrant_g[i]);
                moyenne_b += (i*nbElementsLus_cadrant_b[i]);
            }
            
            int taille_fenetre = abs(nW - x);
            taille_fenetre *= hauteur_fenetre;

            moyenne_r/=(double)(taille_fenetre);
            moyenne_g/=(double)(taille_fenetre);
            moyenne_b/=(double)(taille_fenetre);

            //cout << "MOYENNE : " << moyenne << endl;

            double variance_r = 0.;
            double variance_g = 0.;
            double variance_b = 0.;

            for(int i = 0; i<256;i++){
                variance_r += (i*i*nbElementsLus_cadrant_r[i]);
                variance_g += (i*i*nbElementsLus_cadrant_g[i]);
                variance_b += (i*i*nbElementsLus_cadrant_b[i]);
            }
            variance_r = (variance_r/(double)(taille_fenetre)) - (moyenne_r*moyenne_r);
            variance_g = (variance_g/(double)(taille_fenetre)) - (moyenne_g*moyenne_g);
            variance_b = (variance_b/(double)(taille_fenetre)) - (moyenne_b*moyenne_b);
            //cout << "VARIANCE : " << variance << endl;
            //cout << "ECART TYPE : " << et << endl;
            ecart_type_r.push_back(sqrt(variance_r));
            ecart_type_g.push_back(sqrt(variance_g));
            ecart_type_b.push_back(sqrt(variance_b));
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

        double * nbElementsLus_cadrant_r = new double[256]{};
        double * nbElementsLus_cadrant_g = new double[256]{};
        double * nbElementsLus_cadrant_b = new double[256]{};

        //On doit faire une moyenne des cadrants sur chacun des pixels
        for(int i = 0; i<largeur_fenetre; i++){
            for (int j = y; j < nH; j++){
                nbElementsLus_cadrant_r[ImgIn[(x+i+j*nW)*3]]++; 
                nbElementsLus_cadrant_g[ImgIn[(x+i+j*nW)*3+1]]++;
                nbElementsLus_cadrant_b[ImgIn[(x+i+j*nW)*3+2]]++;
            }
        }

        double moyenne_r = 0.;
        double moyenne_g = 0.;
        double moyenne_b = 0.;

        for(int i = 0; i<256;i++){
            moyenne_r += (i*nbElementsLus_cadrant_r[i]);
            moyenne_g += (i*nbElementsLus_cadrant_g[i]);
            moyenne_b += (i*nbElementsLus_cadrant_b[i]);
        }

        int taille_fenetre = abs(nH - y);
        taille_fenetre *= largeur_fenetre;

        moyenne_r/=(double)(taille_fenetre);
        moyenne_g/=(double)(taille_fenetre);
        moyenne_b/=(double)(taille_fenetre);

        //cout << "MOYENNE : " << moyenne << endl;

        double variance_r = 0.;
        double variance_g = 0.;
        double variance_b = 0.;

        for(int i = 0; i<256;i++){
            variance_r += (i*i*nbElementsLus_cadrant_r[i]);
            variance_g += (i*i*nbElementsLus_cadrant_g[i]);
            variance_b += (i*i*nbElementsLus_cadrant_b[i]);
        }
        variance_r = (variance_r/(double)(taille_fenetre)) - (moyenne_r*moyenne_r);
        variance_g = (variance_g/(double)(taille_fenetre)) - (moyenne_g*moyenne_g);
        variance_b = (variance_b/(double)(taille_fenetre)) - (moyenne_b*moyenne_b);

        //cout << "VARIANCE : " << variance << endl;
        //cout << "ECART TYPE : " << et << endl;
        ecart_type_r.push_back(sqrt(variance_r));
        ecart_type_g.push_back(sqrt(variance_g));
        ecart_type_b.push_back(sqrt(variance_b));

        x = nouv_x;
    }

    {
        cout << "FIN CALCUL ECART TYPE" << endl;
        cout << "   VALEUR DE x : " << x << endl;
        cout << "   VALEUR DE y : " << y << endl;

        //On refait pareil mais sur des largeurs plus faibles
        double * nbElementsLus_cadrant_r = new double[256]{};
        double * nbElementsLus_cadrant_g = new double[256]{};
        double * nbElementsLus_cadrant_b = new double[256]{};

        //Pour les x restants
        for(int i=x;i<nW;i++){
            //On doit faire une moyenne des cadrants sur chacun des pixels
            for (int j = y; j < nH; j++){
                nbElementsLus_cadrant_r[ImgIn[(i+j*nW)*3]]++;
                nbElementsLus_cadrant_g[ImgIn[(i+j*nW)*3+1]]++;
                nbElementsLus_cadrant_b[ImgIn[(i+j*nW)*3+2]]++; 
            }
        }

        double moyenne_r = 0.;
        double moyenne_g = 0.;
        double moyenne_b = 0.;

        for(int i = 0; i<256;i++){
            moyenne_r += (i*nbElementsLus_cadrant_r[i]);
            moyenne_g += (i*nbElementsLus_cadrant_g[i]);
            moyenne_b += (i*nbElementsLus_cadrant_b[i]);
        }

        int taille_fenetre = abs(nW - x);
        taille_fenetre *= abs(nH - y);

        moyenne_r/=(double)(taille_fenetre);
        moyenne_g/=(double)(taille_fenetre);
        moyenne_b/=(double)(taille_fenetre);

        //cout << "MOYENNE : " << moyenne << endl;

        double variance_r = 0.;
        double variance_g = 0.;
        double variance_b = 0.;

        for(int i = 0; i<256;i++){
            variance_r += (i*i*nbElementsLus_cadrant_r[i]);
            variance_g += (i*i*nbElementsLus_cadrant_g[i]);
            variance_b += (i*i*nbElementsLus_cadrant_b[i]);
        }
        
        variance_r = (variance_r/(double)(taille_fenetre)) - (moyenne_r*moyenne_r);
        variance_g = (variance_g/(double)(taille_fenetre)) - (moyenne_g*moyenne_g);
        variance_b = (variance_b/(double)(taille_fenetre)) - (moyenne_b*moyenne_b);
        //cout << "VARIANCE : " << variance << endl;
        //cout << "ECART TYPE : " << et << endl;
        ecart_type_r.push_back(sqrt(variance_r));
        ecart_type_g.push_back(sqrt(variance_g));
        ecart_type_b.push_back(sqrt(variance_b));
    }



//=========== ON ECRIT SUR LIMAGE FINALE
    cout << "DEBUT ECRITURE IMG FINALE (INIT)" << endl;
    for (int i = 0; i < nTaille; i++){
       ImgOut[i] = 0.;
   }
   

    double etmax_r = -1;
    double etmax_g = -1;
    double etmax_b = -1;

    cout << "   NOMBRE DE CADRANS EN LARGEUR : " << nbcad_x << endl;
    cout << "   NOMBRE DE CADRANS EN HAUTEUR : " << nbcad_y << endl;

    nbcad_x++;

    //Ecriture de l'image sortie
    y = 0;
    int ind_ycad = 0;

    int * nbaddcad = new int[nTaille]{};

    double * val_notn_r = new double[nTaille]{};    
    double * val_notn_g = new double[nTaille]{};
    double * val_notn_b = new double[nTaille]{};

    cout << "DETERMINATION VAL IMG FINALE" << endl;
    while(y+hauteur_fenetre<nH){
        int nouv_y = y + pas_y;
        int ind_xcad = 0;
        int x = 0;
        while(x+largeur_fenetre<nW){
            int nouv_x = x+pas_x;

            double valcad_r = (ecart_type_r[ind_xcad + ind_ycad*nbcad_x]<0)? 0 : ecart_type_r[ind_xcad + ind_ycad*nbcad_x];
            double valcad_g = (ecart_type_g[ind_xcad + ind_ycad*nbcad_x]<0)? 0 : ecart_type_g[ind_xcad + ind_ycad*nbcad_x];
            double valcad_b = (ecart_type_b[ind_xcad + ind_ycad*nbcad_x]<0)? 0 : ecart_type_b[ind_xcad + ind_ycad*nbcad_x];
            //On ajoute une val
            for(int i = 0; i<largeur_fenetre; i++){
                for (int j = 0; j < hauteur_fenetre; j++){
                    val_notn_r[x+i+(y+j)*nW] += valcad_r;
                    val_notn_g[x+i+(y+j)*nW] += valcad_g;
                    val_notn_b[x+i+(y+j)*nW] += valcad_b;

                    nbaddcad[x+i+(y+j)*nW]++;
                }
            }
            x = nouv_x;
            ind_xcad++;
        }

        {
            //On refait pareil mais sur des largeurs plus faibles
            double valcad_r = (ecart_type_r[ind_xcad + ind_ycad*nbcad_x]<0)? 0 : ecart_type_r[ind_xcad + ind_ycad*nbcad_x];
            double valcad_g = (ecart_type_g[ind_xcad + ind_ycad*nbcad_x]<0)? 0 : ecart_type_g[ind_xcad + ind_ycad*nbcad_x];
            double valcad_b = (ecart_type_b[ind_xcad + ind_ycad*nbcad_x]<0)? 0 : ecart_type_b[ind_xcad + ind_ycad*nbcad_x];
            
            //Pour les x restants
            for(int i=x;i<nW;i++){
                //On doit faire une moyenne des cadrants sur chacun des pixels
                for (int j = 0; j < hauteur_fenetre; j++){
                    val_notn_r[i+(y+j)*nW] += valcad_r;
                    val_notn_g[i+(y+j)*nW] += valcad_g;
                    val_notn_b[i+(y+j)*nW] += valcad_b;

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
    cout << "DETERMINATION IMG FINALE HAUTEUR RESTANTES" << endl;
    while(x+largeur_fenetre<nW){
        int nouv_x = x+pas_x;
        
        double valcad_r = (ecart_type_r[ind_xcad + ind_ycad*nbcad_x]<0)? 0 : ecart_type_r[ind_xcad + ind_ycad*nbcad_x];
        double valcad_g = (ecart_type_g[ind_xcad + ind_ycad*nbcad_x]<0)? 0 : ecart_type_g[ind_xcad + ind_ycad*nbcad_x];
        double valcad_b = (ecart_type_b[ind_xcad + ind_ycad*nbcad_x]<0)? 0 : ecart_type_b[ind_xcad + ind_ycad*nbcad_x];

        //On doit faire une moyenne des cadrants sur chacun des pixels
        for(int i = 0; i<largeur_fenetre; i++){
            for (int j = y; j < nH; j++){
                val_notn_r[x+i+j*nW] += valcad_r;
                val_notn_g[x+i+j*nW] += valcad_g;
                val_notn_b[x+i+j*nW] += valcad_b; 
                
                nbaddcad[x+i+j*nW]++;
            }
        }
        ind_xcad++;
        x = nouv_x;
    }

    {
        cout << "FIN DETERMINATION IMG FINALE" << endl;
        cout << "   VALEUR DE x : " << x << endl;
        cout << "   VALEUR DE y : " << y << endl;

        //On refait pareil mais sur des largeurs plus faibles
        double valcad_r = (ecart_type_r[ind_xcad + ind_ycad*nbcad_x]<0)? 0 : ecart_type_r[ind_xcad + ind_ycad*nbcad_x];
        double valcad_g = (ecart_type_g[ind_xcad + ind_ycad*nbcad_x]<0)? 0 : ecart_type_g[ind_xcad + ind_ycad*nbcad_x];
        double valcad_b = (ecart_type_b[ind_xcad + ind_ycad*nbcad_x]<0)? 0 : ecart_type_b[ind_xcad + ind_ycad*nbcad_x];
        //Pour les x restants
        for(int i=x;i<nW;i++){
            //On doit faire une moyenne des cadrants sur chacun des pixels
            for (int j = y; j < nH; j++){
                val_notn_r[x+j*nW] += valcad_r;
                val_notn_g[x+j*nW] += valcad_g;
                val_notn_b[x+j*nW] += valcad_b; 
                
                nbaddcad[x+j*nW]++;
            }
        }
    }

    cout << "ECRITURE IMG FINALE" << endl;
    for (int i = 0; i < nTaille; i++){
        
        val_notn_r[i] /= (double) nbaddcad[i];
        val_notn_g[i] /= (double) nbaddcad[i];
        val_notn_b[i] /= (double) nbaddcad[i];

        if(etmax_r<val_notn_r[i]) etmax_r = val_notn_r[i];
        if(etmax_g<val_notn_g[i]) etmax_g = val_notn_g[i];
        if(etmax_b<val_notn_b[i]) etmax_b = val_notn_b[i];
    }
    cout << "   ETMAX R : " << etmax_r << endl;
    cout << "   ETMAX G : " << etmax_g << endl;
    cout << "   ETMAX B : " << etmax_b << endl;

    //Enfin, on normalize car on a pu écrire plusieurs valeurs dans chacun des pixels de l'image finale
    cout << "ECRITURE IMG FINALE (normalisation)" << endl;
    for (int i = 0; i < nTaille; i++){
        //cout << (int) ImgOut[i] << "/" << nbaddcad[i] << " - ";
        //if(nbaddcad[i]>0) ImgOut[i] = nbaddcad[i];
        //else
            //cout << (int)ImgOut[i] << "-";
        ImgOut[i*3] = ceil((val_notn_r[i] / (etmax_r)) * 255);
        ImgOut[i*3+1] = ceil((val_notn_g[i] / (etmax_g)) * 255);
        ImgOut[i*3+2] = ceil((val_notn_b[i] / (etmax_b)) * 255);
    }
    

    ecrire_image_ppm(cNomImgsortie, ImgOut,  nH, nW);
    free(ImgIn);
    free(ImgOut);
    return 1;
}
