#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "demande.h"
#include "credit.h"
#include "style.h"

/* Utilitaires */
static void dateCourante(char *buf, int sz) {
    time_t t = time(NULL);
    struct tm *tm = localtime(&t);
    snprintf(buf, sz, "%02d/%02d/%04d", tm->tm_mday, tm->tm_mon + 1, tm->tm_year + 1900);
}

static int prochainIdDemande(void) {
    DemandeNode *head = chargerDemandes();
    int max = 0;
    DemandeNode *p;
    for (p= head; p; p = p->suiv) if (p->demande.idDemande > max) max = p->demande.idDemande;
    libererDemandes(head);
    return max + 1;
}

/* Fonctions d'E/S */
DemandeNode* chargerDemandes(void) {
    FILE *f = fopen(DEMANDE_FILE, "rb");
    if (!f) return NULL;
    fseek(f, 0, SEEK_END);
    long size = ftell(f);
    if (size <= 0) { fclose(f); return NULL; }
    rewind(f);
    size_t count = (size_t)(size / sizeof(Demande));
    Demande *arr = malloc(count * sizeof(Demande));
    if (!arr) { fclose(f); return NULL; }
    if (fread(arr, sizeof(Demande), count, f) != count) { free(arr); fclose(f); return NULL; }
    fclose(f);

    DemandeNode *head = NULL;
    size_t i;
    for (i = 0; i < count; ++i) {
        DemandeNode *n = malloc(sizeof(DemandeNode));
        n->demande = arr[i]; n->suiv = head; head = n;
    }
    free(arr);
    return head;
}

int sauvegarderDemandes(DemandeNode *head) {
    FILE *f = fopen(DEMANDE_FILE, "wb");
    if (!f) return 0;
    size_t count = 0;
    DemandeNode *p;
    for (p = head; p; p = p->suiv) count++;
    if (count == 0) { fclose(f); return 1; }
    Demande *arr = malloc(count * sizeof(Demande));
    if (!arr) { fclose(f); return 0; }
    size_t idx = 0;

    for (p = head; p; p = p->suiv) arr[idx++] = p->demande;
    int ok = (fwrite(arr, sizeof(Demande), count, f) == count);
    free(arr);
    fclose(f);
    return ok ? 1 : 0;
}

void libererDemandes(DemandeNode *head) {
    while (head) {
        DemandeNode *tmp = head->suiv;
        free(head);
        head = tmp;
    }
}

/* ---------- Fonctions CLIENT ---------- */

int envoyerDemande_interactif(int idClient) {
    Demande d;
    d.idDemande = prochainIdDemande();
    d.idClient = idClient;

    printf("\n" CYAN "==== Nouvelle Demande (ID=%d) ====" RESET "\n", d.idDemande);
    printf("Montant demande (DH) : ");
    if (scanf("%lf", &d.montant) != 1 || d.montant <= 0) { while(getchar()!='\n'); printf(ROUGE "Montant invalide.\n" RESET); return -1; }
    getchar();

    printf("Duree (mois) : ");
    if (scanf("%d", &d.duree) != 1 || d.duree <= 0) { while(getchar()!='\n'); printf(ROUGE "Duree invalide.\n" RESET); return -1; }
    getchar();

    d.taux = 0.0;
    d.statut = STATUT_EN_ATTENTE;
    dateCourante(d.dateDemande, DATE_SIZE);

    DemandeNode *head = chargerDemandes();
    DemandeNode *n = malloc(sizeof(DemandeNode));
    n->demande = d; n->suiv = head; head = n;
    if (!sauvegarderDemandes(head)) { printf(ROUGE "Erreur sauvegarde demande.\n" RESET); libererDemandes(head); return -1; }
    printf(VERT "Demande enregistree (ID=%d) - en attente de traitement.\n" RESET, d.idDemande);
    libererDemandes(head);
    return d.idDemande;
}

void afficherHistoriqueDemandesClient(DemandeNode *head, int idClient) {
    int local = 0;
    if (!head) { head = chargerDemandes(); local = 1; }
    if (!head) { printf(JAUNE "Aucune demande enregistree.\n" RESET); return; }

    printf("\n" BLEU "==== Historique des demandes pour Client ID %d ====" RESET "\n", idClient);
    int found = 0;
    DemandeNode *p;
    for (p = head; p; p = p->suiv) {
        if (p->demande.idClient == idClient) {
            Demande *d = &p->demande;
            const char *etat;
            if (d->statut == STATUT_EN_ATTENTE) etat = JAUNE "EN ATTENTE" RESET;
            else if (d->statut == STATUT_ACCEPTEE) etat = VERT "ACCEPTEE" RESET;
            else etat = ROUGE "REFUSEE" RESET;

            printf("ID Demande:%d Montant:%.2lf DH Duree:%d mois Etat:%s Taux Propose:%.2lf%%\n",
                   d->idDemande, d->montant, d->duree, etat, d->taux);
            found = 1;
        }
    }
    if (!found) printf(JAUNE "Aucune demande trouvee pour ce client.\n" RESET);
    if (local) libererDemandes(head);
}

/* ---------- Fonctions ADMIN ---------- */

void afficherDemandes(DemandeNode *head) {
    int local = 0;
    if (!head) { head = chargerDemandes(); local = 1; }
    if (!head) { printf(JAUNE "Aucune demande.\n" RESET); return; }
    printf("\n" BLEU "==== Liste de toutes les demandes ====" RESET "\n");
    DemandeNode *p;
    for (p = head; p; p = p->suiv) {
        Demande *d = &p->demande;
        const char *etat;
        if (d->statut == STATUT_EN_ATTENTE) etat = JAUNE "EN_ATTENTE" RESET;
        else if (d->statut == STATUT_ACCEPTEE) etat = VERT "ACCEPTEE" RESET;
        else etat = ROUGE "REFUSEE" RESET;

        printf("ID:%d Client:%d Montant:%.2lf Duree:%d Taux:%.2lf Etat:%s Date:%s\n",
               d->idDemande, d->idClient, d->montant, d->duree, d->taux, etat, d->dateDemande);
    }
    if (local) libererDemandes(head);
}

void afficherDemandesEnAttente(DemandeNode *head) {
    int local = 0; if (!head) { head = chargerDemandes(); local = 1; }
    if (!head) { printf(JAUNE "Aucune demande.\n" RESET); return; }
    printf("\n" CYAN "==== Demandes en attente de traitement ====" RESET "\n");
    int found = 0;
    DemandeNode *p;
    for (p = head; p; p = p->suiv) {
        if (p->demande.statut == STATUT_EN_ATTENTE) {
            Demande *d = &p->demande;
            printf("ID:" JAUNE "%d" RESET " Client:%d Montant:%.2lf DH Duree:%d mois Date:%s\n",
                   d->idDemande, d->idClient, d->montant, d->duree, d->dateDemande);
            found = 1;
        }
    }
    if (!found) printf(JAUNE "Aucune demande en attente.\n" RESET);
    if (local) libererDemandes(head);
}

int supprimerDemandeParId(DemandeNode **headRef, int id) {
    DemandeNode *head = *headRef;
    DemandeNode *cur = head, *prev = NULL;
    while (cur) {
        if (cur->demande.idDemande == id) {
            if (prev) prev->suiv = cur->suiv; else head = cur->suiv;
            free(cur);
            *headRef = head;
            printf(VERT "Demande %d supprimee.\n" RESET, id);
            return sauvegarderDemandes(head);
        }
        prev = cur; cur = cur->suiv;
    }
    printf(ROUGE "Demande non trouvee.\n" RESET);
    return 0;
}

int traiterDemandesInteractive(void) {
    DemandeNode *head = chargerDemandes();
    if (!head) { printf(JAUNE "Aucune demande a traiter.\n" RESET); return 0; }

    DemandeNode *p = head;
    int updated = 0;
    printf("\n" CYAN "==== Traitement des Demandes en Attente ====" RESET "\n");

    while (p) {
        if (p->demande.statut == STATUT_EN_ATTENTE) {
            Demande *d = &p->demande;
            printf("\nTraitement ID:%d Client:%d Montant:%.2lf DH Duree:%d mois Date:%s\n",
                   d->idDemande, d->idClient, d->montant, d->duree, d->dateDemande);

            printf("Accepter (" VERT "a" RESET ") / Refuser (" ROUGE "r" RESET ") / Sauter (s) ? ");
            char ch = getchar(); while (getchar()!='\n');

            if (ch == 'a' || ch == 'A') {
                double taux;
                do {
                    printf("Entrer taux annuel (%%) propose : ");
                    if (scanf("%lf", &taux) != 1) { while(getchar()!='\n'); taux = 0.0; }
                    getchar();
                } while (taux <= 0.0);

                d->taux = taux;
                d->statut = STATUT_ACCEPTEE;

                if (importerDemandeCommeCredit((const struct Demande*)d)) {
                    printf(VERT "Demande %d acceptee et convertie en credit.\n" RESET, d->idDemande);
                    updated = 1;
                } else {
                    d->statut = STATUT_EN_ATTENTE;
                    printf(ROUGE "ERREUR: Impossible de creer le credit.\n" RESET);
                }
            } else if (ch == 'r' || ch == 'R') {
                d->statut = STATUT_REFUSEE;
                printf(ROUGE "Demande %d refusee.\n" RESET, d->idDemande);
                updated = 1;
            } else { printf("Saut de la demande.\n"); }
        }
        p = p->suiv;
    }

    if (updated) {
        if (sauvegarderDemandes(head)) printf(VERT "\nSauvegarde effectuee.\n" RESET);
    }
    libererDemandes(head);
    return updated;
}

void menuDemandes(void) {
    int choix;
    do {
        clear();
        espaceCentre();cadre();
        espaceCentre();printf("" CYAN "                           📄  GESTION DES DEMANDES        " RESET "\n");
        espaceCentre();cadre();

        espaceCentre(); printf(""BLANC_GRAS"                   +--------------------------------------+"RESET"\n");
        espaceCentre(); printf(""BLANC_GRAS"                   |                                      |"RESET"\n");
        espaceCentre(); printf(""BLANC_GRAS"                   | 1. Afficher les demandes " JAUNE "EN ATTENTE" RESET "  |"RESET"\n");
        espaceCentre(); printf(""BLANC_GRAS"                   | 2. Traiter les demandes EN ATTENTE   |"RESET"\n");
        espaceCentre(); printf(""BLANC_GRAS"                   | 3. Afficher toutes les demandes      |"RESET"\n");
        espaceCentre(); printf(""BLANC_GRAS"                   | 4. Supprimer une demande             |"RESET"\n");
        espaceCentre(); printf(""BLANC_GRAS"                   | 5. Retour au Menu Admin              |"RESET" \n");
        espaceCentre(); printf(""BLANC_GRAS"                   |                                      |"RESET"\n");
        espaceCentre(); printf(""BLANC_GRAS"                   +--------------------------------------+"RESET"\n");

        printf("\n");
         espaceCentre(); printf("            ➤ Votre choix : ");
        if (scanf("%d", &choix) != 1) { while(getchar() != '\n'); choix = 0; }
        while(getchar() != '\n');

        switch (choix) {
            case 1: afficherDemandesEnAttente(NULL); break;
            case 2: traiterDemandesInteractive(); break;
            case 3: afficherDemandes(NULL); break;
            case 4: {
                int id; printf("ID a supprimer : ");
                if (scanf("%d", &id) == 1) {
                    DemandeNode *h = chargerDemandes();
                    supprimerDemandeParId(&h, id);
                    libererDemandes(h);
                }
                while(getchar() != '\n');
                break;
            }
        }
        if (choix != 5) { printf("\nAppuyez sur Entrer..."); getchar(); }
    } while (choix != 5);
}
