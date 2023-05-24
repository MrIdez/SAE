SET SCHEMA 'public';

CREATE TABLE _annee (
    an INT,
    CONSTRAINT annee_pk PRIMARY KEY (an)
);

CREATE TABLE _type_musique (
    type_m CHAR,
    CONSTRAINT type_music_pk PRIMARY KEY (type_m)
);

CREATE TABLE _pays(
    nom_p CHAR ,
    CONSTRAINT pays_pk PRIMARY KEY (nom_p)
);

CREATE TABLE _formation (
    libelee_formation CHAR,
    CONSTRAINT formation_pk PRIMARY KEY (libelee_formation)
);

CREATE TABLE _edition (
    nom_edition CHAR,
    annee INT,
    CONSTRAINT _edition_pk PRIMARY KEY (nom_edition),
    CONSTRAINT _edition_fk_annee FOREIGN KEY (annee) REFERENCES _annee(an)
);

CREATE TABLE _ville (
    nom_v CHAR,
    pays CHAR,
    CONSTRAINT _ville_pk PRIMARY KEY (nom_v),
    CONSTRAINT _ville_fk_pays FOREIGN KEY (pays) REFERENCES _pays(nom_p)
);
CREATE TABLE _lieu (
    id_lieu CHAR,
    nom_lieu CHAR,
    accesPMR BOOLEAN,
    capacite_max INT,
    Type_lieu CHAR,
    ville CHAR,
    CONSTRAINT _lieu_pk PRIMARY KEY (id_lieu),
    CONSTRAINT _lieu_fk_ville FOREIGN KEY (ville) REFERENCES _ville(nom_v)
);

CREATE TABLE _concert (
    no_concert CHAR,
    titre CHAR,
    resume CHAR,
    duree INT,
    tarif FLOAT,
    edition CHAR,
    CONSTRAINT _concert_pk PRIMARY KEY (no_concert),
    CONSTRAINT _concert_fk_edition FOREIGN KEY (edition) REFERENCES _edition(nom_edition) 
);

CREATE TABLE _groupe_artiste (
    id_groupe_artiste CHAR,
    nUm_groupe_artiste CHAR,
    site_web CHAR,
    an_debut INT,
    an_sortie INT,
    pays CHAR,
    CONSTRAINT _grp_artiste_pk PRIMARY KEY (id_groupe_artiste),
    CONSTRAINT _groupe_artiste_fk_annee FOREIGN KEY (an_debut) REFERENCES _annee(an),
    CONSTRAINT _groupe_artiste_fk2_annee FOREIGN KEY (an_sortie) REFERENCES _annee(an),
    CONSTRAINT _groupe_artiste_fk_pays FOREIGN KEY (pays) REFERENCES _pays(nom_p)
);

CREATE TABLE _representation (
    numero_representation CHAR,
    heure CHAR,
    date_representation DATE,
    lieu CHAR,
    no_concert CHAR,
    groupe_artiste CHAR,
    CONSTRAINT _representation_pk PRIMARY KEY (numero_representation),
    CONSTRAINT _representation_fk_lieu FOREIGN KEY (lieu) REFERENCES _lieu(id_lieu) ,
    CONSTRAINT _representation_fk_concert FOREIGN KEY (no_concert) REFERENCES _concert(no_concert),
    CONSTRAINT _representation_fk_groupe_artiste FOREIGN KEY (groupe_artiste)  REFERENCES _groupe_artiste(id_groupe_artiste)
);

CREATE TABLE _type_ponctuel (
    id_groupe_artiste CHAR,
    type_m CHAR,
    CONSTRAINT _type_ponctuel_pk PRIMARY KEY (id_groupe_artiste,type_m),
    CONSTRAINT _type_ponctuel_fk_groupe_artiste FOREIGN KEY (id_groupe_artiste) REFERENCES _groupe_artiste(id_groupe_artiste),
    CONSTRAINT _type_ponctuel_fk_type_musique FOREIGN KEY (type_m) REFERENCES _type_musique(type_m)
);

CREATE TABLE _type_principal (
    id_groupe_artiste CHAR,
    type_m CHAR,
    CONSTRAINT _type_principal_pk PRIMARY KEY (id_groupe_artiste,type_m),
    CONSTRAINT _type_principal_fk_groupe_artiste FOREIGN KEY (id_groupe_artiste) REFERENCES _groupe_artiste(id_groupe_artiste),
    CONSTRAINT _type_principal_fk_type_musique FOREIGN KEY (type_m) REFERENCES _type_musique(type_m)
);
 
CREATE TABLE _a_pour_formation (
    id_groupe_artiste CHAR,
    libelee_formation CHAR,
    CONSTRAINT _a_pour_formation_pk PRIMARY KEY (id_groupe_artiste,libelee_formation),
    CONSTRAINT _a_pour_formation_fk_groupe_artiste FOREIGN KEY (id_groupe_artiste) REFERENCES _groupe_artiste(id_groupe_artiste),
    CONSTRAINT _a_pour_formation_fk_formation FOREIGN KEY (libelee_formation) REFERENCES _formation(libelee_formation)
);
