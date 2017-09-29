#include "menu.h"


int lireAlphaNum(char *result, int longueur,int x,int y)
{
    char chaine[256];
    char c;
    int i=0;

    do
    {
        c=getch(); //RECUPERE UN CARACTERE
        if (i<longueur && ( ( c>='0' && c<='9')||(c>='a' && c<='z')||(c>='A' && c<='Z') ) ) // SI ON A PAS DEPASSE LA LONGUEUR
        {
            result[i]=c; // ON STOCKE LA CHAINE
            printf("%c",result[i]); // ON AFFICHE LE CARACTERE POUR SIMULER L'ECRITURE
            if(result[0]=='\n') // TRAITER LE CAS DU PREMIER CARACTERE \n POUR EVITER LE SAUT
            {
                gotoxy(x,y);//SE BRANCHER A LA POSITION D'ECRITURE
                return 0;
            }
            i++; // ON INCREMENTE LA LONGUEUR
        }
        if(c==8) // POUR EFFACER (BACK SPACE)
        {
            printf("%c",8);
            result[i]='\0';
            EraseZone(x+i-1,y,1,1); // ON EFFACE LE CARACTeRE A EFFACER
            i--; // ON DECREMENTE LA LONGUEUR
            if(i<0) //TRAITER LE CAS CHAINE NULLE
                i=0;
            gotoxy(x+i,y);// ON SE BRANCHE
        }
        //}

    }while (( (c!= NULL && c!=13 )) || i==0 );
    result[i]='\0'; // FIN DE CHAINE

    return 1;
}

void scanAlphaNum(char *retour ,int x,int y,int sizeX)
{
    int n;
    do
    {
        //EraseZone(x,y,sizeX,1);
        if(x>0 && y>0);
        gotoxy(x,y);
        fflush (stdout);
        n = lireAlphaNum(retour,sizeX,x,y);//scanf ("%d", retour);
        //clean();
    }while (n != 1);
}

void printAnim(char *chaine,int delay)
{
    int i;
    for(i=0;i<strlen(chaine);i++)
    {
        printf("%c",chaine[i]);
        Sleep(delay);
        if( _kbhit())
            getch();
    }
}
int yesNoQuestion(char *ch,int xPlus)
{
    int reprendre=1;
    char reponse;
    DrawBox(25,10,100,10);
    gotoxy(30+xPlus,15);
    printAnim(ch,5);
    gotoxy(35+xPlus,25);
    do
    {

        gotoxy(73,17); if(reprendre) color(12,15);printf("OUI");color(0,15);
        gotoxy(73,18); if(!reprendre) color(12,15);printf("NON");color(0,15);
        reponse=getch();
        while ( (reponse) == -32 /* && *select < nombreMenu-1 */)
        {
            reponse=getch();
            if(reponse==80)
            {
                    reprendre++;
                    if(reprendre >1)
                    {
                        reprendre= 0;
                    }

            }

            if (reponse == 72/* && *select */)
            {
                   reprendre--;
                    if(reprendre <0)
                    {
                        reprendre= 1;
                    }
            }

        }
    }while(reponse!=13);
    color(15,0);
    EraseZone(0,0,200,30);
    return reprendre;
}
void InitChaine(char *chaine,int longu,const char *input)
{
    int i=0;
    if(longu>=strlen(input))
        for(i=0;i<strlen(input);i++)
            chaine[i]=input[i];
        chaine[i]='\0';
}
char* demandeNomFich()
{
     system("cls");
    char chemin[256]="Results/";
    //char nomFichier[256];
    char *nomFichier = malloc(256*sizeof(char));
    char nomExtension[256];
    char quest[256]="Nom choisi \"";
 // ON CREE LE REPERTOIRE QUI CONTIENDRA LES FICHIERS RESULTATS ET ON VERIEFIE QUE LA CREATION S'EST BIEN DEROULEE
    if(yesNoQuestion("Choisissez le nom de votre fichier",5) )
    {
        system("mkdir Results 2> nul");

        do
        {
            InitChaine(quest,256,"Nom choisi : \"");
            InitChaine(chemin,256,"Results/");
            system("cls");
            gotoxy(20,40);
            printf("Entrez le nom du fichier? (Alpha.Num sans Extension)");
            color(2,15);gotoxy(40,40);scanAlphaNum (nomFichier,45,41,20);color(0,15);

            gotoxy(20,44);
            printf("Donnez l'extension");
            color(2,15);gotoxy(40,45);scanAlphaNum (nomExtension,45,45,20);color(0,15);
            strcat(nomFichier,".");
            strcat(nomFichier,nomExtension);
            strcat(chemin,nomFichier);
            strcat(quest,nomFichier);
            strcat(quest,"\" (Valider?)");
        }while(!yesNoQuestion(quest,3));
    }
     EraseZone(15,5,NB_CARAC_LIG_MAX+3,NB_LIG_PAGE_MAX);
     return nomFichier;
}




#define COUL_MENU_FOND 15
#define COUL_MENU_CADRE 15
#define COUL_MENU_TEXT 0
#define FLECHE {color(12,COUL_MENU_FOND);printf("<----");color(3,COUL_MENU_CADRE);printf("%c\n",186);}else {color(0,COUL_MENU_FOND);printf("     ");color(3,COUL_MENU_CADRE);printf("%c\n",186);}
#define nombreMenu 8
#define HAND_COL(x) gotoxy(posXMenu+22,posYMenu+4+x);color(3,COUL_MENU_CADRE);printf("%c",186);color(COUL_MENU_TEXT,COUL_MENU_FOND);if(*select == x) color(12,COUL_MENU_FOND); else color(0,COUL_MENU_FOND);



int tailleSousMenus[nombreMenu]={0};

int handle_slide()
{
    #define DISTANCE_SLIDE 10

    int sync=0,syncH=0;
    int horl=0,horlAp=0;
    int DROITE=0;
    HANDLE hConsole = GetStdHandle(STD_INPUT_HANDLE);
    DWORD dwPreviousMode = 0;
    GetConsoleMode(hConsole, &dwPreviousMode);
    DWORD dwNewMode = dwPreviousMode | ENABLE_MOUSE_INPUT;
    dwNewMode &= ~ENABLE_QUICK_EDIT_MODE;
    SetConsoleMode(hConsole, dwNewMode|ENABLE_EXTENDED_FLAGS);
    BOOL fContinue=1;
    DWORD dwEvents;
    INPUT_RECORD input;
    int SLIDED=0;
    int DisAv=0;
    int DisAp=0;
    int justDONE=0;
    horl=clock();
    int prevEvent=0;
    int choix =0;
    while(! SLIDED && fContinue && ReadConsoleInput(hConsole, &input, 1, &dwEvents) && dwEvents > 0)
    {
        switch (input.EventType)
        {
            case KEY_EVENT:
                if (input.Event.KeyEvent.wVirtualKeyCode == VK_SPACE)
                {
                    fContinue = FALSE;
                    return 0;
                }
            break;
            case MOUSE_EVENT:
                horlAp=clock();
                /*_tprintf(TEXT("X=%d, Y=%d; buttons=%x, shift=%x, flags=%x\n"),
                 input.Event.MouseEvent.dwMousePosition.X,
                 input.Event.MouseEvent.dwMousePosition.Y,
                 input.Event.MouseEvent.dwButtonState,
                 input.Event.MouseEvent.dwControlKeyState,
                 input.Event.MouseEvent.dwEventFlags);*/
                 if(prevEvent==0 && input.Event.MouseEvent.dwButtonState ==1)
                 {
                     DisAv=input.Event.MouseEvent.dwMousePosition.X;
                     DisAp=DisAv;
                 }


                 if(input.Event.MouseEvent.dwButtonState == 0  )
                 {
                     horl=horlAp;
                     DisAp=0;
                     DisAv=0;
                 }
                 if(input.Event.MouseEvent.dwEventFlags == 0)
                    justDONE=0;
                 if(!justDONE &&  horlAp-horl>300 && input.Event.MouseEvent.dwButtonState==1 )
                 {
                     DisAp=input.Event.MouseEvent.dwMousePosition.X;

                 }
                 if(!justDONE && input.Event.MouseEvent.dwButtonState==1 && (DisAp-DisAv>DISTANCE_SLIDE ) || (DisAp-DisAv)<-DISTANCE_SLIDE)
                 {
                     DROITE = DisAp-DisAv>DISTANCE_SLIDE;
                     DisAv=input.Event.MouseEvent.dwMousePosition.X;
                     DisAp=DisAv;
                     horl=horlAp;
                     justDONE=1;
                     SLIDED=1;
                 }

            break;
        }
        prevEvent=input.Event.MouseEvent.dwButtonState;


        /*syncH=clock();
        if(syncH-sync>100)
        {
            sync=syncH;
            FILE *reception = fopen("tmp.trsm","r+");
            choix = GetNumb(reception);
            rewind(reception);
            fprintf(reception,"%d",-1);
            fclose(reception);
            if(choix!=-1)
                return choix;
        } */
    }
    SetConsoleMode(hConsole, dwPreviousMode|ENABLE_EXTENDED_FLAGS);
    if (SLIDED)
        if(DROITE)
            return -1;
        else
            return -2;
}

void DispMenu(int *select,char *mov,int *sousMenu,int *selSous,int *justOpenedSousMenu)
{

    /** PARTIE MENU PRINCIPAL */
    gotoxy(posXMenu+13,posYMenu);printf("              %c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c\n",201,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,187);
    gotoxy(posXMenu+13,posYMenu+1);printf("              %c                    MENU                     %c\n",186,186);
    gotoxy(posXMenu+13,posYMenu+2);printf("              %c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c\n\n",200,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,188);
    color(3,COUL_MENU_CADRE);
    gotoxy(posXMenu+22,posYMenu+3);printf("%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c",201,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,187);
    color(0,COUL_MENU_FOND);

    HAND_COL(0)printf("      1 . Choisir le fichier texte                "); if (*select==0 && !*sousMenu) FLECHE
    HAND_COL(1)printf("      2 . Rechercher un Mot                       "); if (*select==1 && !*sousMenu) FLECHE
    HAND_COL(2)printf("      3 . Remplacer un Mot (Toutes Occurences)    "); if (*select==2 && !*sousMenu) FLECHE
    HAND_COL(3)printf("      4 . Remplacer One By One                    "); if (*select==3 && !*sousMenu) FLECHE
    HAND_COL(4)printf("      5 . Recherche approximative d'un mot        "); if (*select==4 && !*sousMenu) FLECHE
    HAND_COL(5)printf("      6 . Remplacer un Mot APPROX                 "); if (*select==5 && !*sousMenu) FLECHE

    HAND_COL(6)printf("      7 . Remplacer One By One APPROX             "); if (*select==6 && !*sousMenu) FLECHE
    /*HAND_COL(7)printf("      8 . ////////////////////////////////////////"); if (*select==nombreMenu-4 && !*sousMenu) FLECHE
    HAND_COL(8)printf("      9 . ////////////////////////////////////////"); if (*select==nombreMenu-3 && !*sousMenu) FLECHE
    HAND_COL(9)printf("      10. ////////////////////////////////////////"); if (*select==nombreMenu-2 && !*sousMenu) FLECHE*/
    HAND_COL(nombreMenu-1)printf("      11. Quitter                                 "); if (*select==nombreMenu-1 && !*sousMenu) FLECHE

    printf("\n");
    color(3,COUL_MENU_CADRE);
    gotoxy(posXMenu+22,posYMenu+4+nombreMenu);printf("%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c",200,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,188);
    color(0,COUL_MENU_FOND);

    color(15,0);

    /** PARTIE SOUS MENU */
    if(*sousMenu)
    {
    }

}
void HandMenu(int *choix,int *sousChoix,int *sousMenu,char *mov,int *justOpenedSousMenu,int *quit)
{
        *mov=getch();
        while ( (*mov) == -32 /* && *choix < nombreMenu-1 */)
        {
            *mov=getch();
            if(*mov==80)
            {
                if(!*sousMenu)
                {

                    (*choix)++;
                    if(*choix >nombreMenu-1)
                    {
                        *choix= 0;
                    }
                }
                else
                {
                    (*sousChoix)++;
                    if(*sousChoix>tailleSousMenus[*choix]-1)
                    {
                        *sousChoix=0;
                    }
                }
            }

            if (*mov == 72/* && *choix */)
            {
                if(!*sousMenu)
                {
                    (*choix)--;
                    if(*choix <0)
                    {
                        *choix= nombreMenu-1;
                    }
                }
                else
                {
                    (*sousChoix)--;
                    if(*sousChoix<0)
                        *sousChoix = tailleSousMenus[*choix]-1;
                }
            }

        }

        if(*mov == 13 && tailleSousMenus[*choix])
        {

            if(!*sousMenu )
                *justOpenedSousMenu=1; // POUR SAVOIR QU'ON VIENT JUSTE D'OUVRIR UN SOUS MENU DONC LA PREMIERE OPERATION DU SOUS MENU N'EST PAS VALIDEE DIRECTEMENT
            *sousMenu=1;
            //*sousChoix=0;
        }
        if (toupper(*mov)=='Q'&& *sousMenu)
        {
            *sousChoix=0;
            *sousMenu=0;
            EraseZone(58,43,51,10);
        }
        else if (toupper(*mov) == 'Q')
        {
            *choix = nombreMenu-1;
            *sousMenu=0;
            *mov=13;
        }


        if (*mov==13)
        {
            setLastLine(-1);

            char mot[256]="";
            char motRemp[256]="";
            char nomDuFich[256]="";
            char nomDeLind[256]="";
            char *nomTmp=NULL;
            int longueur=0;
            switch(*choix)
            {
                case 0:
                    nomTmp=demandeNomFich();
                    init_index(nomTmp);

                    free(nomTmp);
                    EraseZone(0,0,200,60);
                    break;
                case 1:
                        getNomFich(nomDuFich);
                        getNomIndex(nomDeLind);
                        color(15,0);
                        gotoxy(posXMenu+2,posYMenu+15);
                        printf("Entrez le mot a rechercher");
                        scanAlphaNum(mot,posXMenu+2,posYMenu+16,256);
                        int leoutf=0;
                        if(mot)
                        {
                            int i=1;
                            int pos=0;
                            while(pos>=0)
                            {
                                // CHANGER LE NOM METTRE GLOBAL

                                pos = recherche_index(nomDeLind,mot,i,0,&longueur,motRemp,&leoutf);
                                int ligne=0,page=0;
                                GotoZeChar(nomDuFich,pos,&page,&ligne);
                                if(pos<0)
                                    ligne=-1;
                                EraseZone(15,5,NB_CARAC_LIG_MAX+3,NB_LIG_PAGE_MAX);
                                color(15,0);
                                AffichePage(nomDuFich,page,2,ligne,mot,0);
                                getch();
                                i++;
                            }
                            gotoxy(posXMenu+4,posYMenu+18);
                            color(0,12);
                            printf("Fin de la recherche ");color(15,0);
                            if(pos<0 && i==2)
                            {
                                gotoxy(posXMenu+5,posYMenu+18);
                                color(0,12);
                                printf("Aucune occurence de ");color(15,12);printf("%s",mot);color(0,12);printf(" dans le texte.");color(15,0);

                            }


                        }
                        getch();
                        EraseZone(posXMenu+2,posYMenu+14,256,10);
                    break;
                case 2:
                        color(15,0);
                        getNomFich(nomDuFich);
                        gotoxy(posXMenu+2,posYMenu+15);
                        printf("Entrez le mot a remplacer");
                        scanAlphaNum(mot,posXMenu+2,posYMenu+16,256);
                        gotoxy(posXMenu+2,posYMenu+17);
                        printf("Entrez nouveau mot");
                        scanAlphaNum(motRemp,posXMenu+2,posYMenu+18,256);
                        if(mot)
                        {
                            RemplacerTout(nomDuFich,mot,motRemp,0);
                        }
                        EraseZone(posXMenu+2,posYMenu+14,256,10);
                    break;
                case 3:
                        color(15,0);
                        getNomFich(nomDuFich);
                        getNomIndex(nomDeLind);
                        gotoxy(posXMenu+2,posYMenu+15);
                        printf("Entrez le mot a remplacer");
                        scanAlphaNum(mot,posXMenu+2,posYMenu+16,256);
                        gotoxy(posXMenu+2,posYMenu+17);
                        printf("Entrez nouveau mot");
                        scanAlphaNum(motRemp,posXMenu+2,posYMenu+18,256);
                        int pourApprox=recherche_index(nomDeLind,mot,1,0,&longueur,motRemp,&leoutf);
                        if(mot && pourApprox<0)
                        {
                            EraseZone(15,5,NB_CARAC_LIG_MAX+3,NB_LIG_PAGE_MAX);
                            EraseZone(0,0,200,60);
                            if(yesNoQuestion("Aucune occurence n'a ete trouvee, voulez-vous effectuer une recherche approximative du mot?",0))
                            {
                                RemplacerOnebyOne(nomDuFich,mot,motRemp,1);
                            }
                            color(15,0);
                        }
                        else if (mot)
                        {
                            RemplacerOnebyOne(nomDuFich,mot,motRemp,0);
                        }
                        EraseZone(posXMenu+2,posYMenu+14,256,10);
                    break;
                case 4:
                        getNomFich(nomDuFich);
                        getNomIndex(nomDeLind);
                        color(15,0);
                        gotoxy(posXMenu+2,posYMenu+15);
                        printf("Entrez le mot a rechercher");
                        scanAlphaNum(mot,posXMenu+2,posYMenu+16,256);
                        if(mot)
                        {
                            int i=1;
                            int pos=0;
                            while(pos>=0)
                            {
                                // CHANGER LE NOM METTRE GLOBAL
                                pos = recherche_index(nomDeLind,mot,i,1,&longueur,motRemp,&leoutf);
                                int ligne=0,page=0;
                                GotoZeChar(nomDuFich,pos,&page,&ligne);
                                if(pos<0)
                                    ligne=-1;
                                EraseZone(15,5,NB_CARAC_LIG_MAX+3,NB_LIG_PAGE_MAX);
                                color(15,0);
                                AffichePage(nomDuFich,page,2,ligne,mot,1);
                                getch();
                                i++;
                            }
                            gotoxy(posXMenu+4,posYMenu+18);
                            color(0,12);
                            printf("Fin de la recherche ");color(15,0);
                            if(pos<0 && i==2)
                            {
                                gotoxy(posXMenu+5,posYMenu+18);
                                color(0,12);
                                printf("Aucune occurence de ");color(15,12);printf("%s",mot);color(0,12);printf(" dans le texte.");color(15,0);

                            }


                        }
                        getch();
                        EraseZone(posXMenu+2,posYMenu+14,256,10);
                    break;
                case 5:
                        color(15,0);
                        getNomFich(nomDuFich);
                        getch();
                        gotoxy(posXMenu+2,posYMenu+15);
                        printf("Entrez le mot a remplacer");
                        scanAlphaNum(mot,posXMenu+2,posYMenu+16,256);
                        gotoxy(posXMenu+2,posYMenu+17);
                        printf("Entrez nouveau mot");
                        scanAlphaNum(motRemp,posXMenu+2,posYMenu+18,256);
                        if(mot)
                        {
                            RemplacerTout(nomDuFich,mot,motRemp,1);
                        }
                        EraseZone(posXMenu+2,posYMenu+14,256,10);
                    break;
                case 6:
                        color(15,0);
                        getNomFich(nomDuFich);
                        gotoxy(posXMenu+2,posYMenu+15);
                        printf("Entrez le mot a remplacer");
                        scanAlphaNum(mot,posXMenu+2,posYMenu+16,256);
                        gotoxy(posXMenu+2,posYMenu+17);
                        printf("Entrez nouveau mot");
                        scanAlphaNum(motRemp,posXMenu+2,posYMenu+18,256);
                        //REMPLACER APPROX
                        RemplacerOnebyOne(nomDuFich,mot,motRemp,1);
                        EraseZone(posXMenu+2,posYMenu+14,256,10);
                    break;

                case nombreMenu-1:
                    *quit=1;
                    system("cls");
                    break;
            }
        }
        color(15,0);
}

int Menu()
{
    //system("color F0");

    static int quit =0;
    static int select='\0';
    static char mov='\0';
    static int sousMenu=0;
    static int selSous=0;
    static int justOpenedSousMenu=0;

    DispMenu(&select,&mov,&sousMenu,&selSous,&justOpenedSousMenu);
    HandMenu(&select,&selSous,&sousMenu,&mov,&justOpenedSousMenu,&quit);
    color(15,0);
    if(select==0 && mov==13)
        return -1;
    if(mov == 27)
        return 0;
    else
        return 1;

}

/** PARTIE BOOK */
int nbPos(int n)
{
    int nb=0;
    while(n!=0)
    {
        n=n/10;
        nb=nb+1;
    }
    return nb;
}

double puiss(double n,int p)
{
    int i=1;
    double puissRes=1;
    for(i;i<=p;i++)
    {
        puissRes=puissRes*n;
    }
    return puissRes;
}
