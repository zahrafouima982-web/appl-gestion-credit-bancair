#ifndef COMPTECLIENT_H_INCLUDED
#define COMPTECLIENT_H_INCLUDED

#include "client.h"

// ================================
// STRUCTURES
// ================================
typedef struct {
    Client client;
    float solde;
    char motDePasse[50];
} CompteClient;

typedef struct CompteClientNode {
    CompteClient compte;
    struct CompteClientNode *next;
} CompteClientNode;
typedef struct offreCredit{
    char type_credit[30];
    float montant;
    int duree;
    float mensualite;
    float interet;
    float capital_rembourse;
} offreCredit;

// ================================
// FONCTIONS DE GESTION
// ================================
void afficherCompte(CompteClientNode *c);

// dépôt & retrait
int deposer(CompteClientNode *compte, float montant);  // Retourne 1 si succès, 0 si erreur
int retirer(CompteClientNode *compte, float montant);

// création
CompteClientNode* creerCompte(int idClient, float soldeInitial);

// liste chainée
CompteClientNode* ajouterCompteListe(CompteClientNode *tete, CompteClientNode *compte);
CompteClientNode* chercherCompte(CompteClientNode *tete, int idClient);
void libererListeComptes(CompteClientNode *tete);

// fichier
CompteClientNode* chargerComptesFichier(void);
int sauvegarderComptesFichier(CompteClientNode *tete);  // Retourne 1 si succès, 0 si erreur

// suppression
int supprimerCompte(int idClient);  // Supprime le compte d'un client, retourne 1 si succès, 0 sinon

// validation
int validerMontantCompte(float montant);  // Valide qu'un montant est valide pour un compte
int compteExiste(int idClient);  // Vérifie si un compte existe déjà pour un client
int verifierIntegriteComptes(ClientNode *clients, CompteClientNode *comptes);  // Vérifie l'intégrité des comptes
 int verifierSolde(CompteClientNode *compte, float montant);
// modification
int modifierCompte(int idClient);  // Modifie un compte existant (solde, mot de passe)

// statistiques
void afficherStatistiquesComptes(void);  // Affiche des statistiques sur les comptes
float calculerSoldeTotal(void);  // Calcule le solde total de tous les comptes

// menus
void creerCompteMenu(void);
void afficherTousComptesMenu(void);
void chercherCompteMenu(void);
void deposerMenu(void);
void retirerMenu(void);
void menuGestionComptes(void);





#endif // COMPTECLIENT_H_INCLUDED
