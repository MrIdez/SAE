CREATE SCHEMA IF NOT EXISTS parcoursup;
SET SCHEMA 'parcoursup';

CREATE TABLE _academie (
                          academie_nom VARCHAR NOT NULL,
                          CONSTRAINT ACADEMIE_pk primary key (academie_nom)
);

CREATE TABLE _type_bac (
    type_bac VARCHAR,
    CONSTRAINT TYPE_BAC_pk primary key (type_bac)
);

CREATE TABLE _filiere (
                         filiere_id INT NOT NULL,
                         filiere_libelle VARCHAR NOT NULL,
                         filiere_libelle_tres_abrege VARCHAR NOT NULL,
                         filiere_libelle_abrege VARCHAR NOT NULL,
                         filiere_libelle_detaille_bis VARCHAR NOT NULL,
                         CONSTRAINT FILIERE_pk primary key (filiere_id)
);

CREATE TABLE _etablissement (
                               etablissement_code_uai VARCHAR NOT NULL,
                               etablissement_nom VARCHAR NOT NULL,
                               etablissement_statut VARCHAR NOT NULL,
                               CONSTRAINT ETABLISSEMENT_pk primary key (etablissement_code_uai)
);

CREATE TABLE _session (
                         session_anne INT,
                         CONSTRAINT SESSION_pk primary key (session_anne)
);

CREATE TABLE _region (
                        region_nom VARCHAR NOT NULL,
                        CONSTRAINT REGION_pk primary key (region_nom)
);

CREATE TABLE _departement (
                             departement_code VARCHAR NOT NULL,
                             departement_nom VARCHAR NOT NULL,
                             region VARCHAR NOT NULL,
                             CONSTRAINT DEPARTEMENT_pk primary key (departement_code),
                             CONSTRAINT DEPARTEMENT_fk_REGION foreign key (region) REFERENCES _region(region_nom)
);

CREATE TABLE _commune (
                         departement_communee VARCHAR,
                         CONSTRAINT COMMUNE_fk_DEPARTEMENT foreign key (departement_communee) REFERENCES _departement(departement_code)
);

CREATE TABLE _formation (
    codd_aff_form VARCHAR NOT NULL,
    filiere_libelle_detaille VARCHAR NOT NULL,
    coordonnees_gps VARCHAR NOT NULL,
    list_com VARCHAR NOT NULL,
    concours_communs_banque_epreuve VARCHAR NOT NULL,
    url_formation VARCHAR NOT NULL,
    tri VARCHAR NOT NULL,
    academie_nom VARCHAR NOT NULL,
    etablissement VARCHAR NOT NULL,
    filiere INT NOT NULL,
    constraint formation_pk primary key (codd_aff_form),
    CONSTRAINT FORMATION_fk_ACADEMIE foreign key (academie_nom) REFERENCES _academie(academie_nom),
    CONSTRAINT FORMATION_fk_ETABLISSEMENT FOREIGN KEY (etablissement) REFERENCES _etablissement(etablissement_code_uai),
    CONSTRAINT FORMATION_fk_FILIERE FOREIGN KEY (filiere) REFERENCES _filiere(filiere_id)
);

CREATE TABLE _admission_generalites (
    selective VARCHAR NOT NULL ,
    capacite INT NOT NULL ,
    effectif_total_candidats INT NOT NULL ,
    effectif_total_candidates INT NOT NULL ,
    formation VARCHAR NOT NULL ,
    session INT NOT NULL ,
    CONSTRAINT ADMISSION_GENERALITES_pk primary key (selective),
    CONSTRAINT ADMISSION_fk_FORMATION foreign key (formation) REFERENCES _formation(codd_aff_form),
    CONSTRAINT ADMISSION_fk_SESSION foreign key (session) REFERENCES _session(session_anne)
);

CREATE TABLE _regroupement (
    libelle_regroupement VARCHAR NOT NULL ,
    CONSTRAINT REGROUPEMENT_pk primary key (libelle_regroupement)
);

CREATE TABLE _rang_dernier_appele_selon_regroupement (
    rang_dernier_appel INT,
    code_aff_form VARCHAR,
    libelle_regroupement VARCHAR,
    session_anne INT,
    CONSTRAINT RANG_DERNIER_APPELE_SELON_REGROUPEMENT_pk primary key (code_aff_form,libelle_regroupement,session_anne),
    CONSTRAINT fk_FORMATION foreign key (code_aff_form) REFERENCES _formation(codd_aff_form),
    CONSTRAINT fk_REGROUPEMENT foreign key (libelle_regroupement) REFERENCES _regroupement(libelle_regroupement),
    CONSTRAINT fk_SESSION foreign key (session_anne) REFERENCES _session(session_anne)
);

CREATE TABLE _admission_selon_type_neo_bac (
    effectif_candidat_neo_bac_classes INT,
    type_bac VARCHAR,
    session INT,
    cod_aff_form VARCHAR,
    CONSTRAINT ADMISSION_SELON_TYPE_NEO_BAC_pk primary key (type_bac,session,cod_aff_form),
    CONSTRAINT ADMISSION_SELON_TYPE_NEO_BAC_fk_type_bac foreign key (type_bac) REFERENCES _type_bac(type_bac),
    CONSTRAINT ADMISSION_SELON_TYPE_NEO_BAC_fk foreign key (session) REFERENCES _session(session_anne),
    CONSTRAINT ADMISSION_SELON_TYPE_NEO_BAC_fk_FORMATION foreign key (cod_aff_form) REFERENCES _formation(codd_aff_form)

);

CREATE TABLE _mention_bac (
    libelle_mention VARCHAR NOT NULL,
    CONSTRAINT MENTION_BAC_pk primary key (libelle_mention)
);

CREATE TABLE _effectif_selon_mention (
    effectif_admis_neo_bac_selon_mention_type_mention VARCHAR NOT NULL,
    mention VARCHAR,
    session INT,
    formation VARCHAR,
    CONSTRAINT EFFECTIF_SELON_MENTION_pk primary key (mention,session,formation),
    CONSTRAINT EFFECTIF_SELON_MENTION_fk_MENTION foreign key (mention) REFERENCES _mention_bac(libelle_mention),
    CONSTRAINT EFFECTIF_SELON_MENTION_fk_SESSION foreign key (session) REFERENCES _session(session_anne),
    CONSTRAINT EFFECTIF_SELON_MENTION_fk_FORMATION foreign key (formation) REFERENCES _formation(codd_aff_form)

);

