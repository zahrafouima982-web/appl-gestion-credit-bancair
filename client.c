#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "client.h"
#include "compteClient.h"
#include "credit.h"
#include "paiement.h"
#include "style.h"
#define FILENAME "client(1).txt"

ClientNode* creerNoeud(Client c) {
    ClientNode* nouveau = (ClientNode*) malloc(sizeof(ClientNode));
    nouveau->data = c;
    nouveau->next = NULL;
    return nouveau;
}

int tailleListe(ClientNode *tete) {
    int n = 0;
    while (tete != NULL) {
        n++;
        tete = tete->next;
    }
    return n;
}

void insererEnfin(ClientNode **tete, Client c) {
    ClientNode *autre = creerNoeud(c);
    if (*tete == NULL) {
        *tete = autre;
        return;
    }
    ClientNode *temp = *tete;
    while (temp->next != NULL)
        temp = temp->next;
    temp->next = autre;
}

//------- Liste des clients ajoutés dans cette session -------
ClientNode *teteAjout = NULL;

//------- Afficher les clients ajoutés dans cette session -------
void afficherListeClientsAjout(ClientNode *tete) {
    if (tete == NULL) {
        printf("\n" GRAS "Aucun nouveau client ajoute dans cette session." RESET "\n");
        return;
    }

    printf("\n" BLANC_GRAS "=== Clients nouvellement ajoutes ===" RESET "\n");
    printf(BLEU "%-8s %-12s %-15s %-15s %-15s %-25s" RESET "\n",
           "ID", "CIN", "Nom", "Prenom", "Telephone", "Email");
    printf("------------------------------------------------------------------------------------------\n");

    while (tete != NULL) {
        printf("%-8d %-12s %-15s %-15s %-15s %-25s\n",
               tete->data.ID, tete->data.cin, tete->data.nom,
               tete->data.prenom, tete->data.telephone, tete->data.email);
        tete = tete->next;
    }
}

ClientNode* listerClientsFichier() {
    FILE *f = fopen(FILENAME, "r");
    if (!f) return NULL;
    Client c;
    ClientNode *tete = NULL;
    while (fscanf(f, "%d %19s %49s %49s %10s %49s",
                  &c.ID, c.cin, c.nom, c.prenom, c.telephone, c.email) == 6) {
        insererEnfin(&tete, c);
    }
    fclose(f);
    return tete;
}

void libererListe(ClientNode *tete) {
    ClientNode *temp;
    while (tete != NULL) {
        temp = tete;
        tete = tete->next;
        free(temp);
    }
}

void listerClients(void) {
    ClientNode *liste = listerClientsFichier();
    ClientNode *temp = liste;
    if (liste == NULL) {
        printf(ROUGE "Aucun client trouve !" RESET "\n");
        return;
    }
    printf("\n" CYAN "========================== LISTE DES CLIENTS ====================" RESET "\n");
    printf(CYAN "%-8s %-12s %-15s %-15s %-15s %-25s" RESET "\n",
           "ID", "CIN", "Nom", "Prenom", "Telephone", "Email");
    printf("---------------------------------------------------------------------------\n");
    while (temp != NULL) {
       printf("%-8d %-12s %-15s %-15s %-15s %-25s\n",
           temp->data.ID, temp->data.cin, temp->data.nom,
           temp->data.prenom, temp->data.telephone, temp->data.email);
        temp = temp->next;
    }
    libererListe(liste);
}

int comparerID(ClientNode *liste, int ID) {
    while (liste != NULL) {
        if (liste->data.ID == ID) return 1;
        liste = liste->next;
    }
    return 0;
}

int validerTelephone(char *telephone) {
    char temp[20];
    strcpy(temp, telephone);
    int j = 0, i;
    for ( i = 0; temp[i]; i++) {
        if (temp[i] != ' ') temp[j++] = temp[i];
    }
    temp[j] = '\0';

    if (temp[0] == '+' && strncmp(temp, "+212", 4) == 0) {
        if (strlen(temp) != 13) {
            printf(ROUGE "Erreur : format +212 doit être suivi de 9 chiffres." RESET "\n");
            return 0;
        }
        for (i = 4; i < 13; i++) {
            if (!isdigit(temp[i])) {
                printf(ROUGE "Erreur : format invalide apres +212." RESET "\n");
                return 0;
            }
        }
        temp[0] = '0';
        memmove(temp + 1, temp + 4, 9);
        temp[10] = '\0';
        strcpy(telephone, temp);
        return 1;
    }

    if (strlen(temp) == 10 && temp[0] == '0' && (temp[1]=='5' || temp[1]=='6' || temp[1]=='7')) {
        for ( i = 0; i < 10; i++) {
            if (!isdigit(temp[i])) {
                printf(ROUGE "Erreur : uniquement des chiffres autorisés." RESET "\n");
                return 0;
            }
        }
        strcpy(telephone, temp);
        return 1;
    }
    printf(ROUGE "Erreur : Numéro marocain invalide (ex: 06XXXXXXXX)." RESET "\n");
    return 0;
}

int verifierEmail(const char *email) {
    int len = strlen(email);
    if (len < 6 || len > 50) { printf(ROUGE "Erreur : email trop court/long." RESET "\n"); return 0; }
    const char *at = strchr(email, '@');
    const char *dot = strrchr(email, '.');
    if (!at || !dot || dot < at) { printf(ROUGE "Erreur : format email (@/.) invalide." RESET "\n"); return 0; }
    return 1;
}

void ajouterClient(void) {
    printf("\n" CYAN "====== Ajouter un nouveau client ======" RESET "\n");
    FILE *f = fopen(FILENAME, "a+");
    if (!f) { printf(ROUGE "Erreur d'acces au fichier." RESET "\n"); return; }

    ClientNode *ListeFichier = listerClientsFichier();
    char reponse[4];
    do {
        Client c;
        do {
            printf(GRAS "ID_Client : " RESET);
            scanf("%d", &c.ID);
            if (comparerID(ListeFichier, c.ID) || comparerID(teteAjout, c.ID)) {
                printf(ROUGE "Erreur : Cet ID existe deja !" RESET "\n");

            }
            break;
        } while (1);

        printf("CIN       : "); scanf(" %19s", c.cin);
        printf("Nom       : "); scanf(" %49s", c.nom);
        printf("Prenom    : "); scanf(" %49s", c.prenom);

        do {
            printf("Telephone : "); scanf(" %15s", c.telephone);
        } while (!validerTelephone(c.telephone));

        do {
            printf("Email     : "); scanf(" %49s", c.email);
        } while (!verifierEmail(c.email));

        insererEnfin(&teteAjout, c);
        fprintf(f, "%d %s %s %s %s %s\n", c.ID, c.cin, c.nom, c.prenom, c.telephone, c.email);
        printf(VERT "Client ajoute en memoire !" RESET "\n");
        printf("Ajouter un autre client ? (oui/non) : ");
        scanf("%3s", reponse);
    } while (strcmp(reponse, "oui") == 0);

    fclose(f);
    printf("\n" BLANC_GRAS " Toutes les donnees ont ete sauvegardees." RESET "\n");
    libererListe(ListeFichier);
}

void modifierClient(void) {
    int id, choix, trouve = 0;
    printf(GRAS "Donner l'ID du client a modifier : " RESET);
    scanf("%d", &id);
    ClientNode *liste = listerClientsFichier();
    if (liste == NULL) { printf(ROUGE "Aucun client trouve." RESET "\n"); return; }

    ClientNode *temp = liste;
    while (temp != NULL) {
        if (temp->data.ID == id) {
            trouve = 1;
            printf("\n" CYAN "Donnees actuelles :" RESET "\n");
            printf("Nom: %s | Prenom: %s | CIN: %s\n", temp->data.nom, temp->data.prenom, temp->data.cin);

            printf("\n1. Nom | 2. Prenom | 3. Telephone | 4. Email | 5. Tous | 6. CIN\n");
            printf(GRAS "Votre choix : " RESET);
            scanf("%d", &choix);

            switch (choix) {
                case 1: printf("Nouveau nom : "); scanf(" %49s", temp->data.nom); break;
                case 2: printf("Nouveau prenom : "); scanf(" %49s", temp->data.prenom); break;
                case 3:
                    do { printf("Nouveau Tel : "); scanf(" %15s", temp->data.telephone);
                    } while(!validerTelephone(temp->data.telephone)); break;
                case 4:
                    do { printf("Nouvel Email : "); scanf(" %49s", temp->data.email);
                    } while(!verifierEmail(temp->data.email)); break;
                case 6: printf("Nouveau CIN : "); scanf(" %19s", temp->data.cin); break;
                case 5:
                    printf("Nom : "); scanf(" %49s", temp->data.nom);
                    printf("Prenom : "); scanf(" %49s", temp->data.prenom);
                    printf("CIN : "); scanf(" %19s", temp->data.cin);
                    break;
            }
            break;
        }
        temp = temp->next;
    }

    if (trouve) {
        FILE *f = fopen(FILENAME, "w");
        temp = liste;
        while (temp) {
            fprintf(f, "%d %s %s %s %s %s\n", temp->data.ID, temp->data.cin, temp->data.nom, temp->data.prenom, temp->data.telephone, temp->data.email);
            temp = temp->next;
        }
        fclose(f);
        printf(VERT "✓ Modification réussie !" RESET "\n");
    } else printf(ROUGE "Client introuvable." RESET "\n");
    libererListe(liste);
}

void supprimerClient(void) {
    int id; char reponse[4]; int trouve = 0;
    printf(GRAS "ID du client a supprimer : " RESET);
    scanf("%d", &id);
    ClientNode *liste = listerClientsFichier();
    ClientNode *temp = liste, *prev = NULL;

    while (temp) {
        if (temp->data.ID == id) {
            trouve = 1;
            printf(ROUGE "ATTENTION: Vous allez supprimer le client %s %s." RESET "\n", temp->data.nom, temp->data.prenom);
            printf("Confirmer ? (oui/non) : "); scanf("%3s", reponse);
            if (strcmp(reponse, "oui") != 0) { libererListe(liste); return; }
            if (prev == NULL) liste = temp->next;
            else prev->next = temp->next;
            free(temp);
            break;
        }
        prev = temp; temp = temp->next;
    }

    if (trouve) {
        FILE *f = fopen(FILENAME, "w");
        temp = liste;
        while (temp) {
            fprintf(f, "%d %s %s %s %s %s\n", temp->data.ID, temp->data.cin, temp->data.nom, temp->data.prenom, temp->data.telephone, temp->data.email);
            temp = temp->next;
        }
        fclose(f);
        supprimerCompte(id);
        printf(GRAS " Client et compte associe supprimes." RESET "\n");
    } else printf(ROUGE "ID inexistant." RESET "\n");
    libererListe(liste);
}

void afficherClientNode(ClientNode *node) {
    if (!node) return;
    printf(CYAN "%-8s %-12s %-15s %-15s %-15s %-25s" RESET "\n", "ID", "CIN", "Nom", "Prenom", "Tel", "Email");
    printf("%-8d %-12s %-15s %-15s %-15s %-25s\n",
           node->data.ID, node->data.cin, node->data.nom, node->data.prenom, node->data.telephone, node->data.email);
}

void rechercherClient(void){
    ClientNode *liste = listerClientsFichier();
    if (!liste) { printf(ROUGE "Base vide." RESET "\n"); return; }
    int choix, trouve = 0, idRech; char strRech[50];
    printf("\n" CYAN "=== Recherche ===" RESET "\n1. ID | 2. Nom | 3. Prenom | 6. CIN\nChoix: ");
    scanf("%d", &choix);
    ClientNode *temp = liste;
    if (choix == 1) {
        printf("ID: "); scanf("%d", &idRech);
        while (temp) { if (temp->data.ID == idRech) { trouve = 1; afficherClientNode(temp); break; } temp = temp->next; }
    } else {
        printf("Valeur: "); scanf("%s", strRech);
        while (temp) {
            if ((choix==2 && strcmp(temp->data.nom, strRech)==0) || (choix==6 && strcmp(temp->data.cin, strRech)==0)) {
                trouve = 1; afficherClientNode(temp); break;
            }
            temp = temp->next;
        }
    }
    if (!trouve) printf(ROUGE "Aucun resultat." RESET "\n");
    libererListe(liste);
}
// trier le fichier des clients selon un critère
int comparerClients(Client a, Client b, int critere) {
    switch (critere) {
        case 1:  // Tri par ID
            return a.ID - b.ID;

        case 2:  // Tri par Nom alphabétique
            return strcmp(a.nom, b.nom);

        case 3:  // Tri par Prénom alphabétique
            return strcmp(a.prenom, b.prenom);

        default:
            return 0;
    }
}

// Fonction d'insertion pour le tri par insertion
ClientNode* insererTrieCritere(ClientNode *listeTriee, ClientNode *nouvelElement, int critere) {
    // Cas 1 : liste triée vide OU nouvel élément < premier élément
    if (listeTriee == NULL || comparerClients(nouvelElement->data, listeTriee->data, critere) < 0) {
        nouvelElement->next = listeTriee;
        return nouvelElement;
    }

    ClientNode *temp = listeTriee;

    // Avancer jusqu'à trouver la bonne position
    while (temp->next != NULL && comparerClients(nouvelElement->data, temp->next->data, critere) >= 0) {
        temp = temp->next;
    }

    // Insertion dans la liste
    nouvelElement->next = temp->next;
    temp->next = nouvelElement;

    return listeTriee;
}
void saisirChaine(char *buffer, int taille) {
    if (fgets(buffer, taille, stdin) != NULL) {
        buffer[strcspn(buffer, "\n")] = '\0'; // Enlever le retour à la ligne
    }
}
Client chercherClientParID(int idClient) {
    ClientNode *liste = listerClientsFichier();
    Client c = {0}; // ID = 0 signifie "non trouvé"

    ClientNode *temp = liste;
    while (temp != NULL) {
        if (temp->data.ID == idClient) {
            c = temp->data;
            break;
        }
        temp = temp->next;
    }

    libererListe(liste);
    return c;
}

void trierClients(void) {
    ClientNode *liste = listerClientsFichier();
    if (!liste) return;
    int critere;
    printf("\n" BLEU "=== Tri ===" RESET "\n1. ID | 2. Nom | 3. Prenom\nChoix: ");
    scanf("%d", &critere);
    if (critere < 1 || critere > 3) { libererListe(liste); return; }

    ClientNode *listeTriee = NULL, *temp;
    while (liste) {
        temp = liste; liste = liste->next; temp->next = NULL;
        listeTriee = insererTrieCritere(listeTriee, temp, critere);
    }

    FILE *f = fopen(FILENAME, "w");
    temp = listeTriee;
    while (temp) {
        fprintf(f, "%d %s %s %s %s %s\n", temp->data.ID, temp->data.cin, temp->data.nom, temp->data.prenom, temp->data.telephone, temp->data.email);
        temp = temp->next;
    }
    fclose(f);
    printf(VERT "✓ Tri terminé et sauvegardé." RESET "\n");
    libererListe(listeTriee);
}

void menuGestionClients(void) {
    int choix;
    do {
        clear();

        // --- EN-TÊTE CYAN (Identique au menu Crédit) ---
        printf(CYAN);
        espaceCentre(); printf("█▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀█\n");
        espaceCentre(); printf("█           GESTION DES CLIENTS          █\n");
        espaceCentre(); printf("█▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄█\n");
        printf(RESET);

        printf("\n");

        // --- OPTIONS EN BLANC ---
        printf(BLANC_GRAS);
        espaceCentre(); printf("   [1]  Ajouter un nouveau client\n");
        espaceCentre(); printf("   [2]  Modifier les informations client\n");
        espaceCentre(); printf("   [3]  Supprimer un client du systeme\n");
        espaceCentre(); printf("   [4]  Rechercher un client\n");
        espaceCentre(); printf("   [5]  Lister tous les clients\n");
        espaceCentre(); printf("   [6]  Trier la base de donnees clients\n");

        // Retour en Cyan
        printf(CYAN);
        printf("\n");
        espaceCentre(); printf("   [0]  RETOUR AU MENU ADMIN\n");
        printf(RESET);

        // --- LIGNE DE SEPARATION ---
        printf(CYAN);
        printf("\n");
        espaceCentre(); printf("──────────────────────────────────────────\n");
        printf(BLANC_GRAS);
        espaceCentre(); printf("   ➤  votre choix : ");
        printf(RESET);

        if (scanf("%d", &choix) != 1) {
            while (getchar() != '\n');
            choix = -1;
        }


        switch (choix) {
            case 1: ajouterClient(); break;
            case 2: modifierClient(); break;
            case 3: supprimerClient(); break;
            case 4: rechercherClient(); break;
            case 5: listerClients(); break;
            case 6: trierClients(); break;
            case 0: break; // Retour
            default:
                printf(ROUGE "\n      [!] Option invalide.\n" RESET);
                printf("      Appuyez sur Entrer...");
                getchar();
                break;
        }
    } while (choix != 0);
}
