/**
 * @file puissance4.c
 * @author Guillaume BARON (guillaume.baron.1@etudiant.univ-rennes1.fr)
 * @brief Puissance 4 dans un terminal
 * @version 1.0
 * @date 07-11-2022
 *
 * Ce programme permet à deux joueur de jouer l'un contre l'autre au puissance4 dans un terminal .
 *
 */

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

/***************************************************/
/*                                                 */
/* Définition des constantes et variables globales */
/*                                                 */
/***************************************************/

/**
 * @brief Constante caractère représentant le pion du joueurA
 *
 */
const char PION_A = 'o';

/**
 * @brief Constante caractère représentant le pion du joueurB
 *
 */
const char PION_B = 'x';

/**
 * @brief Constante caractère qui équivaut a la touche qu'il faut entrer
 * pour un deplacement à gauche
 */
const char DEPLACEMENT_GAUCHE = 'q';

/**
 * @brief Constante caractère qui équivaut a la touche qu'il faut entrer
 * pour un deplacement à gauche
 */
const char DEPLACEMENT_DROITE = 'd';

/**
 * @brief Constante caractère représentant le caractére à afficher
 * quand la case est vide.
 *
 */
const char VIDE = '_';

/**
 * @brief Constante caractère représentant un joueur inconnu.
 *
 */
const char INCONNU = ' ';

/**
 * @def NBLIG
 * @brief definie le nombre de ligne de la grille
 *
 */
#define NBLIG 6

/**
 * @def NBCOL
 * @brief definie le nombre de colonne de la grille
 *
 */
#define NBCOL 7

/**
 * @def COLONNE_DEBUT
 * @brief definie la colonne où afficher le pion au debut
 * comme celle du millieu
 *
 */
#define COLONNE_DEBUT NBCOL / 2

/**
 * @def NETTOYER_ECRAN
 * @brief defini l'intruction permettant de nettoyer l'écran
 *
 */
#define NETTOYER_ECRAN printf("\e[1;1H\e[2J")

/**
 * @def RETOURLIGNE
 * @brief definie le printf du retour chariot ('\n')
 *
 */
#define RETOURLIGNE printf("\n")

/**
 * @def NETTOYAGE_BUFFER
 * @brief Defini la suite d'instuction permettant de
 * nettoyer le buffer du STDIN
 */
#define NETTOYAGE_BUFFER while ((getchar()) != '\n')

/*************************/
/*                       */
/* Déclaration des types */
/*                       */
/*************************/

/**
 * @typedef GRILLE
 * @brief Définition le type de la grille
 */
typedef char GRILLE[NBLIG][NBCOL];

/**
 * @typedef NOMJ
 * @brief type tableau de 20 caractéres
 *
 * Permet de representer le type d'un nom joueur.
 */
typedef char NOMJ[20];

/*
Declaration de noms, nb coups et entreeNomJ en variable global
 afin de ne pas les passé inutilement en paramétre
 */

/* Declaration de variable global des noms des joueurs */

/**
 * @brief Declaration de la variable du nom du joueur 1
 */
NOMJ NOMJ1;
/**
 * @brief Declaration de la variable du nom du joueur 2
 */
NOMJ NOMJ2;

/**
 * @brief Declaration de la variable du nombre de coup
 */
int NBCOUPS = 1;

/**
 * @brief Variable global influançant la saisie du nom des joueur.
 * Vrai, si la saisie des noms de joueur doit etre afficher, faux sinon
 */
bool entreeNomJ = true;

/****************************************/
/*                                      */
/* Prototype des fonctions et procédure */
/*                                      */
/****************************************/

bool GrillePleine(GRILLE);

bool colonneGagnante(GRILLE, int, int);

bool ligneGagnante(GRILLE, int, int);

bool diagoGagnanteDroite(GRILLE, int, int);

bool diagoGagnanteGauche(GRILLE, int, int);

bool diagoGagnante(GRILLE, int, int);

bool estVainqueur(GRILLE, int, int);

int choisirColonne(GRILLE, char, int);

int trouverLigne(GRILLE, int);

void printPion(char);

void debutPartie(NOMJ, NOMJ);

void finDePartie(GRILLE g, char);

void initGrille(GRILLE);

void afficher(GRILLE, char, int);

void jouer(GRILLE, char, int *, int *);

/*******************/
/*                 */
/* Fonction main() */
/*                 */
/*******************/

int main()
{
    char vainqueur;
    int ligne, colonne;
    GRILLE g;
    if (entreeNomJ)
    {
        initGrille(g);
    }
    vainqueur = INCONNU;
    debutPartie(NOMJ1, NOMJ2);
    afficher(g, PION_A, COLONNE_DEBUT);
    while ((vainqueur == INCONNU) && !(GrillePleine(g)))
    {
        jouer(g, PION_A, &ligne, &colonne);
        afficher(g, PION_B, COLONNE_DEBUT);
        if (estVainqueur(g, ligne, colonne))
        {
            vainqueur = PION_A;
        }
        else if (!GrillePleine(g))
        {
            jouer(g, PION_B, &ligne, &colonne);
            afficher(g, PION_A, COLONNE_DEBUT);
            if (estVainqueur(g, ligne, colonne))
            {
                vainqueur = PION_B;
            }
        }
    }
    finDePartie(g, vainqueur);
    return EXIT_SUCCESS;
}

/*****************************/
/*                           */
/* Définition des fonctions  */
/*                           */
/*****************************/

/**
 *
 * @brief Teste si toutes les cases de la grille sont occupées ou non.
 *
 * @param g Grille, représente la grille de jeu
 * @return vrai si la grille est pleine
 * @return faux si la grille n'est pas pleine
 *
 * Consiste à parcourir le premier élément de chaque colonne, si un est vide :
 * la grille n'est pas pleine, sinon elle l'est.
 */
bool GrillePleine(GRILLE g)
{
    bool PLEINE = true;
    int j;
    for (j = 0; j < NBCOL; j++)
    {
        if (g[0][j] == VIDE)
        {
            PLEINE = false;
        }
    }
    return PLEINE;
}

/**
 *
 * @brief Test si le pion est au sommet d'une colonne
 *
 * @param g Grille, représente la grille de jeu
 * @param ligne l'indice de la ligne du pion
 * @param colonne l'indice de la colonne du pion
 * @return true le pion complete une colonne gagnante
 * @return false le pion ne complete pas une colonne gagnante
 */
bool colonneGagnante(GRILLE g, int ligne, int colonne)
{
    char pion;
    pion = g[ligne][colonne];
    int nbpion = 0;
    int indice;
    indice = ligne;
    while ((g[indice][colonne] == pion) && (indice + 1 <= NBLIG))
    {
        nbpion++;
        indice++;
    }
    return (nbpion >= 4);
}

/**
 * @brief Test si le pion fait partie d'une ligne gagnante
 *
 * @param g Grille, représente la grille de jeu
 * @param ligne l'indice de la ligne du pion
 * @param colonne l'indice de la colonne du pion
 * @return true
 * @return false
 *
 * Consiste à savoir si le pion dans g[ligne][colonne] fait
 * partie d'une ligne gagnante.
 *
 */
bool ligneGagnante(GRILLE g, int ligne, int colonne)
{
    char pion;
    pion = g[ligne][colonne];
    int nbpion = 0;
    int indice;
    indice = colonne;
    while ((g[ligne][indice] == pion) && (indice - 1 >= 0))
    {
        nbpion++;
        indice--;
    }
    if ((nbpion < 4) && (indice + 1))
    {

        while ((g[ligne][indice] == pion) && (indice + 1 <= NBCOL))
        {
            nbpion++;
            indice++;
        }
    }
    return (nbpion >= 4);
}

/**
 * @brief Test si le pion fait partie d'une diagonale de droite à gauche
 *
 * @param g Grille, représente la grille de jeu
 * @param ligne l'indice de la ligne du pion
 * @param colonne l'indice de la colonne du pion
 * @return vrai si la diagonale est gagante
 * @return faux si la diagonale n'est pas gagnante
 *
 * Consiste à savoir si le pion dans g[ligne][colonne] fait
 * partie d'une ligne gagnante.
 *
 */
bool diagoGagnanteDroite(GRILLE g, int ligne, int colonne)
{
    char pion;
    pion = g[ligne][colonne];
    int nbpion = 1;
    int iCol, iLigne;
    iCol = colonne;
    iLigne = ligne;
    /* Recherche de pion dans la diagonal supérieure gauche */
    if (((iLigne - 1) >= 0) && ((iCol - 1) >= 0))
    {
        iLigne--;
        iCol--;
        while ((g[iLigne][iCol] == pion) &&
               (((iLigne - 1) >= 0) && ((iCol - 1) >= 0)))
        {
            nbpion++;
            iLigne--;
            iCol--;
        }
    }
    iCol = colonne;
    iLigne = ligne;
    /* Recherche de pion dans la diagonal inférieur droit*/
    if (((iLigne + 1) <= NBLIG) &&
        ((iCol + 1) <= NBCOL) && (!(nbpion >= 4)))
    {
        iLigne++;
        iCol++;
        while ((g[iLigne][iCol] == pion) &&
               (((iLigne + 1) <= NBLIG) && ((iCol + 1) <= NBCOL)) && (!(nbpion >= 4)))
        {
            nbpion++;
            iLigne++;
            iCol++;
        }
    }
    return (nbpion >= 4);
}

/**
 * @fn diagoGagnanteGauche(GRILLE g, int ligne, int colonne)
 *
 * @brief Test si le pion fait partie d'une diagonale de gauche à droite
 *
 * @param g Grille, représente la grille de jeu
 * @param ligne l'indice de la ligne du pion
 * @param colonne l'indice de la colonne du pion
 * @return vrai si la diagonale est gagnante
 * @return faux si la diagonale n'est pas gagnante
 *
 * Consiste à compter les pions dans la diagnoanle supérieure droite
 * Puis dans la diagnonale supérieur gauche et enfin a renvoyer si
 * le nombre de pion sur cette diagonal est supérieur à 4.
 *
 */
bool diagoGagnanteGauche(GRILLE g, int ligne, int colonne)
{
    char pion;
    pion = g[ligne][colonne];
    int nbpion = 1;
    int iCol, iLigne;
    iCol = colonne;
    iLigne = ligne;
    if (((iLigne - 1) >= 0) && ((iCol + 1) <= NBCOL))
    {
        iLigne--;
        iCol++;
        while ((g[iLigne][iCol] == pion) &&
               (((iLigne - 1) > 0) && (iCol + 1) <= NBCOL))
        {
            nbpion++;
            iLigne--;
            iCol++;
        }
    }
    iCol = colonne;
    iLigne = ligne;
    if (((iLigne + 1) <= NBLIG) && ((iCol - 1) > 0) && (!(nbpion >= 4)))
    {
        iLigne++;
        iCol--;
        while ((g[iLigne][iCol] == pion) &&
               (((iLigne + 1) <= NBLIG) && ((iCol - 1) >= 0)) &&
               (!(nbpion >= 4)))
        {
            nbpion++;
            iLigne++;
            iCol--;
        }
    }
    return (nbpion >= 4);
}

/**
 * @fn diagoGagnante(GRILLE g, int ligne, int colonne)
 * @brief permet de mutualiser le résultat de diagoGagnanteDroite()
 * et diagoGagnanteGauche()
 * @param g Grille, représente la grille de jeu
 * @param ligne l'indice de la ligne du pion
 * @param colonne l'indice de la colonne du pion
 * @return vrai, si au moins une des diagonale est gagnante
 * @return faux, si les diagonales ne sont pas gagnante
 *
 * Consiste à appeller diagoGagnanteDroite puis diagoGagnanteGauche
 * est de mutualiser le résultat de ces deux fonctions.
 *
 */
bool diagoGagnante(GRILLE g, int ligne, int colonne)
{
    bool trouve = false;
    trouve = diagoGagnanteDroite(g, ligne, colonne);
    if (!trouve)
    {
        trouve = diagoGagnanteGauche(g, ligne, colonne);
    }
    return trouve;
}

/**
 * @fn estVainqueur(GRILLE g, int ligne, int colonne)
 * @brief Indique si le pion situé dans la case repérée par les paramètres ligne et
 * colonne a gagné la partie,
 * c’est-à-dire s’il y a une ligne, une colonne ou une diagonale formée d’au moins 4 de ses pions (la
 * ligne et la colonne passées en paramètres correspondent à la case où le joueur vient de jouer,
 * c’est-à-dire la case à partir de laquelle il faut rechercher 4 pions successifs identiques).
 *
 * @param g Grille, représente la grille de jeu
 * @param ligne entiers représentant la ligne de case à partir de laquelle rechercher une série de
4 successifs identiques
 * @param colonne entiers représentant la colonne de case à partir de laquelle rechercher une série de
4 successifs identiques
 * @return true si il existe une serie de 4 successifs identiques
 * @return false sinon
 *
 * Fait appel a colonneGagnante() puis ligneGagnante() et enfin diagoGagnante() pour rechercher une
 * serie gagnante de pion
 */
bool estVainqueur(GRILLE g, int ligne, int colonne)
{
    bool estVainqueur = false;
    estVainqueur = colonneGagnante(g, ligne, colonne);
    if (!estVainqueur)
    {
        estVainqueur = ligneGagnante(g, ligne, colonne);
    }
    if (!estVainqueur)
    {
        estVainqueur = diagoGagnante(g, ligne, colonne);
    }
    return estVainqueur;
}

/**
 * @fn choisirColonne(GRILLE g, char pion, int colonne)
 * @brief Permet au joueur de choisir la colonne où jouer
 *
 * @param g Grille, représente la grille de jeu
 * @param pion caractère, représente le pion à tester
 * @param colonne colonne de départ (celle au-dessus de laquelle se trouve le pion initialement)
 * @return int indice de la colonne choisie par le joueur
 *
 * Un joueur voit son pion au-dessus de la grille et
 * cette fonction doit lui permettre de "déplacer"
 * son pion d’une colonne vers la gauche (par la touche ‘q’) ou d’une colonne vers la droite (par la
 * touche ‘d’). Après chaque déplacement, la grille est réaffichée. Le joueur peut finalement
 * choisir la colonne où il souhaite faire tomber son pion (par la touche ESPACE).
 */
int choisirColonne(GRILLE g, char pion, int colonne)
{
    char entreeJ;
    int indice;
    indice = colonne;
    scanf("%c", &entreeJ);
    while (entreeJ != ' ')
    {
        if ((entreeJ == DEPLACEMENT_GAUCHE) && (indice > 0))
        {
            indice--;
        }
        else if ((entreeJ == DEPLACEMENT_DROITE) && (indice < NBCOL - 1))
        {
            indice++;
        }
        afficher(g, pion, indice);
        scanf("%c", &entreeJ);
    }
    NETTOYAGE_BUFFER;
    return indice;
}

/**
 * @fn trouverLigne(GRILLE g, int colonne)
 * @brief Permet de trouver la première case non occupée de la colonne.
 *
 * @param g Grille, représente la grille de jeu
 * @param colonne entier, indice de la colonne dans laquelle le pion doit tomber
 * @return int indice de la ligne où le pion devra être ajouté ou -1 si la colonne est pleine
 *
 * Parcour la colonne de haut en bas jusqu'à trouver un pion ou le fond
 * Si la colonne est pleine, la fonction retourne -1.
 */
int trouverLigne(GRILLE g, int colonne)
{
    int i;
    char elem;
    i = 0;
    elem = g[i][colonne];
    while (elem == VIDE)
    {
        i++;
        elem = g[i][colonne];
    }
    return i - 1;
}

/*****************************/
/*                           */
/* Définition des Procédures */
/*                           */
/*****************************/

/**
 * @fn printPion(char pion)
 * @brief Permet d'imprimer le pion d'une certaine couleur,
 * rouge pour 'o' et bleu pour 'x' et blanc pour "VIDE"
 *
 * @param Pion le pion à afficher (peut aussi être VIDE )
 */
void printPion(char pion)
{
    printf("[");
    if (pion == PION_B)
    {
        printf("\033[1;31m");
        printf("%c", pion);
        printf("\033[0m");
    }

    else if (pion == PION_A)
    {
        printf("\033[0;34m");
        printf("%c", pion);
        printf("\033[0m");
    }
    else
    {
        printf("%c", pion);
    }
    printf("]");
}

/**
 * @fn debutPartie(NOMJ NOMJ1, NOMJ NOMJ2)
 * @brief Cette procédure affiche l’écran de début de partie et
 * transmet les entrées de l’utilisateur via ses paramètres de sorties
 *
 * @param NOMJ1 NOMJ, permet de transmettre le nom du joueur 1
 * au programme principal.
 * @param NOMJ2 NOMJ, permet de transmettre le nom du joueur 2
 * au programme principal.
 *
 * Produit l'affichage du debut de partie comme spécifié sur ma Maquette
 */
void debutPartie(NOMJ NOMJ1, NOMJ NOMJ2)
{
    NETTOYER_ECRAN;
    printf("Rentrez votre nom joueur 1 (20 carac MAX, sans ESPACE) :");
    scanf("%s", NOMJ1);
    printf("Rentrez votre nom joueur 2 (20 carac MAX, sans ESPACE) :");
    scanf("%s", NOMJ2);
    NETTOYAGE_BUFFER;
}

/**
 * @fn finDePartie(GRILLE g, char pion)
 * @brief Affiche le résultat d’une partie lorsqu’elle est terminée.
 *
 * @param pion caractère qui représente le pion gagnant (PION_A ou PION_B) ou
 * bien VIDE si match nul
 *
 * Produit l'affichage de fin de partie comme spécifié sur ma Maquette
 * Permet également au joueurs de rejouer.
 */
void finDePartie(GRILLE g, char pion)
{
    int i, j;
    char crarac;
    /* Affichage de la grille*/
    NETTOYER_ECRAN;
    RETOURLIGNE;
    for (i = 0; i < NBLIG; i++)
    {
        for (j = 0; j < NBCOL; j++)
        {
            printPion(g[i][j]);
        }
        RETOURLIGNE;
    }
    RETOURLIGNE;
    if (pion == PION_A)
    {
        printf("Bravo %s tu as gagné !!!", NOMJ1);
    }
    else if (pion == PION_B)
    {
        printf("Bravo %s tu as gagné !!!", NOMJ2);
    }
    else
    {
        printf("Egalité !!");
    }
    RETOURLIGNE;
    RETOURLIGNE;
    printf("Appuyer sur Espace puis entrée pour rejouer avec les mêmes noms, \n");
    printf("entrée pour une autre partie avec des noms différents ou \n");
    printf("sur une autre touche puis entrée pour quitter.");
    RETOURLIGNE;
    scanf("%c", &crarac);
    scanf("%c", &crarac);
    if (crarac == ' ')
    {
        main();
    }
    else if (crarac == '\n')
    {
        entreeNomJ = false;
        main();
    }
}

/**
 * @fn initGrille(GRILLE g)
 * @brief Initialise la grille en affectant la constante VIDE à chacun de ses éléments.
 *
 * @param g Grille, représente la grille de jeu
 */
void initGrille(GRILLE g)
{
    int i, j;
    for (i = 0; i < NBLIG; i++)
    {
        for (j = 0; j < NBCOL; j++)
        {
            g[i][j] = VIDE;
            printf("%c", g[i][j]);
        }
    }
}

/**
 * @fn afficher(GRILLE g, char pion, int colonne)
 * @brief Produit l'affichage de chaque tour comme spécifié dans sur Maquette
 *
 * @param g Grille, représente la grille de jeu
 * @param pion caractère, représente le pion à afficher au-dessus de la grille
 * @param colonne entier, représente l’indice de la colonne au-dessus de laquelle le pion doit être
 * affiché
 *
 *
 * Réalise l’affichage à l’écran du contenu de la grille avec les pions déjà joués.
 * Cette procédure affiche aussi, au-dessus de la grille, le prochain pion à tomber :
 * il sera affiché au-dessus de la colonne dont le numéro est donné en paramètre.
 */
void afficher(GRILLE g, char pion, int colonne)
{
    int numcol, i, j;
    NETTOYER_ECRAN; // instruction permettant d'effacer la console
    printf("Tour %d \n", NBCOUPS);
    RETOURLIGNE;
    printf("%s : %c \n", NOMJ1, PION_A);
    printf("%s : %c \n", NOMJ2, PION_B);
    RETOURLIGNE;
    /* Affichage de l'en tete au dessus la grille*/
    for (numcol = 0; numcol < NBCOL; numcol++)
    {
        printf("[%d]", numcol);
    }
    RETOURLIGNE;
    for (numcol = 0; numcol < colonne; numcol++)
    {
        printf("[%c]", VIDE);
    }
    printPion(pion);
    for (numcol = colonne + 1; numcol < NBCOL; numcol++)
    {
        printf("[%c]", VIDE);
    }
    RETOURLIGNE;
    RETOURLIGNE;
    /* Affichage de la grille*/
    for (i = 0; i < NBLIG; i++)
    {
        for (j = 0; j < NBCOL; j++)
        {
            printPion(g[i][j]);
        }
        RETOURLIGNE;
    }
    RETOURLIGNE;
    if (pion == PION_A)
    {
        printf("C'est au tour de %s \n", NOMJ1);
    }
    else
    {
        printf("C'est au tour de %s \n", NOMJ2);
    }
    RETOURLIGNE;
    printf("Entrez un '%c' pour deplacer votre pion à gauche \n", DEPLACEMENT_GAUCHE);
    printf("Entrez un '%c' pour deplacer votre pion à droite \n", DEPLACEMENT_DROITE);
    printf("Entrez un espace pour faire tomber votre pion \n");
    printf("Terminez votre saise en appuyant sur la touche entrée \n");
    printf("> ");
}

/**
 * @fn jouer(GRILLE g, char pion, int *ligne, int *colonne)
 * @brief permet à un joueur de jouer son pion. La procédure fait appel à choisirColonne,
 * afin que le joueur indique la colonne dans laquelle il veut jouer ;
 * puis fait appel à trouverLigne pour définir la case où ajouter le pion.
 *
 * @param g Grille, représente la grille de jeu
 * @param pion caractère, correspond au pion à jouer
 * @param ligne entier, correspond à la ligne où est tombé le pion
 * @param colonne entier, correspond à la colonne où est tombé le pion
 */
void jouer(GRILLE g, char pion, int *ligne, int *colonne)
{
    int col, li;
    li = -1;
    do
    {
        col = choisirColonne(g, pion, COLONNE_DEBUT);
        li = trouverLigne(g, col);
    } while (li == -1);
    g[li][col] = pion;
    *colonne = col;
    *ligne = li;
    NBCOUPS++;
}