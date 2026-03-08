#include "espaceClientComplet.h"
#include "style.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>

#ifdef _WIN32
#include <conio.h>
#endif

#ifndef FICHIER_PAIEMENTS
#define FICHIER_PAIEMENTS "paiements(1).txt"
#endif

// --- Structures et Enums (Conservés à l'identique) ---
typedef enum {
    STATUT_ATTENTE = 0,
    STATUT_PAYE    = 1,
    STATUT_ECHEC   = 2
} StatutPaiement;

typedef struct paiement {
    int IDPaiement;
    int IDCredit;
    int numMois;
    float montant;
    float montantPenalite;
    char datePrevue[11];
    char datePaiement[11];
    StatutPaiement statut;
} Paiement;
typedef struct demandecredit {
    char CIN[20];
     StatutDemande statut;
    float montant_demander;
     char type_credit[30];
     int duree;
}demandecredit;

typedef struct PaiementNode {
    Paiement data;
    struct PaiementNode *next;
} PaiementNode;

// --- DÉCLARATIONS EXTERNES ---
PaiementNode* chargerPaiementsFichier(void);
int sauvegarderPaiementsFichier(PaiementNode *liste);
void libererListePaiements(PaiementNode *tete);
void genererCalendrierPaiements(Credit *credit);
#include "compteClient.h"

// --- FONCTION DE CHARGEMENT ---
int chargerCompteClient(int id, CompteClient *cc) {
    FILE *f = fopen("comptesClients.txt", "r");
    if (!f) return 0;
    while (fscanf(f, "%d %19s %49s %49s %10s %49s %f %49s",
                  &cc->client.ID, cc->client.cin, cc->client.nom, cc->client.prenom,
                  cc->client.telephone, cc->client.email,
                  &cc->solde, cc->motDePasse) == 8) {
        if (cc->client.ID == id) {
            fclose(f);
            return 1;
        }
    }
    fclose(f);
    return 0;
}

// =========================================================
// 1. ESPACE CLIENT (MENU PRINCIPAL)
// =========================================================
CompteClientNode* espaceClientComplet(void) {
    CompteClient cc;
    CompteClientNode *compteConnecte = NULL;
    int choix;
    int idSaisi;
    char mdpSaisi[50];

    // --- Connexion client ---
    espaceCentre();cadre();
    printf(CYAN "                                              CONNEXION CLIENT             \n" RESET);
    espaceCentre();cadre();
printf("\n");
    espaceCentre();printf(YELLOW "ID : " RESET);
    scanf("%d", &idSaisi);
    while(getchar() != '\n');

    espaceCentre();printf(YELLOW "Mot de passe : " RESET);
    scanf("%49s", mdpSaisi);
    while(getchar() != '\n');

    if (!chargerCompteClient(idSaisi, &cc)) {
        printf(RED " ID non trouvable.\n" RESET);
        return NULL;
    }
    if (strcmp(mdpSaisi, cc.motDePasse) != 0) {
        printf(RED "Mot de passe incorrect.\n" RESET);
        return NULL;
    }

    espaceCentre();printf(GREEN "Authentification reussie ! Bienvenue %s %s\n" RESET, cc.client.nom, cc.client.prenom);

    // --- Créer un noeud temporaire pour le menu ---
    compteConnecte = (CompteClientNode*) malloc(sizeof(CompteClientNode));
    compteConnecte->compte = cc;
    compteConnecte->next = NULL;

    // --- Menu client ---
    do {
        espaceCentre();printf(BLEU "                       -----👤 Client ID %d : Menu ----\n" RESET, compteConnecte->compte.client.ID);
        espaceCentre();printf(BLANC_GRAS"          +---------------------------------------------------+\n"RESET);
        espaceCentre();printf(BLANC_GRAS "         |  1. Consulter mes informations                    |\n" RESET);
        espaceCentre();printf(BLANC_GRAS "         |  2. ⚙ Modifier mes informations personnelles      |\n" RESET);
        espaceCentre();printf(BLANC_GRAS "         |  3. 📝 Soumettre une nouvelle demande de credit   |\n" RESET);
        espaceCentre();printf(BLANC_GRAS "         |  4. 📄 Voir l'historique de mes demandes          |\n" RESET);
        espaceCentre();printf(BLANC_GRAS "         |  5. 💰 Voir mes credits actifs                    |\n" RESET);
        espaceCentre();printf(BLANC_GRAS "         |  6. Acceder aux paiements d'un credit             |\n" RESET);
        espaceCentre();printf(BLANC_GRAS "         |  7. 📊 Voir mon resume financier (Statistiques)   |\n" RESET);
        espaceCentre();printf(BLANC_GRAS "         |  8. 🧮 Simuler mon credit                         |\n" RESET);
        espaceCentre();printf(BLANC_GRAS "         |  9. 📑 Voir les offres disponibles                |\n" RESET);
        espaceCentre();printf(BLANC_GRAS "         |  0. 🔙 Deconnexion                                |\n" RESET);
        espaceCentre();printf(BLANC_GRAS"          +---------------------------------------------------+\n"RESET);
        printf("\n");
         espaceCentre(); printf("            ➤ Votre choix : ");

        if (scanf("%d", &choix) != 1) {
            while(getchar() != '\n');
            choix = -1;
        }
        while(getchar() != '\n');

        switch (choix) {
            case 1:
                afficherInformationsClient(compteConnecte);
                break;
            case 2:
                modifierInformationsPersonnelles(compteConnecte);
                break;
            case 3:
                deposerDemandeCredit(compteConnecte);
                break;
            case 4:
                consulterDemandesCredit(compteConnecte);
                break;
            case 5:
                voirMesCredits(compteConnecte);
                break;
            case 6:
                accederPaiementsCredit(compteConnecte);
                break;
            case 7:
                afficherStatistiquesClient(compteConnecte->compte.client.ID);
                break;
            case 8:
                simulationCreditSimple();
                break;
            case 9:
                afficherOffresCredit();
                break;
            case 0:
                printf(GREEN BOLD "\nDeconnexion reussie. Merci d'avoir utilise notre plateforme bancaire.\n" RESET);
                break;
            default:
                printf(RED "Option invalide. Veuillez ressayer.\n" RESET);
                break;
        }

    } while (choix != 0);

    free(compteConnecte);
    return NULL;
}

   // À l'intérieur d'une fonction, par exemple après modification du compte

// Libération mémoire

// Fonction 1: Afficher les informations du client
void afficherInformationsClient(CompteClientNode *compte) {

    if (!compte) {
        printf(RED BOLD "Erreur : compte non valide.\n" RESET);
        return;
    }

    printf(L_GREEN BOLD "\n==============================================================\n" RESET);
    printf(MAGENTA BOLD "                    MON COMPTE CLIENT\n" RESET);
    printf(L_GREEN BOLD "==============================================================\n" RESET);

    printf(YELLOW BOLD "%-8s %-12s %-10s %-10s %-15s %-30s %-15s\n" RESET,
           "ID", "CIN", "Nom", "Prenom", "Telephone", "Email", "Solde");

    printf(L_GREEN BOLD "--------------------------------------------------------------"
                        "--------------------------------------\n" RESET);

    printf(YELLOW "%-8d %-12s %-10s %-10s %-15s %-30s %.2f DH\n" RESET,
           compte->compte.client.ID,
           compte->compte.client.cin,
           compte->compte.client.nom,
           compte->compte.client.prenom,
           compte->compte.client.telephone,
           compte->compte.client.email,
           compte->compte.solde);

    printf(L_GREEN BOLD "--------------------------------------------------------------"
                        "--------------------------------------\n" RESET);
}
// =========================================================
// 2. FONCTIONS DE GESTION DU COMPTE
// =========================================================


void modifierInformationsPersonnelles(CompteClientNode *compte) {
    int choix;
    char buffer[50];
    do {
        printf("\n" CYAN "=== Modifier mon compte ===" RESET);
        printf("\n1 - Modifier Email\n2 - Modifier Telephone\n3 - Modifier Mot de passe\n0 - Retour\nChoix : ");
        scanf("%d", &choix); getchar();

        if (choix == 1) {
            printf("Nouvel Email : "); saisirChaine(buffer, 50);
            strcpy(compte->compte.client.email, buffer);
            printf(GRAS "Email modifie !" RESET "\n");
        } else if (choix == 2) {
            printf("Nouveau Tel : "); saisirChaine(buffer, 50);
            strcpy(compte->compte.client.telephone, buffer);
            printf(GRAS "Telephone  modifie !" RESET "\n");
        } else if (choix == 3) {
            printf("Nouveau MDP : "); saisirChaine(buffer, 50);
            strcpy(compte->compte.motDePasse, buffer);
            printf(GRAS "Mot de passe modifie!" RESET "\n");
        }
    } while (choix != 0);
}

// =========================================================
// 3. FONCTIONS CRÉDITS ET PAIEMENTS
// =========================================================

void voirMesCredits(CompteClientNode *compte) {
    CreditNode *liste = chargerCredits();
    CreditNode *tmp = liste;
    int trouve = 0;
    printf("\n" CYAN "==================== VOS CREDITS ====================" RESET "\n");
    printf("%-10s | %-12s | %-6s | %-15s\n", "ID Credit", "Capital", "Duree", "Reste a payer");
    while (tmp) {
        if (tmp->credit.idClient == compte->compte.client.ID) {
            printf("%-10d | %-10.2f | %-6d | " ROUGE "%.2f DH" RESET "\n",
                   tmp->credit.idCredit, tmp->credit.montant, tmp->credit.duree, tmp->credit.montantRestant);
            trouve = 1;
        }
        tmp = tmp->suiv;
    }
    if (!trouve) printf(BLANC_GRAS "Aucun Credit trouve." RESET "\n");
    libererCredits(liste);
}

void accederPaiementsCredit(CompteClientNode *compte) {
    int idCredit;
    printf("\nID du Credit : ");
    scanf("%d", &idCredit); getchar();

    CreditNode *listeC = chargerCredits();
    CreditNode *credit = rechercherCreditParId(listeC, idCredit);
    if (!credit || credit->credit.idClient != compte->compte.client.ID) {
        printf(ROUGE "Credit invalide." RESET "\n");
        libererCredits(listeC); return;
    }

    PaiementNode *listeP = chargerPaiementsFichier();
    PaiementNode *tmp = listeP;
    printf("\n" BLEU "--- CALENDRIER DES PAIEMENTS ---" RESET "\n");
    int trouveP = 0;
    while (tmp) {
        if (tmp->data.IDCredit == idCredit) {
            printf("Mois %d | Date: %s | Montant: %.2f | Statut: ", tmp->data.numMois, tmp->data.datePrevue, tmp->data.montant);
            if (tmp->data.statut == STATUT_PAYE) printf(BLANC_GRAS "PAYE" RESET "\n");
            else if (tmp->data.statut == STATUT_ECHEC) printf(ROUGE "ECHEC" RESET "\n");
            else printf(JAUNE "ATTENTE" RESET "\n");
            trouveP = 1;
        }
        tmp = tmp->next;
    }

    if (!trouveP) {
        printf(BLANC_GRAS "Generation du calendrier..." RESET "\n");
        genererCalendrierPaiements(&(credit->credit));
    }

    printf("\nPayer le mois courant ? (oui/non) : ");
    char rep[10]; scanf("%s", rep);
    if (strcmp(rep, "oui") == 0) effectuerPaiementAutomatique(compte, idCredit);

    libererCredits(listeC);
    libererListePaiements(listeP);
}

void effectuerPaiementAutomatique(CompteClientNode *compte, int idCredit) {
    PaiementNode *listeP = chargerPaiementsFichier();
    PaiementNode *cible = NULL;
    PaiementNode *tmp = listeP;

    while (tmp) {
        if (tmp->data.IDCredit == idCredit && tmp->data.statut == STATUT_ATTENTE) {
            cible = tmp; break;
        }
        tmp = tmp->next;
    }

    if (!cible) {
        printf(GRAS "Pas de mensualite en attente." RESET "\n");
        libererListePaiements(listeP); return;
    }

    if (compte->compte.solde >= cible->data.montant) {
        compte->compte.solde -= cible->data.montant;
        cible->data.statut = STATUT_PAYE;

        CreditNode *lc = chargerCredits();
        CreditNode *cr = rechercherCreditParId(lc, idCredit);
        if(cr) cr->credit.montantRestant -= cible->data.montant;
        sauvegarderCredits(lc); libererCredits(lc);

        printf(BLANC_GRAS " Paiement reussi ! Nouveau solde : %.2f DH" RESET "\n", compte->compte.solde);
    } else {
        cible->data.statut = STATUT_ECHEC;
        cible->data.montantPenalite = 50.0f;
        printf(ROUGE " Solde insuffisant ! Penalite appliquee." RESET "\n");
    }

    sauvegarderPaiementsFichier(listeP);
    libererListePaiements(listeP);
}

// =========================================================
// 4. FONCTIONS ADDITIONNELLES (OFFRES ET SIMULATION)
// =========================================================

void afficherOffresCredit() {
    FILE *f = fopen("offres.txt", "r");

    if (!f) {
        printf(RED "Erreur : fichier introuvable\n" RESET);
        return;
    }

    offreCredit o;
    int console_width = 60; // largeur totale du cadre
    printf(L_GREEN BOLD "\n===== Offres de credit disponibles =====\n\n" RESET);

    while (fscanf(f, "%s %f %d %f %f %f",
                  o.type_credit, &o.montant, &o.duree,
                  &o.mensualite, &o.interet, &o.capital_rembourse) == 6) {
        int i;

        // Ligne du haut du cadre (étoiles magenta)
        for (i = 0; i < console_width; i++) printf(MAGENTA BOLD "*" RESET);
        printf("\n");

        // Titre centre
        char titre[100];
        sprintf(titre, "  %s  ", o.type_credit);
        int padding = (console_width - (int)strlen(titre)) / 2;
        printf(MAGENTA BOLD "*");
        for (i = 1; i < console_width - 1; i++) {
            if (i >= padding && i < padding + strlen(titre))
                printf(L_GREEN BOLD "%c" RESET, titre[i - padding]);
            else
                printf(" ");
        }
        printf(MAGENTA BOLD "*\n" RESET);

        // Ligne de séparation
        for (i = 0; i < console_width; i++) printf(MAGENTA BOLD "*" RESET);
        printf("\n");

        // Détails centrés
        char ligne[100];
        sprintf(ligne, "Montant total : %.2f DH", o.montant);
        padding = (console_width - (int)strlen(ligne)) / 2;
        for (i = 0; i < padding; i++) printf(" ");
        printf(YELLOW "%s" RESET "\n", ligne);

        sprintf(ligne, "Duree : %d mois", o.duree);
        padding = (console_width - (int)strlen(ligne)) / 2;
        for (i = 0; i < padding; i++) printf(" ");
        printf(YELLOW "%s" RESET "\n", ligne);

        sprintf(ligne, "Mensualite : %.2f DH/mois", o.mensualite);
        padding = (console_width - (int)strlen(ligne)) / 2;
        for (i = 0; i < padding; i++) printf(" ");
        printf(YELLOW "%s" RESET "\n", ligne);

        sprintf(ligne, "Interet 1er mois : %.2f DH", o.interet);
        padding = (console_width - (int)strlen(ligne)) / 2;
        for (i = 0; i < padding; i++) printf(" ");
        printf(YELLOW "%s" RESET "\n", ligne);

        sprintf(ligne, "Capital rembourse : %.2f DH", o.capital_rembourse);
        padding = (console_width - (int)strlen(ligne)) / 2;
        for (i = 0; i < padding; i++) printf(" ");
        printf(YELLOW "%s" RESET "\n", ligne);

        // Ligne du bas du cadre (étoiles magenta)
        for (i = 0; i < console_width; i++) printf(MAGENTA BOLD "*" RESET);
        printf("\n\n");
    }

    fclose(f);
}

void consulterDemandesCredit(CompteClientNode *compte) {
    DemandeNode *liste = chargerDemandes();
    DemandeNode *tmp = liste;
    int trouve = 0;
    printf("\n" CYAN "--- VOS DEMANDES DE CREDIT ---" RESET "\n");
    while (tmp) {
        if (tmp->demande.idClient == compte->compte.client.ID) {
            printf("ID Demande: %d | Montant: %.2f | Statut: %d\n", tmp->demande.idDemande, tmp->demande.montant, tmp->demande.statut);
            trouve = 1;
        }
        tmp = tmp->suiv;
    }
    if (!trouve) printf("Aucune demande en cours.\n");
    libererDemandes(liste);
}

void deposerDemandeCredit(CompteClientNode *compte) {
    envoyerDemande_interactif(compte->compte.client.ID);
}

void simulationCreditSimple() {
    demandecredit c;
    int choix_credit;
    char CIN_client[20];
    // Entree CIN decoree
    printf(L_GREEN BOLD "\n********************************************\n" RESET);
    printf(MAGENTA BOLD "*        SIMULATION DE CREDIT CLIENT       *\n" RESET);
    printf(L_GREEN BOLD "********************************************\n" RESET);
    printf(YELLOW "Veuillez saisir votre CIN : " RESET);
    scanf("%19s", CIN_client);
     while (getchar() != '\n');
     printf(YELLOW "Veuillez saisir le montant demande : " RESET);
    scanf("%f",&c.montant_demander);
    while (getchar() != '\n');
    printf(YELLOW "Veuillez saisir la duree: " RESET);
    scanf("%d",&c.duree);
      while (getchar() != '\n');
    printf(L_GREEN BOLD "********************************************\n" RESET);
        double tauxAnnuel;
        // --- Saisie du Type de crédit par liste ---
    printf(YELLOW BOLD "\n les type de credit disponibles :\n" RESET);
    printf(L_GREEN "1. Immobilier (Taux par defaut: 4.0%%)\n" RESET);
    printf(L_GREEN "2. Auto (Taux par defaut: 6.0%%)\n" RESET);
    printf(L_GREEN "3. Consommation/Autre (Taux par defaut: 8.0%%)\n" RESET);
    printf(YELLOW BOLD "\nChoisissez le type de votre credit :\n" RESET);
    scanf("%d",&choix_credit);
        switch (choix_credit) {
        case 1:
            strcpy(c.type_credit, "immobilier");
            tauxAnnuel = 4.0;
            break;
        case 2:
            strcpy(c.type_credit, "auto");
            tauxAnnuel = 6.0;
            break;
        case 3:
        default:
            strcpy(c.type_credit, "consommation");
            tauxAnnuel = 8.0;
            break;
    }
        double tauxMensuel = tauxAnnuel / 12 / 100;
        double mensualite = (c.montant_demander * tauxMensuel) / (1 - pow(1 + tauxMensuel, -c.duree));
        double capitalRestant = c.montant_demander;
        double interet;

        // --- Affichage client ---
        printf(L_GREEN BOLD "\n********************************************\n" RESET);
        printf(MAGENTA BOLD "* Simulation de credit pour le client : %s *\n" RESET, c.CIN);
        printf(L_GREEN BOLD "********************************************\n" RESET);
        printf(YELLOW "Montant demande : " RESET "%.2f DH\n", c.montant_demander);
        printf(YELLOW "Type de credit  : " RESET "%s\n", c.type_credit);
        printf(YELLOW "Duree           : " RESET "%d mois\n", c.duree);
        printf(YELLOW "Taux annuel     : " RESET "%.2f%%\n", tauxAnnuel);
        printf(YELLOW "Mensualite       : " RESET "%.2f DH/mois\n\n", mensualite);
        // --- Tableau decoratif avec separateurs gras et colores ---
        printf(L_GREEN BOLD "+-----+------------+-----------+-----------------+----------------+\n" RESET);
        printf(YELLOW BOLD "| Mois| Mensualite | Interets  | Capital rembourse | Capital restant |\n" RESET);
        printf(L_GREEN BOLD "+-----+------------+-----------+-----------------+----------------+\n" RESET);

int i;
        for ( i = 1; i <= c.duree; i++) {
            interet = capitalRestant * tauxMensuel;
            double capitalRembourse = mensualite - interet;
            capitalRestant -= capitalRembourse;
            if (capitalRestant < 0) capitalRestant = 0;

            printf(YELLOW "|%4d ", i);
            printf("|%10.2f ", mensualite);
            printf("|%9.2f ", interet);
            printf("|%16.2f ", capitalRembourse);
            printf("|%15.2f|\n" RESET, capitalRestant);
        }
        printf(L_GREEN BOLD "+-----+------------+-----------+-----------------+----------------+\n" RESET);
        double coutTotal = mensualite * c.duree;
        printf(GREEN BOLD "Cout total du credit : %.2f DH\n" RESET, coutTotal);
        printf(L_GREEN BOLD "=====================================================\n" RESET);

}
