
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <windows.h>
// Headers des modules
#include "demande.h"
#include "credit.h"
#include "statistique.h"
#include "paiement.h"
#include "espaceClientComplet.h"
#include "espaceAdmin.h"
#include "client.h"
#include "compteClient.h"
#include "style.h"


void menuClient(void);

int main() {

    int choix;

    while(1) {
        SetConsoleOutputCP(CP_UTF8);
        clear();

        logoBanque();

        choix = ecranConnexion();

        switch(choix) {

            case 1:
                clear();
                logoAdmin();
                espaceAdminComplet();
                break;

            case 2:
                clear();

                menuClient();
                break;

            case 0:
                printf(ROUGE "\nFermeture du programme...\n" RESET);
                return 0;

            default:
                printf(ROUGE "\nChoix invalide\n" RESET);
        }

        printf("\nAppuyez sur ENTER pour continuer...");
        getchar();
        getchar();
    }

    return 0;
}

    // ==========================
// Menu Client
// ==========================
void menuClient(void) {
    CompteClientNode *compteConnecte;
    int choix;

    compteConnecte = espaceClientComplet();
    if (!compteConnecte) {
        printf(RED "Échec de la connexion. Retour au menu principal.\n" RESET);
        return;
    }

    /* ===== AFFICHAGE DU MENU ===== */
    printf(L_GREEN BOLD "\n--- CLIENT ID %d : Menu ---\n" RESET, compteConnecte->compte.client.ID);
    printf(YELLOW "1. Soumettre une nouvelle demande\n" RESET);
    printf(YELLOW "2. Voir l'historique de mes demandes\n" RESET);
    printf(YELLOW "3. Voir mes credits actifs\n" RESET);
    printf(YELLOW "4. Voir mon resume financier (Statistiques)\n" RESET);
    printf(YELLOW "5. Simuler mon credit\n" RESET);
    printf(YELLOW "6. Voir les offres disponibles\n" RESET);
    printf(YELLOW "7. Modifier mes informations personnelles\n" RESET);
    printf(YELLOW "8. Retour au Menu Principal\n" RESET);
    printf("Choix: ");

    if (scanf("%d", &choix) != 1) {
        while (getchar() != '\n');
        free(compteConnecte);
        return;
    }
    while (getchar() != '\n');

    /* ===== Boucle d'attente pour retour ===== */
    do {
        printf(YELLOW "\n8. Retour au Menu Principal\n" RESET);
        printf("Choix: ");
        if (scanf("%d", &choix) != 1) {
            while(getchar() != '\n');
            choix = 0;
        }
        while(getchar() != '\n');
    } while (choix != 8);

    printf(GREEN BOLD "Deconnexion Client.\n" RESET);
    free(compteConnecte);
}
