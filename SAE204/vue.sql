SET SCHEMA 'parcoursup3';

CREATE VIEW admission AS SELECT cod_aff_form,session_annee,effectif_total_candidats,effectif_total_candidates,effectif_total_admis,effectif_total_admises FROM "_admissions_generalites";

CREATE VIEW admission_type_neo AS SELECT cod_aff_form, session_annee, type_bac,effectif_candidat_neo_bac_classes FROM "_admissions_selon_type_neo_bac";

DROP VIEW admission_selon;
CREATE OR REPLACE VIEW "admission_selon" AS SELECT cod_aff_form,session_annee,type_bac,effectif_total_candidats,effectif_total_candidates,effectif_total_admis,effectif_total_admises FROM admission_type_neo NATURAL JOIN admission;

SELECT * FROM admission_selon;