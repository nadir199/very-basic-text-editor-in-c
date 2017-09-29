#include <windows.h>
#define UNICODE
#define _UNICODE
#include <tchar.h>
#include <time.h>
#include "menu.h"

int GetNumb(FILE *f)
{
    int c=0;
    int nb=-1;
    c=fgetc(f);
    if('0'<=c && c<='9')
        nb=0;
    while('0'<=c && c<='9')
    {
        nb=nb*10 + c-'0';
        c=fgetc(f);
    }
    return nb;
}
int main()
{
    system("MODE con LINES=60 COLS=200");
    SetConsoleTitle("TP TRAITEMENT DE TEXTE SFSD9o - Allaoua - Debbagh");
    int page=1;
    int mov=0;
    char nomFichier[256]="";
    char nomIndex[256]="";
    int *pos=NULL;
    int taitab=0;
    init_index("tes.txt");

    color(4,0);
    DrawBox(10,4,NB_CARAC_LIG_MAX+10,NB_LIG_PAGE_MAX+2);
    DrawBox(posXMenu,posYMenu+14,100,28);

    gotoxy(5,2);
    color(15,0);
    int slide=0;
    int choix=-1;
    int ret=-1;
    int nombPages= 0,nombMots=0,nombCarac=0,nombLignes=0;
    //system("start /shared menu.exe");
    while(1)
    {
        color(4,0);
        DrawBox(10,4,NB_CARAC_LIG_MAX+10,NB_LIG_PAGE_MAX+2);
        DrawBox(posXMenu,posYMenu+14,100,28);
        color(15,0);
        getNomFich(nomFichier);
        getNomIndex(nomIndex);
        gotoxy(60,0);
        printf("MANIPULATION DE FICHIERS TEXTE");
        gotoxy(1,1);
        color(12,15);
        if(ret==-1)
        {
            nombPages=nbPages(nomFichier,&nombLignes);
            nombMots=NbWords(nomFichier);
            nombCarac=nbChar(nomFichier);

            ret=0;
        }
        //nbPages(nomFichier);
        printf("Fichier Manipule : %s  |  Nom de l'index associe : %s |  Nombre de pages : %d |  Nombre de lignes : %d  |  Nombre de mots : %d  |  Nombre de caracteres : %d",nomFichier,nomIndex,nombPages,nombLignes,nombMots,nombCarac);
        color(15,0);
        AffichePage(nomFichier,page,0,0,NULL,0);
        color(0,12);
        gotoxy(POS_X+NB_CARAC_LIG_MAX/2+ 16, POS_Y + NB_LIG_PAGE_MAX +3);
        printf("%d",nombPages);
        color(15,0);
        gotoxy(0,0);

        if(ret)
        {
            ret=Menu();
            if(ret==0)
            {
                EraseZone(posXMenu,posYMenu,70,14);
            }
        }
        else
        {
            slide=0;
            slide = handle_slide();
            if(slide ==0)
                ret=1;
        }


        if(mov=='d' || slide == -1)
            page++;
        else if (mov=='q' || slide == -2)
            page--;
        if(page<1)
            page=1;
        if(page>nombPages)
            page=nombPages;

        EraseZone(15,5,NB_CARAC_LIG_MAX+3,NB_LIG_PAGE_MAX);


    }
    return 0;
}
