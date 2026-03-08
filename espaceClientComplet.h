#ifndef ESPACE_CLIENT_COMPLET_H
#define ESPACE_CLIENT_COMPLET_H
#include "compteClient.h"
#include "client.h"
#include "credit.h"
#include "demande.h"

// Couleurs ANSI
#define RED     "\033[1;31m"
#define GREEN   "\033[1;32m"
#define YELLOW  "\033[1;33m"
#define CYAN    "\033[1;36m"
#define MAGENTA "\033[1;35m"
#define L_GREEN "\033[1;92m"
#define BOLD    "\033[1m"
#define RESET   "\033[0m"

// Forward declarations
typedef struct PaiementNode PaiementNode;
typedef struct CreditNode CreditNode;

// Fonction principale de l'espace client
CompteClientNode* espaceClientComplet(void);

// Fonctions du menu client
void afficherInformationsClient(CompteClientNode *compte);
void modifierInformationsPersonnelles(CompteClientNode *compte);
void consulterDemandesCredit(CompteClientNode *compte);
void deposerDemandeCredit(CompteClientNode *compte);
void voirMesCredits(CompteClientNode *compte);
void accederPaiementsCredit(CompteClientNode *compte);
void effectuerPaiementAutomatique(CompteClientNode *compte, int idCredit);
void afficherOffresCredit();
void simulationCreditSimple();
int  authentifierClient();
#endif
