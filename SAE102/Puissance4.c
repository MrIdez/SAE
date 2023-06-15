#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// les constantes
#define NB_LIGNES 6
#define NB_COLONNES 7
#define PION_A 'X'
#define PION_B 'O'
#define VIDE ' '
#define INCONNU ' '

// les types
typedef int Grille[NB_LIGNES][NB_COLONNES];

// Liste noire
typedef bool LNOIRE[NB_COLONNES];

LNOIRE lnoire;

// prototypes des fonctions
void initGrille(Grille laGrille);
void afficher(Grille laGrille, char pion);
bool grillePleine(Grille laGrille);
void jouer(Grille laGrille, char pion, int *ligne, int *colonne);
void jouerIA(Grille laGrille, char pion, int *ligne, int *colonne);
void bloquerJ(Grille laGrille, int *colonne, char pion);
void chercherListeNoire(Grille laGrille, char pion);
void chercherPionG(Grille laGrille, int *colonne, char pion);
int choisirColonne(Grille laGrille, char pion);
int choisirColonneIA(Grille laGrille, char pion);
int chercherLigne(Grille laGrille, int col);
bool estVainqueur(Grille laGrille, int lig, int col);
int tailleAlignement(Grille laGrille, int lig, int col);
int tailleDiag(Grille laGrille, int lig, int col);
void finDePartie(char vainqueur);

// Programme principal. C'est le pion A qui commence a jouer
int main()
{
    Grille laGrille;
    char vainqueur = INCONNU;
    int ligne, colonne;
    initGrille(laGrille);
    afficher(laGrille, PION_A);
    while (vainqueur == INCONNU && !grillePleine(laGrille))
    {
        jouer(laGrille, PION_A, &ligne, &colonne);
        afficher(laGrille, PION_B);
        if (estVainqueur(laGrille, ligne, colonne))
        {
            vainqueur = PION_A;
        }
        else if (!grillePleine(laGrille))
        {
            jouerIA(laGrille, PION_B, &ligne, &colonne);
            afficher(laGrille, PION_A);
            if (estVainqueur(laGrille, ligne, colonne))
            {
                vainqueur = PION_B;
            }
        }
    }
    finDePartie(vainqueur);
    system(EXIT_SUCCESS);
}

void initGrille(Grille laGrille)
{
    int l, c;
    for (l = 0; l < NB_LIGNES; l++)
    {
        for (c = 0; c < NB_COLONNES; c++)
        {
            laGrille[l][c] = VIDE;
        }
    }
}

void afficher(Grille laGrille, char pion)
{
    int l, c;
    system("clear");
    printf("\t");
    printf("  %c\n", pion);
    printf("\t");
    for (c = 0; c < NB_COLONNES; c++)
    {
        printf("----");
    }
    printf("-\n");
    for (l = 0; l < NB_LIGNES; l++)
    {
        printf("\t");
        for (c = 0; c < NB_COLONNES; c++)
        {
            printf("| %c ", laGrille[l][c]);
        }
        printf("|\n");
        printf("\t");
        for (c = 0; c < NB_COLONNES; c++)
        {
            printf("----");
        }
        printf("-\n");
    }
    printf("\t");
    for (c = 0; c < NB_COLONNES; c++)
    {
        printf("  %d ", c);
    }
    printf("\n\n");
}

bool grillePleine(Grille laGrille)
{
    bool pleine = true;
    int c = 0;
    while (pleine && c < NB_COLONNES)
    {
        if (laGrille[0][c] == VIDE)
        {
            pleine = false;
        }
        c++;
    }
    return pleine;
}

/**
 * @brief C la fonction pour jouer là
 *
 * @param laGrille
 * @param pion
 * @param ligne
 * @param colonne
 */
void jouer(Grille laGrille, char pion, int *ligne, int *colonne)
{
    *ligne = -1;
    do
    {
        *colonne = choisirColonne(laGrille, pion);
        *ligne = chercherLigne(laGrille, *colonne);
    } while (*ligne == -1);
    laGrille[*ligne][*colonne] = pion;
}

/**
 * @brief Jouer le joueur IA
 *
 * @param laGrille
 * @param pion
 * @param ligne
 * @param colonne
 */
void jouerIA(Grille laGrille, char pion, int *ligne, int *colonne)
{
    chercherListeNoire(laGrille, pion);
    *colonne = choisirColonneIA(laGrille, pion);
    *ligne = chercherLigne(laGrille, *colonne);
    laGrille[*ligne][*colonne] = pion;
}

/**
 * @brief Renvoie le numéro d'une colonne pour bloquer un joueur
 *
 * @param laGrille
 * @param pion
 */
void bloquerJ(Grille laGrille, int *colonne, char pion)
{
    int colonneLoc;
    char pion_adversaire;
    if (pion == PION_A)
    {
        pion_adversaire = PION_B;
    }
    else
    {
        pion_adversaire = PION_A;
    }
    chercherPionG(laGrille, &colonneLoc, pion_adversaire);
    if (colonneLoc == -1)
    {
        for (int i = 0; i < NB_COLONNES; i++)
        {
            int li = chercherLigne(laGrille, i);
            int nb = 0;
            li++;
            if (laGrille[li][i] == pion_adversaire)
            {
                nb = tailleAlignement(laGrille, li, i);
            }
            if (nb == 2)
            {
                if (laGrille[li][i - 1] == VIDE)
                {
                    colonneLoc = i - 1;
                }
                else if (laGrille[li][i + 1] == VIDE)
                {
                    colonneLoc = i + 1;
                }
            }
        }
    }
    *colonne = colonneLoc;
}

/**
 * @brief Petmet de rechercher et de remplire
 *
 * @param lagrille
 * @param pion
 * @param lnoire
 */
void chercherListeNoire(Grille lagrille, char pion)
{
    char pion_adversaire;
    if (pion == PION_A)
    {
        pion_adversaire = PION_B;
    }
    else
    {
        pion_adversaire = PION_A;
    }
    for (int i = 0; i < NB_COLONNES; i++)
    {
        int l = chercherLigne(lagrille, i);
        if (lagrille[l + 1][i] == pion_adversaire)
        {
            lagrille[l-1][i] = pion_adversaire;
            lnoire[i] = (tailleDiag(lagrille, l - 1, i) == 4);
            lagrille[l-1][i] = VIDE;
        }
        else
        {
            lnoire[i] = false;
        }
    }
}

/**
 * @brief Permet de rechercher un pion gagnant
 *
 * @param laGrille
 * @param colonne
 * @param ligne
 * @param pion
 */
void chercherPionG(Grille laGrille, int *colonne, char pion)
{
    bool trouve;
    int ligneLocal;
    int i = 0;
    trouve = false;
    while ((i < NB_COLONNES) && (!trouve))
    {
        ligneLocal = chercherLigne(laGrille, i);
        if (laGrille[ligneLocal + 1][i] == pion)
        {
            *colonne = i;
            int test = (tailleAlignement(laGrille, ligneLocal + 1, i));
            trouve = (test == 3);
        }
        i++;
    }
    if (!trouve)
    {
        *colonne = -1;
    }
}

int choisirColonne(Grille laGrille, char pion)
{
    int col;

    do
    {
        printf("Numero de colonne ? ");
        scanf("%d", &col);
    } while (col < 0 || col > 6);
    return col;
}

int choisirColonneIA(Grille laGrille, char pion)
{
    int colonne;
    char pion_adversaire;
    colonne = -1;
    if (pion == PION_A)
    {
        pion_adversaire = PION_B;
    }
    else
    {
        pion_adversaire = PION_A;
    }
    bloquerJ(laGrille, &colonne, pion);
    if (colonne == -1)
    {
        chercherPionG(laGrille, &colonne, pion);
    }

    if (colonne == -1)
    {
        colonne = 3;
        if (laGrille[0][colonne] != VIDE)
        {
            colonne = 0;
            if (laGrille[0][colonne] != VIDE)
            {
                colonne = 1;
                if (laGrille[0][colonne] != VIDE)
                {
                    colonne = 6;
                    if (laGrille[0][colonne] != VIDE)
                    {
                        colonne = 5;
                        if (laGrille[0][colonne] != VIDE)
                        {
                            colonne = 4;
                            if (laGrille[0][colonne] != VIDE)
                            {
                                colonne = 2;
                            }
                        }
                    }
                }
            }
        }
    }
    if (lnoire[colonne])
    {
        colonne = -1;
    }
    if (colonne == -1)
    {
        for (int i = 0; i < NB_COLONNES; i++)
        {
                int l = chercherLigne(laGrille, i);
                if (laGrille[l - 1][i] == pion_adversaire)
                {
                    colonne = i;
                }
            }
        }
    return colonne;
}

int chercherLigne(Grille laGrille, int col)
{
    int ligne = -1;

    while (ligne < NB_LIGNES - 1 && laGrille[ligne + 1][col] == VIDE)
    {
        ligne++;
    }
    return ligne;
}

bool estVainqueur(Grille laGrille, int lig, int col)
{
    // consiste a regarder si une ligne de 4 pions s'est formÃ©e autour du pion qui vient de tomber en (lig, col)
    char lePion = laGrille[lig][col];
    int cpt, i, j;

    // regarder la verticale, en comptant le nombre de pions au Sud (inutile de regarder au Nord du pion qui vient de tomber)
    i = lig;
    cpt = 0;
    while (i < NB_LIGNES && laGrille[i][col] == lePion)
    {
        cpt++;
        i++;
    }
    if (cpt >= 4)
    {
        return true;
    }
    // regarder l'horizontale, en comptant le nombre de pions Ã  l'Est et Ã  l'Ouest
    j = col;
    cpt = 0;
    // on regarde Ã  l'est
    while (j >= 0 && laGrille[lig][j] == lePion)
    {
        cpt++;
        j--;
    }
    j = col + 1;
    // on regarde a l'ouest
    while (j < NB_COLONNES && laGrille[lig][j] == lePion)
    {
        cpt++;
        j++;
    }
    if (cpt >= 4)
    {
        return true;
    }
    // regarder la diagonale descendante, en comptant le nombre de pions au Nord Ouest et au Sud Est
    i = lig;
    j = col;
    cpt = 0;
    // on regarde au Nord Ouest
    while (j >= 0 && i >= 0 && laGrille[i][j] == lePion)
    {
        cpt++;
        i--;
        j--;
    }
    i = lig + 1;
    j = col + 1;
    // on regarde au Sud Est
    while (i < NB_LIGNES && j < NB_COLONNES && laGrille[i][j] == lePion)
    {
        cpt++;
        i++;
        j++;
    }
    if (cpt >= 4)
    {
        return true;
    }
    // regarder la diagonale descendante, en comptant le nombre de pions au Nord Est et au Sud Ouest
    i = lig;
    j = col;
    cpt = 0;
    // on regarde au Nord Est
    while (j < NB_COLONNES && i >= 0 && laGrille[i][j] == lePion)
    {
        cpt++;
        i--;
        j++;
    }
    i = lig + 1;
    j = col - 1;
    // on regarde au Sud Ouest
    while (i < NB_LIGNES && j >= 0 && laGrille[i][j] == lePion)
    {
        cpt++;
        i++;
        j--;
    }
    if (cpt >= 4)
    {
        return true;
    }
    return false;
}

int tailleAlignement(Grille laGrille, int lig, int col)
{
    // consiste a regarder si une ligne de 4 pions s'est formÃ©e autour du pion qui vient de tomber en (lig, col)
    char lePion = laGrille[lig][col];
    int cpt, cptV, cptH, cptD1, cptD2, i, j;

    // regarder la verticale, en comptant le nombre de pions au Sud (inutile de regarder au Nord du pion qui vient de tomber)
    i = lig;
    cptV = 0;
    while (i < NB_LIGNES && laGrille[i][col] == lePion)
    {
        cptV++;
        i++;
    }
    cpt = cptV;
    // regarder l'horizontale, en comptant le nombre de pions a l'Est et a l'Ouest
    j = col;
    cptH = 0;
    // on regarde Ã  l'est
    while (j >= 0 && laGrille[lig][j] == lePion)
    {
        cptH++;
        j--;
    }
    j = col + 1;
    // on regarde Ã  l'ouest
    while (j < NB_COLONNES && laGrille[lig][j] == lePion)
    {
        cptH++;
        j++;
    }
    if (cptH > cpt)
    {
        cpt = cptH;
    }
    // regarder la diagonale descendante, en comptant le nombre de pions au Nord Ouest et au Sud Est
    i = lig;
    j = col;
    cptD1 = 0;
    // on regarde au Nord Ouest
    while (j >= 0 && i >= 0 && laGrille[i][j] == lePion)
    {
        cptD1++;
        i--;
        j--;
    }
    i = lig + 1;
    j = col + 1;
    // on regarde au Sud Est
    while (i < NB_LIGNES && j < NB_COLONNES && laGrille[i][j] == lePion)
    {
        cptD1++;
        i++;
        j++;
    }
    if (cptD1 > cpt)
    {
        cpt = cptD1;
    }
    // regarder la diagonale descendante, en comptant le nombre de pions au Nord Est et au Sud Ouest
    i = lig;
    j = col;
    cptD2 = 0;
    // on regarde au Nord Est
    while (j < NB_COLONNES && i >= 0 && laGrille[i][j] == lePion)
    {
        cptD2++;
        i--;
        j++;
    }
    i = lig + 1;
    j = col - 1;
    // on regarde au Sud Ouest
    while (i < NB_LIGNES && j >= 0 && laGrille[i][j] == lePion)
    {
        cptD2++;
        i++;
        j--;
    }
    if (cptD2 > cpt)
    {
        cpt = cptD2;
    }
    return cpt;
}

int tailleDiag(Grille laGrille, int lig, int col)
{
    char lePion = laGrille[lig][col];
    int cpt, cptD1, cptD2, i, j;
    // regarder la diagonale descendante, en comptant le nombre de pions au Nord Ouest et au Sud Est
    i = lig;
    j = col;
    cptD1 = 0;
    // on regarde au Nord Ouest
    while (j >= 0 && i >= 0 && laGrille[i][j] == lePion)
    {
        cptD1++;
        i--;
        j--;
    }
    i = lig + 1;
    j = col + 1;
    // on regarde au Sud Est
    while (i < NB_LIGNES && j < NB_COLONNES && laGrille[i][j] == lePion)
    {
        cptD1++;
        i++;
        j++;
    }
    cpt = cptD1;
    // regarder la diagonale descendante, en comptant le nombre de pions au Nord Est et au Sud Ouest
    i = lig;
    j = col;
    cptD2 = 0;
    // on regarde au Nord Est
    while (j < NB_COLONNES && i >= 0 && laGrille[i][j] == lePion)
    {
        cptD2++;
        i--;
        j++;
    }
    i = lig + 1;
    j = col - 1;
    // on regarde au Sud Ouest
    while (i < NB_LIGNES && j >= 0 && laGrille[i][j] == lePion)
    {
        cptD2++;
        i++;
        j--;
    }
    if (cptD2 > cpt)
    {
        cpt = cptD2;
    }
    return cpt;
}

void finDePartie(char vainqueur)
{
    if (vainqueur != INCONNU)
    {
        printf("Joueur %c vainqueur\n", vainqueur);
    }
    else
    {
        printf("MATCH NUL");
    }
}
