#ifndef STATISTIQUE_H_INCLUDED
#define STATISTIQUE_H_INCLUDED
#include "compteClient.h"
#include "credit.h"
#include "demande.h"
#include "paiement.h"

/* Structure des Indicateurs Cl�s pour l'Admin (Globaux Credit) */
typedef struct StatsCredit {
    int totalCredits;
    int creditsActifs;
    int creditsClotures;
    double montantTotalEmis;
    double montantTotalRestant;
    double tauxMoyen;
    double ratioRemboursement;
    double mensualiteTotaleMensuelle;
} StatsCredit;

/* NOUVEAU: Structure des Indicateurs pour les Paiements */
typedef struct StatsPaiement {
    int totalPaiementsPrevu;
    int paiementsReussis;
    int paiementsEchoues; // Paiements en retard / �chec
    double montantTotalPaye;
    double montantTotalEchoue;
    double penalitesCumulees;
} StatsPaiement;


/* Structure des Indicateurs pour les Demandes */
typedef struct StatsDemandes {
    int totalDemandes;
    int demandesEnAttente;
    int demandesAcceptees;
    int demandesRefusees;
    double montantTotalDemande;
    double tauxAcceptation;
} StatsDemandes;

/* Structure des Indicateurs pour un Client Individuel */
typedef struct StatsClient {
    int creditsTotal;
    int creditsActifs;
    double montantEmprunteTotal;
    double montantRestantClient;
    double mensualiteTotaleClient;
} StatsClient;


/* ---------- API du module STATISTIQUES ---------- */
void sousmenuStatistique(void);
/** 1. Statistique Credits (Globale) **/
void afficherStatistiquesCredit(void);

/** 2. Statistique Demandes (Globale) **/
void afficherStatistiquesDemandes(void);

/** 3. NOUVEAU: Statistique Paiements (Globale) **/
void afficherStatistiquesPaiements(void);

/** 4. Statistique Clients (Individuelle) **/
void afficherStatistiquesClient(int idClient);

#endif
