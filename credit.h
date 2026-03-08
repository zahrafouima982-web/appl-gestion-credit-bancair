#ifndef CREDIT_H_INCLUDED
#define CREDIT_H_INCLUDED

#include <stdio.h>

#define DATE_SIZE 15
#define CREDIT_FILE "credits.bin"

/* Constantes d'état */
#define ETAT_ACTIF 1
#define ETAT_CLOTURE 0
/* Déclaration forward pour éviter la dépendance circulaire */
typedef struct PaiementNode PaiementNode;
/* Structure Credit */
typedef struct Credit {
    int idCredit;
    int idClient;
    double montant;
    double taux;
    int duree;
    double mensualite;
    double montantRestant;
    char dateCreation[DATE_SIZE];
    char date_debut[DATE_SIZE];
    int etat;
    PaiementNode *paiement;
} Credit;

/* Liste chaînée */
typedef struct CreditNode {
    Credit credit;
    struct CreditNode *suiv;
} CreditNode;

/* ---------- API du module crédit ---------- */

/* Fonctions d'E/S */
CreditNode* chargerCredits(void);
int sauvegarderCredits(CreditNode *head);
void libererCredits(CreditNode *head );
void menuGestionCredits(void);

/* Fonctions ADMIN (Gestion complète) */
int ajouterCreditFromInteractive(void);
CreditNode* ajouterCreditNode(CreditNode *head, Credit c);
void afficherCredits(CreditNode *head);
void afficherCreditsActifs(CreditNode *head);
void rechercherEtAfficherCredit(void);
void rechercherEtAfficherCreditsParClient(void);
int modifierCredit(CreditNode *head, int id);
int cloturerCredit(CreditNode *head, int id);
int supprimerCreditInteractive(void);
CreditNode* supprimerCreditNode(CreditNode *head, int id);
CreditNode* rechercherCreditParId(CreditNode *head, int id);

/* Fonctions CLIENT (Consultation) */
void afficherCreditsClient(CreditNode *head, int idClient);

/* Fonctions transversales (Paiement / Intégration) */
double calculerMensualite(double montant, double tauxAnnuel, int dureeMois);
void afficherTableauAmortissement(Credit *c);
struct Demande;
int importerDemandeCommeCredit(const struct Demande *d);

/* MODIFICATION: API standard pour le module Paiement */
int appliquerPaiementACredit(int idCredit, double montantPaiement);

/* Utilitaires */
int prochainIdCredit(void);

#endif /* CREDIT_H_INCLUDED */
