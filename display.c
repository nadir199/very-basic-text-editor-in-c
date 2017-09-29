#include "display.h"

#define NB_SEPAR 21
#define NB_ESP 3
int INDIC_BALISE[6]={0};
int LASTBAL=-1;
char NOM_FICHIER_AMANIP[256]="";
char NOM_INDEX_AMANIP[256]="";
int LASTLINE=-1;
int LENUMLIG=1;

void getNomFich(char nom[])
{
    strcpy(nom,NOM_FICHIER_AMANIP);
}
void getNomIndex(char nom[])
{
    strcpy(nom,NOM_INDEX_AMANIP);
}

void setLastLine(int nb)
{
    LASTLINE=nb;
}
void setNomFich(char *nom)
{
    if(nom)
        strcpy(NOM_FICHIER_AMANIP,nom);
}
void setNomIndex(char *nom)
{
    if(nom)
        strcpy(NOM_INDEX_AMANIP,nom);
}
char separ[NB_SEPAR]={' ','\t','\n','/','\\','+',';',',','!','?','(',')','[',']','@','%','-','_','\'','\"',EOF};
char espac[NB_ESP]={' ','\n','\t'};
int separateur(char c)
{
    int i=0;
    while(i<NB_SEPAR && separ[i]!=c)
    {
        i++;
    }
    return (i<NB_SEPAR);
}
int espacement(char c)
{
    int i=0;
    while(i<NB_ESP && espac[i]!=c)
    {
        i++;
    }
    return (i<NB_SEPAR);
}
void ClearChaine(char *chaine,int longu)
{
    int i=0;
    for(i=0;i<longu;i++)
        chaine[i]='\0';
}
void color(int t,int f)
{
        HANDLE H=GetStdHandle(STD_OUTPUT_HANDLE);
            SetConsoleTextAttribute(H,f*16+t);
}
void gotoxy (int x, int y)
{
    COORD pos;
    pos.X = x;
    pos.Y = y;
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    if (!SetConsoleCursorPosition(hConsole, pos))
    {
        printf("Erreur dans le GOTOXY CURSOR POS SET");
    }
}
void DrawBox(int x,int y,int sizeX,int sizeY)
{
    int i=x;
    gotoxy(x,y);printf("%c",201);
    for(i=x+1;i<x+sizeX;i++)  printf("%c",205);
    printf("%c",187);

    gotoxy(x,y+sizeY);printf("%c",200);
    for(i=x+1;i<x+sizeX;i++)  printf("%c",205);
    printf("%c",188);

    for(i=y+1;i<y+sizeY;i++)
    {
        gotoxy(x,i);
        printf("%c",186);
        gotoxy(x+sizeX,i);
        printf("%c",186);
    }
    gotoxy(x+1,y+1);printf("%c",177);
    gotoxy(x+1,y+sizeY-1);printf("%c",177);
    gotoxy(x+sizeX-1,y+1);printf("%c",177);
    gotoxy(x+sizeX-1,y+sizeY-1);printf("%c",177);
}
void EraseZone(int x,int y,int sizeX,int sizeY)
{
    int i=x,j=y;
    for(j=y;j<y+sizeY;j++)
    {
        i=x;
        gotoxy(i,j);
        for(i=x;i<x+sizeX;i++)
        {
            printf(" ");
        }
    }
}
void suppSpace(char *chaine)
{
    char tmpCh[200]="";
    ClearChaine(tmpCh,200);
    int i=0;
    while((chaine[i]==' '))
    {
        i++;
    }
    int j=0;
    while(i<strlen(chaine))
    {
        tmpCh[j]=chaine[i];
        i++;
        j++;
    }
    tmpCh[j]='\0';
    i=strlen(tmpCh)-1;
    while((tmpCh[i]==' '))
    {
        tmpCh[i]='\0';
        i--;
    }
    strcpy(chaine,tmpCh);
}
/** MOTION FICHIERS */
/// ME PLACE AU DEBUT DE LA LIGNE @line
char * GotoLine(FILE *f,int page,int line)
{
    int numChar=0;
    char * chaine= malloc(200*sizeof(chaine));
    int c=0,ind=0,nbLig=1,nbCar=0,bal=-1;
    ClearChaine(chaine,200);
    if(f)
    {
        rewind(f);
        GotoZePage(f,page);
        while(c!=EOF && nbLig < line)
        {
                bal=fonc_dec_balise(f,&numChar);
                c=fgetc(f);
                if(c!='\n')
                {
                    chaine[ind]=c;
                    ind++;
                }

                nbCar++;
                if(c=='\n' || nbCar>NB_CARAC_LIG_MAX || c==EOF || bal==4 || nbLig>NB_LIG_PAGE_MAX || (bal>=0 && LASTBAL>=0))
                {
                    nbCar=0;
                    nbLig++;
                    arrangeLigne(f,chaine,&numChar);

                    ind=0;

                }

                if(bal==4 || nbLig>NB_LIG_PAGE_MAX)
                {
                    /*gotoxy(15,4+nbLig);
                    printf("%s\n",chaine);*/
                    nbCar=0;
                    nbLig=1;
                    ind=0;

                }
                else if (bal>=0)
                {
                    //INDIC_BALISE[bal]=!INDIC_BALISE[bal];
                }
        }
    }
    return chaine;
}
/** FONCTIONS AFFICHAGE */
int fonc_dec_balise(FILE * f,int * numChar)
{

    int out=-1;
    int c=0;
    LASTBAL=-1;
    c=fgetc(f);
    (*numChar)++;
    if(c=='/')
    {
        c=fgetc(f);
        (*numChar)++;
        switch(c)
        {
            case 'c': out=0;
                break;
            case 'd': out=1;
                break;
            case 'g': out=2;
                break;
            case 'j': out=3;
                break;
            case 'p': out=4;
                break;
        }
        c=fgetc(f);
        (*numChar)++;
        if(c!='/' && c!=EOF)
        {
            out=-1;
            (*numChar)-=3;
            fseek(f,-3,SEEK_CUR);
        }
        else if (c!=EOF && out >=0)
        {
            if(INDIC_BALISE[out])
                LASTBAL=out;
            INDIC_BALISE[out]=!INDIC_BALISE[out];
        }

    }
    else if (c!=EOF)
    {
        fseek(f,-1,SEEK_CUR);
        (*numChar)--;
    }


    return out;
}
int traiter_dep_mort(FILE *f,char *chaine)
{
    int numChar=0; // USELESS
    int c=0;
    int i=0,j=0;
    int lon=strlen(chaine);
    int out=0;
    if (f&& ! espacement(chaine[lon-1]))
    {
        c=fgetc(f);
        while(!separateur(c))
        {
            fseek(f,-2,SEEK_CUR);
            c=fgetc(f);
            i++;
        }
        while(j<i-1)
        {
            chaine[lon-1-j]='\0';
            j++;
        }
        i=0;
        int bal=-1;
        while(espacement(c))
            c=fgetc(f);
        while((!separateur(c) && bal<0) || c==' ')
        {
            c=fgetc(f);
            bal=fonc_dec_balise(f,&numChar);
            i++;
        }
        if(c=='\n' && bal<0)
            out=1;
        else
            out=0;
        fseek(f,-i,SEEK_CUR);


        c=fgetc(f);
        if(!espacement(c))
            fseek(f,-1,SEEK_CUR);
    }
    return out;
}
void GotoZeChar(char *nom_fich, int leChar, int *PAGE, int *LINE )
{
    int ledep=0;
    int numChar = 0;
    FILE *f= fopen(nom_fich,"r");
    int compt_Pages=-1,nbLig=-1;
    if(f)
    {
        int ind=0,nbCar=0,c=0,bal=-1;
        char chaine[200];
        compt_Pages=1;
        nbLig=1;
        ClearChaine(chaine,200);
        while(!feof(f) && numChar<leChar)
        {
                bal=fonc_dec_balise(f,&numChar);
                c=fgetc(f);
                numChar++;
                if(c!='\n')
                {
                    chaine[ind]=c;
                    ind++;
                }

                nbCar++;
                if(c=='\n' || nbCar>NB_CARAC_LIG_MAX)
                {
                    nbCar=0;
                    nbLig++;
                    ledep=arrangeLigne(f,chaine,&numChar);

                    //system("pause");
                    ind=0;
                    c='\0';
                    ClearChaine(chaine,200);
                }

                if(bal==4 || nbLig>=NB_LIG_PAGE_MAX)
                {
                    nbCar=0;
                    nbLig=1;
                    compt_Pages++;
                }
                else if (bal>=0)
                {
                    //INDIC_BALISE[bal]=!INDIC_BALISE[bal];
                }
        }
        fclose(f);
    }
    *PAGE=compt_Pages;
    *LINE = nbLig+1; // RETOURNE LA PAGE OU SE TROUVE LE CHAR
    /*if(ledep)
        (*LINE)++; */
}
void GotoZePage(FILE *f,int page)
{
    int numChar=0; // USELESS
    if(f)
    {
        rewind(f);
        int killDatSlashN=0,ind=0,nbCar=0,nbLig=1,compt_Pages=1,c=0,bal=-1;
        char chaine[200];
        ClearChaine(chaine,200);
        while(!feof(f) && compt_Pages < page)
        {
                bal=fonc_dec_balise(f,&numChar);
                c=fgetc(f);
                if(c!='\n')
                {
                    chaine[ind]=c;
                    ind++;
                }

                nbCar++;
                if(c=='\n' || nbCar>NB_CARAC_LIG_MAX)
                {
                    nbCar=0;
                    nbLig++;
                    killDatSlashN=arrangeLigne(f,chaine,&numChar);
                    ind=0;
                    c='\0';
                    ClearChaine(chaine,200);
                }

                if(bal==4 || nbLig>=NB_LIG_PAGE_MAX)
                {
                    nbCar=0;
                    nbLig=1;
                    compt_Pages++;
                }
                else if (bal>=0)
                {
                    //INDIC_BALISE[bal]=!INDIC_BALISE[bal];
                }
        }

    }
}
int arrangeLigne(FILE *f,char *chaine,int *numChar)
{
    int c=0;
    int i=0,j=0;
    int lon=strlen(chaine);
    int out=0;
    if (f)
    {
        c=fgetc(f);
        (*numChar)++;
        // REVENIR AU PREMIER SEPARATEUR
        if(separateur(c))
        {
            fseek(f,-1,SEEK_CUR);
            (*numChar)--;
        }
        while(!separateur(c) || c=='\'' )
        {
            fseek(f,-2,SEEK_CUR);
            c=fgetc(f);
            (*numChar)--;
            i++;
        }
        // REMPLACER PAR DES \0 CE QUI A DEJA ETE ECRIT
        if(i>0)
            out=1;
        while(j<i-1)
        {
            chaine[lon-1-j]='\0';
            j++;
        }
        i=0;
        int bal=-1;
        //while(espacement(c))
            //c=fgetc(f);
        /*while((!separateur(c) && bal<0)&& c!=EOF)
        {
            bal=fonc_dec_balise(f,numChar);
            c=fgetc(f);
            (*numChar)++;

            i++;
        }
        /*if(c=='\n' && bal<0)
            out=1;
        else
            out=0;
        fseek(f,-i-2,SEEK_CUR);
        (*numChar)-=i+2;


        c=fgetc(f);
        (*numChar)++;
        if(!espacement(c))
        {
            fseek(f,-1,SEEK_CUR);
            (*numChar)--;
        } */

    }
    if(chaine[strlen(chaine)-1]=='/')
        chaine[strlen(chaine)-1]='\0';

    return out;
}
int NbWords(char * nom_fich)
{
    int numChar=0;
    int cpt=0,c=0,bal=-1;
    FILE *f=NULL;
    f=fopen(nom_fich,"r");
    char *word=NULL;
    if(f)
    {
        while(!feof(f))
        {
            word=getNextWord(f,&numChar);

            if(word)
            {
                suppSpace(word);
                if(word[0]!='\0')
                    cpt++;
                free(word);
            }
        }
    }
    fclose(f);
    if(cpt!=0)
        cpt++;
    fclose(f);
    return cpt;
}
int nbPages(char *nom_fichier,int *meslignes)
{
    FILE *f = fopen(nom_fichier,"r");
    int nombreLignes=1;
    int compt_Pages=1;
    if(f)
    {
        rewind(f);
        int killDatSlashN=0,ind=0,nbCar=0,nbLig=1,c=0,bal=-1;
        int numChar=0;
        char chaine[200];
        ClearChaine(chaine,200);
        while(!feof(f))
        {
                bal=fonc_dec_balise(f,&numChar);
                c=fgetc(f);
                if(c!='\n')
                {
                    chaine[ind]=c;
                    ind++;
                }

                nbCar++;
                if(c=='\n' || nbCar>NB_CARAC_LIG_MAX)
                {
                    nbCar=0;
                    nbLig++;
                    nombreLignes++;
                    killDatSlashN=arrangeLigne(f,chaine,&numChar);
                    ind=0;
                    c='\0';
                    ClearChaine(chaine,200);
                }

                if(bal==4 || nbLig>=NB_LIG_PAGE_MAX)
                {
                    nbCar=0;
                    nbLig=1;
                    compt_Pages++;
                }
                else if (bal>=0)
                {
                    //INDIC_BALISE[bal]=!INDIC_BALISE[bal];
                }
        }

    }
    fclose(f);
    *meslignes=nombreLignes;
    return compt_Pages;
}
int nbChar(char *nom_fichier)
{
    int numchar =0;
    char *c=0;
    int MOIJESORS=0;
    FILE *f =fopen(nom_fichier,"r");
    while(!feof(f))
    {
        fonc_dec_balise(f,&numchar);
        c=fgetc(f);
        MOIJESORS++;
    }
    fclose(f);
    return MOIJESORS;
}
int premPosWordChaine(char *chaine,char*mot,int myem,int APPROX)
{
    int i=0,j=0,save=0,yem=0;
    char word[256]={'\0'};
    while( chaine[i]!='\0' && yem<myem)
    {
        save=i;
        while(!separateur(chaine[i]) && chaine[i]!='.' && chaine[i]!='\0')
        {
            word[j]=chaine[i];
            j++;
            i++;

        }

        word[j]='\0';
//        printf(":%s:",word);
        if(CritereRecherche(word,mot,APPROX))
        {
            //printf("lecon : %s",word);
            yem++;
        }
        while(separateur(chaine[i]) || chaine[i]=='.' && chaine[i]!='\0')
        {
            i++;
        }
        j=0;


    }
    if(yem>=myem && save!=0)
        return save-1;
    else if (yem>=myem && save==0)
        return 0;
    else
        return -2;
}
void AffichePage(char *nom_fichier,int page, int MODE , int LINE,char *mot,int APPROX)
{
    color(15,0);
    int numChar=0;
    int rein=0;

    while(rein<6)
    {
        INDIC_BALISE[rein]=0;
        rein++;
    }
    FILE *f=NULL;
    int bal=-1;
    f=fopen(nom_fichier,"r");
    if(f)
    {
        strcpy(NOM_FICHIER_AMANIP,nom_fichier);

        //printf("%d\n",NbWords(f));
        //_system("pause");
        GotoZePage(f,page);
        int CBON=0;
        int killDatSlashN=0,ind=0,nbCar=0,nbLig=1,compt_Pages=1,c=0,bal=-1;
        char chaine[200];
        ClearChaine(chaine,200);
        char *tempChaine=malloc(200/*NB_CARAC_LIG_MAX*2*sizeof(char)*/);
        ClearChaine(tempChaine,200);
        while(c!=EOF && nbLig < NB_LIG_PAGE_MAX)
        {
            color(15,0);
                bal=fonc_dec_balise(f,&numChar);
                c=fgetc(f);
                if(c!='\n')
                {
                    chaine[ind]=c;
                    ind++;
                }

                nbCar++;
                if(c=='\n' || nbCar>NB_CARAC_LIG_MAX || c==EOF || bal==4 || nbLig>NB_LIG_PAGE_MAX || (bal>=0 && LASTBAL>=0))
                {
                    nbCar=0;
                    nbLig++;
                    //printf("AV:%s\n",chaine);
                    killDatSlashN=arrangeLigne(f,chaine,&numChar);
                    gotoxy(15,4+nbLig);
                    /** TRAITEMENT SELON LA BALISE*/
                    int longueur =strlen(chaine);
                   /* int parc=0;
                    while(parc<5)
                    {

                        printf("%d",INDIC_BALISE[parc]);
                        parc++;
                    }*/
                    int i=0;
                    if(INDIC_BALISE[0] ||LASTBAL ==0)
                    {
                        strcpy(tempChaine,chaine);
                        i=0;
                        while(2*i<NB_CARAC_LIG_MAX-longueur)
                        {
                            chaine[i]=' ';
                            i++;
                        }
                        chaine[i]='\0';
                        i=0;
                        strcat(chaine,tempChaine);
                    }
                    else if(INDIC_BALISE[1]||LASTBAL ==1) /** FIXER QUAND BALISE DANS LIGNE PREND LA LIGNE A DROITE FIXER EN METTANT UN SAUT DE LIGNE ET TRAITER SEPAR */
                    {
                        i=0;
                        suppSpace(chaine);
                        longueur=strlen(chaine);
                        strcpy(tempChaine,chaine);
                        while(i<NB_CARAC_LIG_MAX-longueur+1)
                        {
                            chaine[i]=' ';
                            i++;
                        }
                        chaine[i]='\0';

                        strcat(chaine,tempChaine);
                    }
                    else if (INDIC_BALISE[2]||LASTBAL ==2)
                    {
                        suppSpace(chaine);
                    }
                    else if (INDIC_BALISE[3]||LASTBAL ==3 )
                    {
                        // voir apres pr jutifie

                        suppSpace(chaine);
                        if(strlen(chaine)>NB_CARAC_LIG_MAX-20)
                        {
                            int manque = NB_CARAC_LIG_MAX -strlen(chaine)+1;
                            int i=0;
                            int j=0;
                            //printf("LACHAINE : %s",chaine);
                            while(strlen(chaine)<NB_CARAC_LIG_MAX+1)
                            {

                                while(chaine[j]==' ')
                                {
                                    j++;
                                }
                                while(chaine[j]!=' ' && chaine[j]!='\t')
                                {
                                    j++;
                                }
                                //printf("%d",j);
                                int saveJ=j+1;
                                int longueureee=strlen(chaine);
                                int k=longueureee-1;
                                chaine[k+2]='\0';
                                while(k>=saveJ)
                                {

                                    chaine[k+1]=chaine[k];
                                    k--;
                                }
                                if(saveJ!=strlen(chaine))
                                {
                                    chaine[saveJ]=' ';
                                    i++;
                                }
                                else
                                    j=0;

                                if(j>strlen(chaine))
                                    j=0;
                                /*printf("%s",chaine);
                                getch();*/
                                //printf("%s/",chaine);
                            }
                            //printf("LALONG : %d",strlen(chaine));
                        }

                    }
                    else
                    {
                        suppSpace(chaine);
                    }
                    char sauvegarde[300]="";
                    char Part1[256]="";
                    char LEMOT[256]="";
                    char Part2[256]="";
                    int kifkif=0;
                    if (MODE ==2 && nbLig==LINE)
                    {
                        if(LASTLINE==-1)
                        {
                            LASTLINE=LINE;
                            LENUMLIG=1;
                        }

                        else if(LASTLINE==LINE)
                            LENUMLIG++;
                        else
                        {
                            LASTLINE=LINE;
                            LENUMLIG=1;
                        }


                        strcpy(sauvegarde,chaine);
                        color(0,15);

                        char *mots=strtok(sauvegarde," \n./\\+;,!?()[]@%-_'");
                        while(mots && ! kifkif)
                        {
                            //printf("%s",mots);

                            //getch();
                            if(mots!=NULL && CritereRecherche(mots,mot,APPROX))
                            {
                                kifkif=1;
                            }
                            mots = strtok(NULL," \n./\\+;,!?()[]@%-_'");

                        }
                        MODE=-1;
                        if(kifkif==0)
                        {
                            color(15,0);
                            LINE++;
                            MODE=2;
                            CBON=0;
                        }
                        else
                        {
                            int loch=strlen(chaine);
                            int pos=premPosWordChaine(chaine,mot,LENUMLIG,APPROX);
                            int i=0;
                            if(pos>=0)
                            {
                                while(i<=pos)
                                {
                                    Part1[i]=chaine[i];
                                    i++;
                                }
                                Part1[i]='\0';
                                int s=0;
                                while(!separateur(chaine[i]) && chaine[i]!='.')
                                {
                                    LEMOT[s]=chaine[i];
                                    i++;
                                    s++;
                                }
                                LEMOT[s]='\0';

                                int j=0;
                                while(i<loch)
                                {
                                    Part2[j]=chaine[i];
                                    i++;
                                    j++;
                                }
                                Part2[j]='\0';
                                CBON=1;
                            }
                            else
                                CBON=0;


                        }

                    }
                    if(CBON )
                    {
                        color(15,0);
                        printf("%s",Part1);
                        color(0,15);
                        printf("%s",LEMOT);
                        color(15,0);
                        printf("%s",Part2);
                        CBON=0;
                    }
                    else
                        printf("%s\n",chaine);

                    if(MODE ==-1)
                    {
                        color(15,0);
                        MODE=0;
                    }

                    //system("pause");
                    ind=0;
                    ClearChaine(chaine,200);
                }

                if(bal==4 || nbLig>NB_LIG_PAGE_MAX)
                {
                    /*gotoxy(15,4+nbLig);
                    printf("%s\n",chaine);*/
                    nbCar=0;
                    nbLig=1;
                    compt_Pages++;

                }
                else if (bal>=0)
                {
                    //INDIC_BALISE[bal]=!INDIC_BALISE[bal];
                }
        }
        color(0,12);
        gotoxy(POS_X+NB_CARAC_LIG_MAX/2+ 7, POS_Y + NB_LIG_PAGE_MAX +3);
        printf("%d   /    ",page);
        color(15,0);
        free(tempChaine);
        fclose(f);
    }



}
/** FONCTIONS INDEX*/
char * getNextWord(FILE *f,int *numChar)
{
    char *word=malloc(200*sizeof(char));
    ClearChaine(word,200);
    int c=0,i=0;
    fonc_dec_balise(f,numChar);
    while(separateur(c) && c!=EOF)
    {
        c=fgetc(f);
        (*numChar)++;
    }

    while(!separateur(c) && c!='.' && c!=EOF)
    {
        if(c!=0)
        {
            fonc_dec_balise(f,numChar);
            word[i]=c;
            i++;
        }
        c=fgetc(f);
        (*numChar)++;
    }
    word[i]='\0';

    if(c==EOF)
        return NULL;

    return word;

}
int getNextNumbLine(FILE *f)
{
    int n=0;
    char c=0;
    while(!(  (c>='0')&& (c<='9')) && c!=EOF && c!='\n')
    {
        c=fgetc(f);
    }
    while((c>='0')&& (c<='9') && c!=EOF && c!='\n')
    {
        n=n*10+c-'0';
        c=fgetc(f);
    }
    if(c==EOF)
        return -1;
    return n;
}

void init_index(char *nom_fich)
{
    /**D*/
    strcpy(NOM_FICHIER_AMANIP,nom_fich);

    char nomInd[256]="";
    int numChar=0,compt_pages=1,numMot=0;
    int longueur= strlen(nom_fich);
    int POS_SAVE=0;
    char *mot;
    FILE *f=NULL,*ind = NULL;
    /**C*/
    strcpy(nomInd,nom_fich);
    nomInd[longueur-4]='I';
    nomInd[longueur-3]='N';
    nomInd[longueur-2]='D';
    nomInd[longueur-1]='.';
    nomInd[longueur]='t';
    nomInd[longueur+1]='x';
    nomInd[longueur+2]='t';
    nomInd[longueur+3]='\0';
    setNomIndex(nomInd);
    f=fopen(nom_fich,"r");
    ind= fopen(nomInd,"w");
    if(f&& ind)
    {
        while(!feof(f))
        {
            //POS_SAVE=numChar;
            mot=getNextWord(f,&numChar);
            if(mot!=NULL)
                if(strlen(mot)>0)
                {
                    numMot++;
                    fprintf(ind,"%s %d\n",mot,numChar-strlen(mot)-1);
                }
            if(mot!=NULL)
                free(mot);
        }
    }
    fclose(ind);
    fclose(f);

}
int CritereRecherche(char *chaine,char *autre, int approxOUPAS)
{
    if(chaine && autre)
    {
        //printf("\nNONULL");
        if (approxOUPAS)
        {
            //printf("\nAVPROX");
            int tmp=EqAprrox(chaine,autre);
            //printf("\nAPPROX");
            return tmp;
        }
        else
        {
            return !strcmp(chaine,autre);
        }
    }

    else
        return 0;
}
int recherche_index(char *nom_fich,char *mot,int occNumb,int APPROX,int *longMot,char *MotRemp,int *OUTF)
{
    *OUTF=0;
    int numChar=0;
    int n=0;
    char *word=NULL;
    FILE *f=NULL;
    int nb=0;
    f= fopen(nom_fich,"r+");
    int lastPos=0;
    int SAVEWORDLENGTH=-1;
    int positions = -1;

    int outout=0;

    if(f)
        while(!feof(f) && n>=0 && nb<occNumb) //&& positions<0
        {
            word=getNextWord(f,&numChar);
            if(word)
            {

                n=getNextNumbLine(f);
                //printf("\nAV CRIT");
                if(CritereRecherche(word,mot,APPROX))
                    {
                        if(n>=0)
                        {
                            nb++;
                            if(nb==occNumb )
                                if(MotRemp!=NULL)
                                    if(APPROX && CritereRecherche(word,MotRemp,APPROX))
                                    {
                                        outout=1;
                                    }
                        //printf("tours");
                        //printf("DONCRIT");

                            SAVEWORDLENGTH=strlen(word);
                            lastPos=positions;
                            positions = n;

                        }
                    }
            }
            free(word);
        }
    if(outout)
        *OUTF=-2;
    if(nb!= occNumb) //si
        positions=-1;
    fclose(f);
    *longMot= SAVEWORDLENGTH;

    return positions;
}

int RemplacerUnConf(char *nom_fich,char *mot,char *motRemp,int mYemMot,int CONFONOFF,int APPROX)
{
    color(15,0);
    FILE *newF=NULL,*f=NULL;
    int longueur=0;
    int LEOUTF=0;
    int charPos= recherche_index(NOM_INDEX_AMANIP,mot,mYemMot,APPROX,&longueur,motRemp,&LEOUTF);
    if(APPROX==0)
        longueur=strlen(mot);
    int mov=0;
    if(charPos>=0)
    {
        if(CONFONOFF)
        {
            int ligne=0,page=0;
            GotoZeChar(nom_fich,charPos,&page,&ligne);
            if(charPos<0)
                ligne=-1;
            EraseZone(15,5,NB_CARAC_LIG_MAX+3,NB_LIG_PAGE_MAX);
            AffichePage(nom_fich,page,2,ligne,mot,APPROX);

            /*char *leword =getMotCharPos(nom_fich,charPos);
            printf("REPLACE THIS MATCH ? %s",leword);
            free(leword); */

            mov=getch();
            if(ligne == LASTLINE && mov ==13)
                LENUMLIG--;
        }
        else
            mov=13;



        if(mov==13)
        {
            newF=fopen("tmp.txt","w");
            f=fopen(nom_fich,"r");
            rewind(f);
            int i=0,c=0;
            while(i<charPos&& c!=EOF)
            {
                c=fgetc(f);
                fprintf(newF,"%c",c);
                i++;
            }
            i=0;
            while(i<longueur&& ! feof(f))
            {
                c=fgetc(f);
                i++;
            }
            fprintf(newF,"%s",motRemp);
            while(!feof(f))
            {
                c=fgetc(f);
                fprintf(newF,"%c",c);
            }
            fclose(newF);
            fclose(f);
            rename(NOM_FICHIER_AMANIP,"del.txt");
            //system("rename tes.txt del.txt");
            //system("rename tmp.txt tes.txt");
            remove("del.txt");
            rename("tmp.txt",NOM_FICHIER_AMANIP);
            init_index(NOM_FICHIER_AMANIP);
            if(LEOUTF)
                return 2;
            else
                return 1;
        }
        else if (mov !=27)
            return -1;
        else
            return 0;
    }
    else if(LEOUTF == -2)
    {

        return -1;
    }
    else
    {
        //printf("IL ES PETIT");
        return 0;
    }

}
void RemplacerOnebyOne(char *nom_fich,char *mot,char *motRemp,int APPROX)
{
    int still=1;
    int mYem=1;
    if(nom_fich)
    {
        while(still)
        {
            still=RemplacerUnConf(nom_fich,mot,motRemp,mYem,1,APPROX); // APPEL AVEC CONFIRMATION ON
            //printf("STILL : %d",still);
            if(still<0 ||still==2)
                mYem++;
            if(still==2 && mYem>2)
                LENUMLIG++;
            //printf("LEMYEM : %d",mYem);
        }

        EraseZone(15,5,NB_CARAC_LIG_MAX+3,NB_LIG_PAGE_MAX);

    }
    //system("pause");

}

void RemplacerTout(char *nom_fich,char *mot,char *motRemp,int APPROX)
{
    int still=1;
    int myem=1;
    if(nom_fich)
    {
        while(still)
        {
            still=RemplacerUnConf(nom_fich,mot,motRemp,myem,0,APPROX); // APPEL AVEC CONFIRMATION OFF
            if(APPROX)
                myem++;
        }
        EraseZone(15,5,NB_CARAC_LIG_MAX+3,NB_LIG_PAGE_MAX);

    }
}

int DisLevenshtein(char *mot,char *mot2)
{
    int cost=0;
    int l1=strlen(mot)+1;
    int l2=strlen(mot2)+1;
    int **tab=malloc(l1*sizeof(int*));
    int i=0,j=0;
    while(i<l1)
    {
        tab[i]=malloc(l2*sizeof(int));
        i++;
    }
    //printf("\nALLOC");
    for(i=0;i<l1;i++)
    {
        tab[i][0]=i;
    }
    for(j=0;j<l2;j++)
    {
        tab[0][j]=j;
    }
    for(i=1;i<l1;i++)
    {
        for(j=1;j<l2;j++)
        {
            if(mot[i-1]==mot2[j-1])
                cost=0;
            else
                cost=1;
            tab[i][j]=min(tab[i-1][j]+1, min(tab[i][j-1]+1,tab[i-1][j-1]+cost));
        }
    }
    //printf("\nCALCUL");
    int leret=tab[l1-1][l2-1];
    //printf("\FINC");
    i=0;
    while(i<l1)
    {
        if(tab[i])
            free(tab[i]);
        tab[i]=NULL;
        i++;
    }
    if(tab!=NULL)
        free(tab);
    //printf("\nLib");
    return leret;
}
int LettreInvers(char *ch,char *autre)
{
    int resu=0;
    char chaine[256]="";
    strcpy(chaine,ch);
    int i=0, lon=strlen(chaine);
    char tmp=0;
    if (abs(lon-strlen(autre))<=1)
    {
        while(i<lon-1 && !resu)
        {
            tmp=chaine[i];
            chaine[i]=chaine[i+1];
            chaine[i+1]=tmp;
            if(DisLevenshtein(chaine,autre)<2)
            {
                resu=1;
            }
            tmp=chaine[i];
            chaine[i]=chaine[i+1];
            chaine[i+1]=tmp;
            i++;
        }

    }
    return resu;

}
int EqAprrox(char *mot,char *mot2)
{
    if (mot && mot2)
    {
        //printf("\nNONONULL");
        if(DisLevenshtein(mot,mot2)<2 || LettreInvers(mot,mot2))
        {
            //printf("L7A9T");
            return 1;
        }
    }

    return 0;
}


