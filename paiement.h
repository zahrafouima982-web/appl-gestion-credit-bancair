#ifndef PAIEMENT_H
#define PAIEMENT_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "credit.h"
#include "paiement.h"
#include "client.h"
#include "compteClient.h"

// ===============================
//       CONSTANTES
// ===============================

#define FICHIER_PAIEMENTS "paiements.txt"
#define PENALITE_RETARD 50.0f

// ===============================
//       CONSTANTES / ENUMS
// ===============================

typedef enum {
    STATUT_ATTENTE = 0,
    STATUT_PAYE    = 1,
    STATUT_ECHEC   = 2
} StatutPaiement;

// ===============================
//       STRUCTURE PAIEMENT
// ===============================
typedef struct paiement{
	int IDPaiement;               // Identifiant unique du paiement
    int IDCredit;                 // Référence au crédit
    int numMois;                  // Numéro du mois du crédit (1, 2, ...)
    float montant;                // Montant de la mensualité
    float montantPenalite;        // Pénalité appliquée si paiement échoue
    char datePrevue[11];          // Date prévue de paiement "AAAA-MM-JJ"
    char datePaiement[11];        // Date effective du paiement
    StatutPaiement statut;        // Statut : ATTENTE, PAYE, ECHEC
} Paiement;

typedef struct PaiementNode {
    Paiement data;
    struct PaiementNode *next;
} PaiementNode;

// ====== Gestion de la liste ======
PaiementNode* ajouterPaiementListe(PaiementNode *tete, Paiement p);
void libererListePaiements(PaiementNode *tete);

// ====== Lecture / écriture fichier ======
PaiementNode* chargerPaiementsFichier();
int sauvegarderPaiementsFichier(PaiementNode *liste);  // Retourne 1 si succès, 0 si erreur

// ====== Fonctions utilitaires ======
double calculerMensualiteCredit(Credit credit);  // Wrapper pour calculerMensualite du module crédit
void ajouterMois(char *dateStr);

// ====== Validation ======
int validerDate(const char *date);              // Valide le format et la cohérence d'une date
int validerMontant(float montant);              // Valide qu'un montant est positif et raisonnable
int verifierIntegritePaiements(CreditNode *credits, PaiementNode *paiements);  // Vérifie l'intégrité des données

// ====== Gestion des crédits & paiements ======
void genererCalendrierPaiements(Credit *credit);
void afficherPaiementsCredit(Credit *credit);
void paiementAutomatiqueMensuel();
void paiementManuel(int idCredit, int idPaiement);
float capitalRestant(int idCredit);
// Fonction ajouterCreditMenu déclarée plus bas

// ====== Fonctions Menu / Interface Console ======
void afficherPaiementsCreditMenu(CreditNode *listeCredits);
void paiementAutomatiqueMenu();
void paiementManuelMenu();
void capitalRestantMenu(CreditNode *listeCredits);
void ajouterCreditMenu(CreditNode *listeCredits);
void menuPaiements(CreditNode *listeCredits, CompteClientNode *listeComptes);  // menu principal de gestion des paiements
void menuGestionPaiements(void);  // wrapper qui charge les données et appelle menuPaiements

#endif // PAIEMENT_H
