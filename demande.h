#ifndef DEMANDE_H_INCLUDED
#define DEMANDE_H_INCLUDED

#include <stdio.h>
#define DATE_SIZE 15
#define DEMANDE_FILE "demandes.bin"

/* Constantes de statut */
#define STATUT_EN_ATTENTE 0
#define STATUT_ACCEPTEE 1
#define STATUT_REFUSEE 2
typedef enum { EN_COURS = 0, ACCEPTEE = 1, REFUSEE = 2 } StatutDemande;
/* Demande structure */
typedef struct Demande {
    int idDemande;
    int idClient;
    double montant;
    double taux;
    int duree;
    char dateDemande[DATE_SIZE];
    int statut;
} Demande;


typedef struct DemandeNode {
    Demande demande;
    struct DemandeNode *suiv;
} DemandeNode;

/* ---------- API du module Demande ---------- */

/* Fonctions d'E/S */
DemandeNode* chargerDemandes(void);
int sauvegarderDemandes(DemandeNode *head);
void libererDemandes(DemandeNode *head);

/* Fonctions CLIENT */
int envoyerDemande_interactif(int idClient);
void afficherHistoriqueDemandesClient(DemandeNode *head, int idClient);
void menuDemandes();
/* Fonctions ADMIN */
void afficherDemandes(DemandeNode *head);
void afficherDemandesEnAttente(DemandeNode *head);
int traiterDemandesInteractive(void);
int supprimerDemandeParId(DemandeNode **headRef, int id);

#endif /* DEMANDE_H_INCLUDED */
