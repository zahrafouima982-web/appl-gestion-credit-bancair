
#include "espaceAdmin.h"
#include "compteClient.h"
#include "client.h"
#include "credit.h"
#include "style.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "demande.h"
#include "statistique.h"

#define ADMIN_ID "admin"
#define ADMIN_PASSWORD "admin123"
#define ADMIN_FILE "admin.txt"

// Fonction pour charger les identifiants admin depuis un fichier
int chargerIdentifiantsAdmin(char *id, char *password, int taille) {
    FILE *f = fopen(ADMIN_FILE, "r");
    if (!f) {
        f = fopen(ADMIN_FILE, "w");
        if (f) {
            fprintf(f, "%s\n%s\n", ADMIN_ID, ADMIN_PASSWORD);
            fclose(f);
        }
        strcpy(id, ADMIN_ID);
        strcpy(password, ADMIN_PASSWORD);
        return 1;
    }

    if (fgets(id, taille, f)) {
        id[strcspn(id, "\n")] = 0;
        if (fgets(password, taille, f)) {
            password[strcspn(password, "\n")] = 0;
            fclose(f);
            return 1;
        }
    }
    fclose(f);
    return 0;
}

// Fonction d'authentification admin
int authentifierAdmin(void) {
    char idSaisi[50];
    char passwordSaisi[50];
    char idAdmin[50];
    char passwordAdmin[50];


    espaceCentre(); printf("  \n               Bienvenue dans l'espace Admin            \n");




    chargerIdentifiantsAdmin(idAdmin, passwordAdmin, sizeof(idAdmin));

     espaceCentre();printf(GRAS "ID Admin : " RESET);
    if (scanf("%49s", idSaisi) != 1) {
        while(getchar() != '\n');
        return 0;
    }
    while(getchar() != '\n');

     espaceCentre();printf(GRAS"Mot de passe : " RESET);
    // Note : Dans un vrai projet, on masquerait les caractères ici
    if (scanf("%49s", passwordSaisi) != 1) {
        while(getchar() != '\n');
        return 0;
    }
    while(getchar() != '\n');

    if (strcmp(idSaisi, idAdmin) == 0 && strcmp(passwordSaisi, passwordAdmin) == 0) {
        printf("\n" BLANC_GRAS " Authentification reussie ! Bienvenue." RESET "\n");
        return 1;
    } else {
        printf("\n" ROUGE " Echec de l'authentification. ID ou mot de passe incorrect." RESET "\n");
        return 0;
    }
}

void menuAdmin(void) {

    int idClient = 0, modeAdmin = 1;
    char choix;

    do {

        clear();

printf(CYAN);

    espaceCentre(); printf("==========================================================================\n");
    espaceCentre(); printf("                             Gestion de systeme bancaire               \n");
    espaceCentre(); printf("==========================================================================\n");

    printf(RESET);
        printf(BLANC_GRAS);

        espaceCentre(); printf("                   +--------------------------------------+\n");
        espaceCentre(); printf("                   |  1.👤  Gestion des clients           |\n");
        espaceCentre(); printf("                   |  2.💳  Gestion des comptes clients   |\n");
        espaceCentre(); printf("                   |  3.💰  Gestion des credits           |\n");
        espaceCentre(); printf("                   |  4.💵  Gestion des paiements         |\n");
        espaceCentre(); printf("                   |  5.📄  Gestion des demandes          |\n");
        espaceCentre(); printf("                   |  6.📊  Statistiques                  |\n");
        espaceCentre(); printf("                   |  7.💾  Sauvegarde / Restauration     |\n");
        espaceCentre(); printf("                   |  0.🔙  Quitter                       |\n");
        espaceCentre(); printf("                   +--------------------------------------+\n");

        printf(RESET);

        printf("\n");

        espaceCentre(); printf("            ➤ Votre choix : ");

        scanf(" %c",&choix);
        while(getchar()!='\n');

        switch(choix) {

            case '1': menuGestionClients(); break;
            case '2': menuGestionComptes(); break;
            case '3': menuGestionCredits(); break;
            case '4': menuGestionPaiements(); break;
            case '5': menuDemandes(idClient, modeAdmin); break;
            case '6': sousmenuStatistique(); break;

            case '7': {
                CompteClientNode *comptes = chargerComptesFichier();

                if(sauvegarderComptesFichier(comptes))
                    printf(VERT "Sauvegarde reussie\n" RESET);
                else
                    printf(ROUGE "Erreur sauvegarde\n" RESET);

                libererListeComptes(comptes);
                break;
            }

            case '0':
                espaceCentre(); printf("Retour menu principal\n");
                break;

            default:
               espaceCentre(); printf(ROUGE "Choix invalide\n" RESET);
        }

    } while(choix!='0');
}
void espaceAdminComplet(void) {
    if (!authentifierAdmin()) {
        printf(ROUGE "Acces refuse. Retour au menu principal." RESET "\n");
        return;
    }
    menuAdmin();
}
