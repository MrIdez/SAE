#include <stdio.h>
#include <stdlib.h>

#define LIGNE 6
#define COLONNE 7

typedef int t_grille[LIGNE][COLONNE];

int main(int argc, char **argv);
int choisirColonneIA(t_grille g, char pion);
void chargerGrille(char **argv, t_grille grille);
int tailleAlignement(t_grille g, int lig, int col);
int tailleDiag(t_grille g, int lig, int col);
void bloquerJ(t_grille g, int *colonne, char pion);
void chercherListeNoire(t_grille g, char pion);
void chercherPionG(t_grille g, int *colonne, char pion);

const int VIDE = 0;
const int JOUEUR = 1;
const int ADVERSAIRE = 2;

// Liste noire
typedef int LNOIRE[COLONNE];

LNOIRE lnoire;

int chercherLigne(t_grille laGrille, int col)
{
    int ligne = -1;

    while (ligne < LIGNE - 1 && laGrille[ligne + 1][col] == VIDE)
    {
        ligne++;
    }
    return ligne;
}

int choisirColonneIA(t_grille g, char pion)
{
    int colonne;
    bloquerJ(g, &colonne, pion);
    if (colonne == -1)
    {
        chercherPionG(g, &colonne, pion);
    }

    if (colonne == -1)
    {
        colonne = 3;
        if (g[0][colonne] != VIDE)
        {
            colonne = 0;
            if (g[0][colonne] != VIDE)
            {
                colonne = 1;
                if (g[0][colonne] != VIDE)
                {
                    colonne = 6;
                    if (g[0][colonne] != VIDE)
                    {
                        colonne = 5;
                        if (g[0][colonne] != VIDE)
                        {
                            colonne = 4;
                            if (g[0][colonne] != VIDE)
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
        for (int i = 0; i < COLONNE; i++)
        {
            int l = chercherLigne(g, i);
            if (g[l - 1][i] == ADVERSAIRE)
            {
                colonne = i;
            }
        }
    }
    return colonne;
}

/*
 * VOTRE STRATEGIE
 */
int maStrategie(t_grille grille)
{
    // TODO: Votre strategie de victoire. La seule fonction que vous aurez à modifier.
    // libre à vous d'en créer d'autres pour aérer votre code.
    // La variable grille est un tableau à deux dimensions d'entiers, c'est la grille du tour.
    // Un VIDE représente une case vide, 1 représente vos jetons et 2 représente les jetons adverses.
    // A vous de choisir le meilleur prochain coup !

    // Retourne la meilleure colonne à jouer
    return choisirColonneIA(grille,JOUEUR);
}

void bloquerJ(t_grille g, int *colonne, char pion)
{
    int colonneLoc;
    chercherPionG(g, &colonneLoc, ADVERSAIRE);
    if (colonneLoc == -1)
    {
        for (int i = 0; i < COLONNE; i++)
        {
            int li = chercherLigne(g, i);
            int nb = 0;
            li++;
            if (g[li][i] == ADVERSAIRE)
            {
                nb = tailleAlignement(g, li, i);
            }
            if (nb == 2)
            {
                if (g[li][i - 1] == VIDE)
                {
                    colonneLoc = i - 1;
                }
                else if (g[li][i + 1] == VIDE)
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
 * @param g
 * @param pion
 * @param lnoire
 */
void chercherListeNoire(t_grille g, char pion)
{
    for (int i = 0; i < COLONNE; i++)
    {
        int l = chercherLigne(g, i);
        if (g[l + 1][i] == ADVERSAIRE)
        {
            g[l - 1][i] = ADVERSAIRE;
            lnoire[i] = (tailleDiag(g, l - 1, i) == 4);
            g[l - 1][i] = VIDE;
        }
        else
        {
            lnoire[i] = 0;
        }
    }
}

/**
 * @brief Permet de rechercher un pion gagnant
 *
 * @param g
 * @param colonne
 * @param ligne
 * @param pion
 */
void chercherPionG(t_grille g, int *colonne, char pion)
{
    int trouve;
    int ligneLocal;
    int i = 0;
    trouve = 0;
    while ((i < COLONNE) && (!trouve))
    {
        ligneLocal = chercherLigne(g, i);
        if (g[ligneLocal + 1][i] == pion)
        {
            *colonne = i;
            int test = (tailleAlignement(g, ligneLocal + 1, i));
            trouve = (test == 3);
        }
        i++;
    }
    if (!trouve)
    {
        *colonne = -1;
    }
}

int tailleAlignement(t_grille g, int lig, int col)
{
    // consiste a regarder si une ligne de 4 pions s'est formÃ©e autour du pion qui vient de tomber en (lig, col)
    char lePion = g[lig][col];
    int cpt, cptV, cptH, cptD1, cptD2, i, j;

    // regarder la verticale, en comptant le nombre de pions au Sud (inutile de regarder au Nord du pion qui vient de tomber)
    i = lig;
    cptV = 0;
    while (i < LIGNE && g[i][col] == lePion)
    {
        cptV++;
        i++;
    }
    cpt = cptV;
    // regarder l'horizontale, en comptant le nombre de pions a l'Est et a l'Ouest
    j = col;
    cptH = 0;
    // on regarde Ã  l'est
    while (j >= 0 && g[lig][j] == lePion)
    {
        cptH++;
        j--;
    }
    j = col + 1;
    // on regarde Ã  l'ouest
    while (j < COLONNE && g[lig][j] == lePion)
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
    while (j >= 0 && i >= 0 && g[i][j] == lePion)
    {
        cptD1++;
        i--;
        j--;
    }
    i = lig + 1;
    j = col + 1;
    // on regarde au Sud Est
    while (i < LIGNE && j < COLONNE && g[i][j] == lePion)
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
    while (j < COLONNE && i >= 0 && g[i][j] == lePion)
    {
        cptD2++;
        i--;
        j++;
    }
    i = lig + 1;
    j = col - 1;
    // on regarde au Sud Ouest
    while (i < LIGNE && j >= 0 && g[i][j] == lePion)
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

int tailleDiag(t_grille g, int lig, int col)
{
    char lePion = g[lig][col];
    int cpt, cptD1, cptD2, i, j;
    // regarder la diagonale descendante, en comptant le nombre de pions au Nord Ouest et au Sud Est
    i = lig;
    j = col;
    cptD1 = 0;
    // on regarde au Nord Ouest
    while (j >= 0 && i >= 0 && g[i][j] == lePion)
    {
        cptD1++;
        i--;
        j--;
    }
    i = lig + 1;
    j = col + 1;
    // on regarde au Sud Est
    while (i < LIGNE && j < COLONNE && g[i][j] == lePion)
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
    while (j < COLONNE && i >= 0 && g[i][j] == lePion)
    {
        cptD2++;
        i--;
        j++;
    }
    i = lig + 1;
    j = col - 1;
    // on regarde au Sud Ouest
    while (i < LIGNE && j >= 0 && g[i][j] == lePion)
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

// La fonction principale reçoit la grille du tour et retourne le coup choisi
// Vous n'avez pas à modifier cette fonction
int main(int argc, char **argv)
{
    t_grille grille;

    chargerGrille(argv, grille);

    return maStrategie(grille);
}

// Charge la grille du tour actuel
// Vous n'avez pas à modifier cette fonction
void chargerGrille(char **argv, t_grille grille)
{
    for (int i = 0; i < LIGNE; i++)
        for (int j = 0; j < COLONNE; j++)
            grille[i][j] = atoi(argv[i * COLONNE + j + 1]);
}
