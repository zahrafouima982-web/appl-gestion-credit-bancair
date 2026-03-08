#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "statistique.h"
#include "credit.h"
#include "demande.h"
#include "paiement.h"
#include <math.h>
#include "compteClient.h"
#include "style.h"

/* =============== PARTIE 1: STATISTIQUES CREDITS =============== */

static StatsCredit calculerStatistiquesCredit(CreditNode *head) {
    StatsCredit stats = {0};
    int countTaux = 0;
    if (!head) return stats;
    CreditNode *p;
    for (p = head; p; p = p->suiv) {
        Credit *c = &p->credit;
        stats.totalCredits++;
        stats.montantTotalEmis += c->montant;
        stats.montantTotalRestant += c->montantRestant;
        stats.mensualiteTotaleMensuelle += c->mensualite;
        stats.tauxMoyen += c->taux;
        countTaux++;
        if (c->etat == ETAT_ACTIF) stats.creditsActifs++;
        else stats.creditsClotures++;
    }
    if (countTaux > 0) stats.tauxMoyen /= (double)countTaux;
    if (stats.montantTotalEmis > 0) {
        double montantTotalPaye = stats.montantTotalEmis - stats.montantTotalRestant;
        if (montantTotalPaye < 0) montantTotalPaye = 0;
        stats.ratioRemboursement = (montantTotalPaye / stats.montantTotalEmis) * 100.0;
    }
    return stats;
}

void afficherStatistiquesCredit(void) {
    CreditNode *head = chargerCredits();
    if (!head) {
        printf("\n" ROUGE "Aucun credit trouve. Statistiques non disponibles." RESET "\n");
        return;
    }
    StatsCredit stats = calculerStatistiquesCredit(head);

    printf("\n" GRAS "============================================" RESET);
    printf("\n" CYAN "      ANALYSE GLOBALE DU PORTEFEUILLE CREDIT          " RESET);
    printf("\n" GRAS "============================================" RESET);
    printf("\n" GRAS " --- Vue Generale ---" RESET);
    printf("\n Total des Credits enregistres : %d", stats.totalCredits);
    printf("\n Credits " VERT "ACTIFS" RESET "                 : %d", stats.creditsActifs);
    printf("\n Credits " ROUGE "CLOTURES" RESET "               : %d", stats.creditsClotures);
    printf("\n" GRAS " --------------------------------------------" RESET);
    printf("\n" CYAN " --- Indicateurs Financiers ---" RESET);
    printf("\n Montant total emis            : " BLANC_GRAS "%.2lf DH" RESET, stats.montantTotalEmis);
    printf("\n Montant total restant         : " ROUGE "%.2lf DH" RESET, stats.montantTotalRestant);
    printf("\n Mensualite totale attendue    : %.2lf DH", stats.mensualiteTotaleMensuelle);
    printf("\n Taux d'interet moyen          : %.2lf %%", stats.tauxMoyen);
    printf("\n Ratio de Remboursement Global : " VERT "%.2lf %%" RESET, stats.ratioRemboursement);
    printf("\n" GRAS "============================================" RESET "\n");

    libererCredits(head);
}

/* =============== PARTIE 2: STATISTIQUES DEMANDES =============== */

static StatsDemandes calculerStatistiquesDemandes(DemandeNode *head) {
    StatsDemandes stats = {0};
    if (!head) return stats;
    DemandeNode *p;
    for (p = head; p; p = p->suiv) {
        Demande *d = &p->demande;
        stats.totalDemandes++;
        stats.montantTotalDemande += d->montant;
        if (d->statut == STATUT_EN_ATTENTE) stats.demandesEnAttente++;
        else if (d->statut == STATUT_ACCEPTEE) stats.demandesAcceptees++;
        else stats.demandesRefusees++;
    }
    if (stats.totalDemandes > 0) {
        stats.tauxAcceptation = (double)stats.demandesAcceptees / (double)stats.totalDemandes * 100.0;
    }
    return stats;
}

void afficherStatistiquesDemandes(void) {
    DemandeNode *head = chargerDemandes();
    if (!head) {
        printf("\n" ROUGE "Aucune demande trouvee." RESET "\n");
        return;
    }
    StatsDemandes stats = calculerStatistiquesDemandes(head);

    printf("\n"GRAS "============================================" RESET);
    printf("\n"CYAN "        ANALYSE DE L EFFICACITE DES DEMANDES            " RESET);
    printf("\n"GRAS "============================================" RESET);
    printf("\n Total des Demandes reçues     : %d", stats.totalDemandes);
    printf("\n Demandes " VERT "Acceptees" RESET "            : %d", stats.demandesAcceptees);
    printf("\n Demandes " ROUGE "Refusees" RESET "             : %d", stats.demandesRefusees);
    printf("\n Demandes " BLANC_GRAS "EN ATTENTE" RESET "           : %d", stats.demandesEnAttente);
    printf("\n" GRAS " ------------------------------------------" RESET);
    printf("\n Montant total demande         : " BLANC_GRAS "%.2lf DH" RESET, stats.montantTotalDemande);
    printf("\n" GRAS " TAUX D'ACCEPTATION GLOBAL     : " VERT "%.2lf %%" RESET, stats.tauxAcceptation);
    printf("\n" GRAS "===========================================" RESET "\n");

    libererDemandes(head);
}

/* =============== PARTIE 3: STATISTIQUES PAIEMENTS =============== */

void afficherStatistiquesPaiements(void) {
    PaiementNode *headPaiements = chargerPaiementsFichier();
    StatsPaiement stats = {0};
    if (!headPaiements) {
        printf("\n" ROUGE "Aucun paiement trouve." RESET "\n");
        return;
    }
    PaiementNode *p = headPaiements;
    while(p) {
        stats.totalPaiementsPrevu++;
        if (p->data.statut == STATUT_PAYE) {
            stats.paiementsReussis++;
            stats.montantTotalPaye += p->data.montant;
        } else if (p->data.statut == STATUT_ECHEC) {
            stats.paiementsEchoues++;
            stats.montantTotalEchoue += p->data.montant;
            stats.penalitesCumulees += p->data.montantPenalite;
        }
        p = p->next;
    }

    printf("\n" GRAS "===================================================" RESET);
    printf("\n" CYAN "         ANALYSE DES PERFORMANCES PAIEMENT             " RESET);
    printf("\n" GRAS "===================================================" RESET);
    printf("\n Total des Paiements prevus    : %d", stats.totalPaiementsPrevu);
    printf("\n Paiements " VERT "Reussis" RESET "             : %d", stats.paiementsReussis);
    printf("\n Paiements en " ROUGE "Echec/Retard" RESET "     : %d", stats.paiementsEchoues);
    printf("\n" GRAS " --------------------------------------------------" RESET);
    printf("\n Montant total paye            : " VERT "%.2lf DH" RESET, stats.montantTotalPaye);
    printf("\n Montant total à recouvrer     : " ROUGE "%.2lf DH" RESET, stats.montantTotalEchoue);
    printf("\n Penalites cumulees            : " JAUNE "%.2lf DH" RESET, stats.penalitesCumulees);
    printf("\n" GRAS "==================================================" RESET "\n");

    libererListePaiements(headPaiements);
}

/* =============== PARTIE 4: STATISTIQUES CLIENTS =============== */

static StatsClient calculerStatistiquesPourClient(CreditNode *head, int idClient) {
    StatsClient stats = {0};
    if (!head) return stats;
    CreditNode *p;
    for (p = head; p; p = p->suiv) {
        Credit *c = &p->credit;
        if (c->idClient == idClient) {
            stats.creditsTotal++;
            stats.montantEmprunteTotal += c->montant;
            stats.montantRestantClient += c->montantRestant;
            stats.mensualiteTotaleClient += c->mensualite;
            if (c->etat == ETAT_ACTIF) stats.creditsActifs++;
        }
    }
    return stats;
}

void afficherStatistiquesClient(int idClient) {
    CreditNode *head = chargerCredits();
    if (!head) {
        printf("\n" ROUGE "Aucun credit trouve." RESET "\n");
        return;
    }
    StatsClient stats = calculerStatistiquesPourClient(head, idClient);

    printf("\n" GRAS "-------------------------------------------" RESET);
    printf("\n" CYAN "     RESUME FINANCIER (Client %d)          " RESET, idClient);
    printf("\n"GRAS "-------------------------------------------" RESET);

    if (stats.creditsTotal == 0) {
        printf("\n" JAUNE " Aucun historique de credit pour ce client." RESET "\n");
    } else {
        printf("\n Nombre total de credits       : %d", stats.creditsTotal);
        printf("\n Credits Actifs                : " VERT "%d" RESET, stats.creditsActifs);
        printf("\n"GRAS " ------------------------------------------" RESET);
        printf("\n Montant emprunte total        : %.2lf DH", stats.montantEmprunteTotal);
        printf("\n Reste a payer                 : " ROUGE "%.2lf DH" RESET, stats.montantRestantClient);
        printf("\n Mensualite totale             : %.2lf DH", stats.mensualiteTotaleClient);
        printf("\n" GRAS " ------------------------------------------" RESET "\n");
    }
    libererCredits(head);
}

/* =============== MENU STATISTIQUES =============== */

void sousmenuStatistique(void){
    int choix;
    do {

        printf("\n" GRAS "============================================" RESET);
        printf("\n" CYAN "          MENU STATISTIQUES                 " RESET);
        printf("\n" GRAS "============================================" RESET);
        printf("\n== 1. Statistiques Credits (Globaux)        ==");
        printf("\n== 2. Statistiques Paiements                ==");
        printf("\n== 3. Statistiques Demandes                 ==");
        printf("\n== 4. Statistiques Comptes                  ==");
        printf("\n== 5. Analyse par Client (Individuelle)     ==");
        printf("\n==6. " ROUGE "Retour au Menu Admin" RESET "             ==");
        printf("\n" GRAS "============================================" RESET);
        printf("\n" JAUNE "Votre choix : " RESET);

        if (scanf("%d", &choix) != 1) { while(getchar()!='\n'); choix = 0; }
        while(getchar()!='\n');

        switch (choix) {
            case 1: afficherStatistiquesCredit(); break;
            case 2: afficherStatistiquesPaiements(); break;
            case 3: afficherStatistiquesDemandes(); break;
            case 4: afficherStatistiquesComptes(); break;
            case 5: {
                int idClient;
                printf(JAUNE "Entrez l'ID du Client : " RESET);
                if (scanf("%d", &idClient) == 1) afficherStatistiquesClient(idClient);
                while(getchar()!='\n');
                break;
            }
            case 6: printf(VERT "Retour...\n" RESET); break;
            default: printf(ROUGE "Option invalide.\n" RESET); break;
        }
    } while (choix != 6);
}
