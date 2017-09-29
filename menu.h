#ifndef MENU_H_INCLUDED
#define MENU_H_INCLUDED
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <conio.h>
#include "display.h"



int lireAlphaNum(char *result, int longueur,int x,int y);
void scanAlphaNum(char *retour ,int x,int y,int sizeX);
void printAnim(char *chaine,int delay);
int yesNoQuestion(char *ch,int xPlus);
void InitChaine(char *chaine,int longu,const char *input);
char * demandeNomFich();

int handle_slide();
void DispMenu(int *select,char *mov,int *sousMenu,int *selSous,int *justOpenedSousMenu);
void HandMenu(int *choix,int *sousChoix,int *sousMenu,char *mov,int *justOpenedSousMenu,int *quit);
int Menu();

#define ZONE_SAISIE_X 59
#define ZONE_SAISIE_Y 51


int nbPos(int n);
double puiss(double n,int p);



#endif // MENU_H_INCLUDED
