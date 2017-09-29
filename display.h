#ifndef DISPLAY_H_INCLUDED
#define DISPLAY_H_INCLUDED
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

#define NB_LIG_PAGE_MAX 40
#define NB_CARAC_LIG_MAX 60

#define posXMenu 81
#define posYMenu 4

#define POS_X 5
#define POS_Y 2


//GETTERS
void getNomFich(char nom[]);
void getNomIndex(char nom[]);

//SETTERS
void setNomFich(char *nom);
void setNomIndex(char *nom);


int separateur(char c);
int espacement(char c);
void ClearChaine(char *chaine,int longu);
void gotoxy (int x, int y);
void color(int t,int f);
void DrawBox(int x,int y,int sizeX,int sizeY);
void EraseZone(int x,int y,int sizeX,int sizeY);
void suppSpace(char *chaine);


void GotoZePage(FILE *f,int page);
char * GotoLine(FILE *f,int page,int line);
void AffichePage(char *nom_fichier,int page, int MODE , int LINE,char *mot,int APPROX);
/** FONCTIONS AFFICHAGE*/
int fonc_dec_balise(FILE * f,int *numChar);
int traiter_dep_mort(FILE *f,char *chaine);
int arrangeLigne(FILE *f,char *chaine,int *numChar);
int NbWords(char * nom_fich);

/** FONCTIONS INDEX */
char * getNextWord(FILE *f,int *numChar);
void init_index(char *nom_fich);
//int *recherche_index(char *nom_fich,char *mot,int *nbOcc);
int recherche_index(char *nom_fich,char *mot,int occNumb,int APPROX,int *longMot,char *MotRemp,int *OUTF);

/** RECHERCHE */
int DisLevenshtein(char *mot,char *mot2);
int LettreInvers(char *ch,char *autre);
int EqAprrox(char *mot,char *mot2);
void GotoZeChar(char *nom_fich, int leChar, int *PAGE, int *LINE );
int CritereRecherche(char *chaine,char *autre, int approxOUPAS);

int RemplacerUnConf(char *nom_fich,char *mot,char *motRemp,int mYemMot,int CONFONOFF,int APPROX);
void RemplacerOnebyOne(char *nom_fich,char *mot,char *motRemp,int APPROX);
void RemplacerTout(char *nom_fich,char *mot,char *motRemp,int APPROX);


void getNomFich(char nom[]);
void getNomIndex(char nom[]);
#endif // DISPLAY_H_INCLUDED
