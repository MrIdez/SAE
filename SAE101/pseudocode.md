# Pseudo Code - Guillaume BARON

## Description

C'est l'algorithme principale de mon Puissance 4 écris en pseudo-code

## Algorithme
```text
programme Puissance4 c'est
    type nomJ = chaine[20]; // type pour le nom des joueurs
    constante caractères JETONJ1 := 'o'; // jetons du joueur 1
    constante caractères JETONJ2 := 'x'; // jetons du joueur 2
    constante entier NBCOLONNE := 7;
    constante entier NBLIGNE := 6;
    type tabGrille = tableau[1..NBLIGNE][1..NBCOLONNE] de entier;
    type tabLigne = tableau[1..NBCOLONNE] de entier;
    procédure initGrille(sortF grille:tabGrille)
    procédure afficheLigne(entF ligne:tabLigne);
    procédure afficheTour(entF grille:tabGrille,entF numTour:entier);
    procédure nettoyerEcran();
    procédure debutPartie(sortF nomJ1:nomJ sortF nomJ2:nomJ);
    procédure finPartie(entF grille:tabGrille,entF numGagnant:entier)
    procédure coupJoueur(entF numCoup:entier,entF/sortF grille:tabGrille);
    procédure gagnant(entF grille:tabGrille,sortF terminée, sortF numGagnant);
    
debut
    Grille : tabGrille;
    nJoueur1,nJoueur2 : nomj;
    numGagnant,nbtour : entier ;
    terminée : booléen ; // équivaut a la valeur de vérité de la proposition : " La partie est-elle terminée ?"
    terminée := Faux;
    nbtour := 1;
    initGrille(entE )
    debutPartie(sortE nJoueur1,sortE nJoueur2);
    tantque (!terminée) faire
        afficheTour(entE Grille,entE nbtour);
        coupJoueur(entE nbtour,entE/sortE Grille);
        gagnant(entE Grille,sortE terminée,sortE numGagnant);//Permet de déterminé si le joueur qui à joué a gagné
    finfaire
    finPartie(entE Grille,entE numGagnant);
fin
```
