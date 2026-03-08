#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

#include "credit.h"
#include "demande.h"
#include "style.h" // Inclusion de ton nouveau fichier de style

/* --------------- helpers I/O et Utilitaires --------------- */

static void dateCourante(char *buf, int sz) {
    time_t t = time(NULL);
    struct tm *tm = localtime(&t);
    snprintf(buf, sz, "%02d/%02d/%04d", tm->tm_mday, tm->tm_mon + 1, tm->tm_year + 1900);
}

int prochainIdCredit(void) {
    CreditNode *head = chargerCredits();
    int max = 0;
    CreditNode *p;
    for (p = head; p; p = p->suiv) if (p->credit.idCredit > max) max = p->credit.idCredit;
    libererCredits(head);
    return max + 1;
}

CreditNode* chargerCredits(void) {
    FILE *f = fopen(CREDIT_FILE, "rb");
    if (!f) return NULL;
    fseek(f, 0, SEEK_END);
    long size = ftell(f);
    if (size <= 0) { fclose(f); return NULL; }
    rewind(f);

    size_t count = (size_t)(size / sizeof(Credit));
    Credit *arr = malloc(count * sizeof(Credit));
    if (!arr) { fclose(f); return NULL; }

    if (fread(arr, sizeof(Credit), count, f) != count) {
        free(arr); fclose(f); return NULL;
    }
    fclose(f);

    CreditNode *head = NULL;
    size_t i;
    for (i = 0; i < count; ++i) {
        CreditNode *n = malloc(sizeof(CreditNode));
        n->credit = arr[i];
        n->suiv = head;
        head = n;
    }
    free(arr);
    return head;
}

int sauvegarderCredits(CreditNode *head) {
    FILE *f = fopen(CREDIT_FILE, "wb");
    if (!f) return 0;
    size_t count = 0;
    CreditNode *p;
    for (p = head; p; p = p->suiv) count++;
    if (count == 0) { fclose(f); return 1; }

    Credit *arr = malloc(count * sizeof(Credit));
    if (!arr) { fclose(f); return 0; }
    size_t idx = 0;
    for (p = head; p; p = p->suiv) {
        arr[idx++] = p->credit;
    }
    int ok = (fwrite(arr, sizeof(Credit), count, f) == count);
    free(arr);
    fclose(f);
    return ok ? 1 : 0;
}

void libererCredits(CreditNode *head) {
    while (head) {
        CreditNode *tmp = head->suiv;
        free(head);
        head = tmp;
    }
}

/* --------------- Fonctions Financières --------------- */

double calculerMensualite(double montant, double tauxAnnuel, int dureeMois) {
    if (dureeMois <= 0) return 0.0;
    double t = tauxAnnuel / 100.0 / 12.0;
    if (fabs(t) < 1e-12) return montant / (double)dureeMois;
    double denom = 1.0 - pow(1.0 + t, -dureeMois);
    if (fabs(denom) < 1e-12) return montant / (double)dureeMois;
    return (montant * t) / denom;
}

/* --------------- Fonctions ADMIN --------------- */

CreditNode* ajouterCreditNode(CreditNode *head, Credit c) {
    CreditNode *n = malloc(sizeof(CreditNode));
    n->credit = c;
    n->suiv = head;
    return n;
}

int ajouterCreditFromInteractive(void) {
    Credit c;
    c.idCredit = prochainIdCredit();

    clear();
    titre("CREATION D'UN NOUVEAU CREDIT");
    printf(JAUNE "ID Credit genere : %d\n\n" RESET, c.idCredit);

    printf("ID client       : "); if (scanf("%d", &c.idClient) != 1) { while(getchar()!='\n'); printf(ROUGE "Entree invalide.\n" RESET); return 0; } while(getchar()!='\n');
    printf("Montant (DH)    : "); if (scanf("%lf", &c.montant) != 1 || c.montant <= 0) { while(getchar()!='\n'); printf(ROUGE "Montant invalide.\n" RESET); return 0; } while(getchar()!='\n');
    printf("Taux annuel (%%) : "); if (scanf("%lf", &c.taux) != 1 || c.taux < 0) { while(getchar()!='\n');printf(ROUGE "Taux invalide.\n" RESET) ;return 0; } while(getchar()!='\n');
    printf("Duree (mois)    : "); if (scanf("%d", &c.duree) != 1 || c.duree <= 0) { while(getchar()!='\n'); printf(ROUGE "Durée invalide.\n" RESET);return 0; } while(getchar()!='\n');

    c.mensualite = calculerMensualite(c.montant, c.taux, c.duree);
    c.montantRestant = c.montant;
    dateCourante(c.dateCreation, DATE_SIZE);
    dateCourante(c.date_debut, DATE_SIZE);
    c.etat = ETAT_ACTIF;

    CreditNode *head = chargerCredits();
    head = ajouterCreditNode(head, c);
    int ok = sauvegarderCredits(head);
    libererCredits(head);
    if (ok) {
        printf("\n" VERT "✔ Credit cree avec succes !\n" RESET);
        printf(BLANC_GRAS "Mensualite calculee : %.2lf DH\n" RESET, c.mensualite);
    } else {
        printf(ROUGE "Erreur sauvegarde credit.\n" RESET);
    }
    printf("\nAppuyez sur Entrer pour continuer...");
    getchar();
    return ok;
}

void afficherCredits(CreditNode *head) {
    int local = 0;
    CreditNode *p;
    if (!head) { head = chargerCredits(); local = 1; }

    clear();
    titre("LISTE DE TOUS LES CREDITS (ADMIN)");

    if (!head) { printf(JAUNE "Aucun credit enregistre.\n" RESET); printf("\nAppuyez sur Entrer..."); getchar(); return; }

    printf(CYAN "%-4s | %-8s | %-12s | %-6s | %-10s | %-8s\n" RESET, "ID", "CLIENT", "MONTANT", "DUREE", "MENSU.", "ETAT");
    printf("--------------------------------------------------------------------------\n");
    for (p= head; p; p = p->suiv) {
        Credit *c = &p->credit;
        printf("%-4d | %-8d | %-10.2lf DH | %-3d m | %-10.2lf | %s%s" RESET "\n",
               c->idCredit, c->idClient, c->montant, c->duree, c->mensualite,
               c->etat == ETAT_ACTIF ? VERT : ROUGE,
               c->etat == ETAT_ACTIF ? "ACTIF":"CLOTURE");
    }
    if (local) libererCredits(head);
    printf("\nAppuyez sur Entrer pour continuer...");
    getchar();
}

void afficherCreditsActifs(CreditNode *head) {
    int local = 0;
    CreditNode *p;
    if (!head) { head = chargerCredits(); local = 1; }

    clear();
    titre("LISTE DES CREDITS ACTIFS (ADMIN)");

    if (!head) { printf(JAUNE "Aucun credit enregistre.\n" RESET); printf("\nAppuyez sur Entrer..."); getchar(); return; }

    int found = 0;
    printf(CYAN "%-4s | %-8s | %-12s | %-10s | %-10s\n" RESET, "ID", "CLIENT", "MONTANT", "MENSU.", "RESTANT");
    for (p = head; p; p = p->suiv) {
        Credit *c = &p->credit;
        if (c->etat == ETAT_ACTIF) {
            printf("%-4d | %-8d | %-10.2lf | %-10.2lf | %-10.2lf\n",
                   c->idCredit, c->idClient, c->montant, c->mensualite, c->montantRestant);
            found = 1;
        }
    }
    if (!found) printf(JAUNE "Aucun credit actif trouve.\n" RESET);
    if (local) libererCredits(head);
    printf("\nAppuyez sur Entrer pour continuer...");
    getchar();
}

CreditNode* rechercherCreditParId(CreditNode *head, int id) {
    if (!head) return NULL;
    CreditNode *p;
    for (p = head; p; p = p->suiv) if (p->credit.idCredit == id) return p;
    return NULL;
}

void rechercherEtAfficherCredit(void) {
    int id;
    clear();
    titre("RECHERCHE DE CREDIT");
    printf("Entrez l'ID du credit a afficher : ");
    if (scanf("%d", &id) != 1) { while(getchar()!='\n'); printf(ROUGE "ID invalide.\n" RESET); return; }
    while(getchar()!='\n');

    CreditNode *head = chargerCredits();
    CreditNode *n = rechercherCreditParId(head, id);

    if (n) {
        Credit *c = &n->credit;
        printf("\n" CYAN "Details du Credit ID %d (Client: %d)" RESET "\n", c->idCredit, c->idClient);
        printf("=========================================\n");
        printf("Montant Initial : %.2lf DH\n", c->montant);
        printf("Taux Annuel     : %.2lf %%\n", c->taux);
        printf("Duree (mois)    : %d\n", c->duree);
        printf("Mensualite      : " JAUNE "%.2lf DH" RESET "\n", c->mensualite);
        printf("Montant Restant : %.2lf DH\n", c->montantRestant);
        printf("Etat            : %s\n", c->etat == ETAT_ACTIF ? VERT "ACTIF" RESET : ROUGE "CLOTURE" RESET);
        printf("Date Creation   : %s\n", c->dateCreation);
        printf("=========================================\n");

        afficherTableauAmortissement(c);

    } else {
        printf(ROUGE "Credit ID %d introuvable.\n" RESET, id);
    }
    libererCredits(head);
    printf("\nAppuyez sur Entrer pour continuer...");
    getchar();
}

void rechercherEtAfficherCreditsParClient(void) {
    int idClient;
    clear();
    titre("RECHERCHE CREDITS PAR CLIENT");
    printf("Entrez l'ID du Client : ");
    if (scanf("%d", &idClient) != 1) { while(getchar()!='\n'); printf(ROUGE "ID invalide.\n" RESET); return; }
    while(getchar()!='\n');

    CreditNode *head = chargerCredits();
    afficherCreditsClient(head, idClient);
    libererCredits(head);
}

int modifierCredit(CreditNode *head, int id) {
    int local = 0;
    if (!head) { head = chargerCredits(); local = 1; }

    CreditNode *n = rechercherCreditParId(head, id);
    if (!n) {
        printf(ROUGE "Credit introuvable.\n" RESET);
        if (local) libererCredits(head);
        return 0;
    }

    Credit *c = &n->credit;
    printf("\n" JAUNE "==== Modification Credit ID=%d ====" RESET "\n", id);

    printf("Montant (Actuel: %.2lf) -> Nouveau (0 pour garder): ", c->montant);
    double tmp; if (scanf("%lf", &tmp) == 1 && tmp > 0) { c->montant = tmp; } while(getchar()!='\n');

    printf("Taux (Actuel: %.2lf) -> Nouveau (-1 pour garder): ", c->taux);
    if (scanf("%lf", &tmp) == 1 && tmp >= 0) { c->taux = tmp; } while(getchar()!='\n');

    printf("Duree (Actuelle: %d) -> Nouvelle (0 pour garder): ", c->duree);
    int di; if (scanf("%d", &di) == 1 && di > 0) { c->duree = di; } while(getchar()!='\n');

    c->mensualite = calculerMensualite(c->montant, c->taux, c->duree);

    int ok = sauvegarderCredits(head);
    if (ok) printf(VERT "Credit modifie avec succes !\n" RESET); else printf(ROUGE "Erreur sauvegarde.\n" RESET);

    if (local) libererCredits(head);
    return ok;
}

int cloturerCredit(CreditNode *head, int id) {
    int local = 0;
    if (!head) { head = chargerCredits(); local = 1; }

    CreditNode *n = rechercherCreditParId(head, id);
    if (!n) {
        printf(ROUGE "Credit introuvable.\n" RESET);
        if (local) libererCredits(head);
        return 0;
    }

    n->credit.etat = ETAT_CLOTURE;
    n->credit.montantRestant = 0.0;
    dateCourante(n->credit.date_debut, DATE_SIZE);

    int ok = sauvegarderCredits(head);
    if (ok) printf(VERT "Credit %d cloture avec succes.\n" RESET, id);

    if (local) libererCredits(head);
    return ok;
}

int supprimerCreditInteractive(void) {
    int id;
    clear();
    titre("SUPPRESSION DEFINITIVE D'UN CREDIT");
    printf(ROUGE "ATTENTION : Cette action est irreversible.\n" RESET);
    printf("Entrez l'ID du credit a supprimer : ");
    if (scanf("%d", &id) != 1) { while(getchar()!='\n'); printf(ROUGE "ID invalide.\n" RESET); return 0; }
    while(getchar()!='\n');

    printf("Confirmez la suppression du Credit ID %d (O/N) : ", id);
    char conf = getchar();
    if (conf != 'O' && conf != 'o') {
        printf("Suppression annulee.\n");
        return 0;
    }
    while(getchar()!='\n');

    CreditNode *head = chargerCredits();
    head = supprimerCreditNode(head, id);
    libererCredits(head);
    return 1;
}

CreditNode* supprimerCreditNode(CreditNode *head, int id) {
    CreditNode *cur = head, *prev = NULL;
    while (cur) {
        if (cur->credit.idCredit == id) {
            if (prev) prev->suiv = cur->suiv; else head = cur->suiv;
            free(cur);
            sauvegarderCredits(head);
            printf(VERT "Credit %d supprime.\n" RESET);
            return head;
        }
        prev = cur; cur = cur->suiv;
    }
    printf(ROUGE "Credit non trouve.\n" RESET);
    return head;
}

/* --------------- Fonctions CLIENT (Vues) --------------- */

void afficherCreditsClient(CreditNode *head, int idClient) {
    int local = 0;
    if (!head) { head = chargerCredits(); local = 1; }

    clear();
    titre("VOS DOSSIERS DE CREDIT");

    if (!head) { printf(JAUNE "Aucun credit enregistre.\n" RESET); printf("\nAppuyez sur Entrer..."); getchar(); return; }

    int found = 0;
    CreditNode *p;
    printf(CYAN "%-4s | %-12s | %-8s | %-10s | %-10s | %-8s\n" RESET, "ID", "MONTANT", "TAUX", "MENSU.", "RESTE", "ETAT");
    for (p = head; p; p = p->suiv) {
        Credit *c = &p->credit;
        if (c->idClient == idClient) {
            printf("%-4d | %-10.2lf | %-6.2lf%% | %-10.2lf | %-10.2lf | %s%s" RESET "\n",
                   c->idCredit, c->montant, c->taux, c->mensualite, c->montantRestant,
                   c->etat == ETAT_ACTIF ? VERT : ROUGE,
                   c->etat == ETAT_ACTIF ? "ACTIF":"CLOTURE");
            found = 1;
        }
    }
    if (!found) printf(JAUNE "Vous n'avez aucun credit enregistre.\n" RESET);
    if (local) libererCredits(head);
    printf("\nAppuyez sur Entrer pour continuer...");
    getchar();
}

void afficherTableauAmortissement(Credit *c) {
    if (!c) { printf(ROUGE "Credit invalide.\n" RESET); return; }
    double montant = c->montant;
    int duree = c->duree;
    double mensualite = c->mensualite;
    double tauxM = c->taux / 100.0 / 12.0;

  printf("\n" BLANC_GRAS "--- TABLEAU D'AMORTISSEMENT (ID %d) ---" RESET "\n", c->idCredit);
  printf(CYAN "%-5s | %-12s| %-12s| %-12s" RESET "\n", "Mois", "Interet", "Principal", "Reste");
   printf("------------------------------------------------------------\n");

    double reste = montant;
    int m;
    for (m = 1; m <= duree; ++m) {
        double interet = reste * tauxM;
        double principal = mensualite - interet;
        if (principal < 0) principal = 0;
        reste -= principal;
        if (reste < 0) reste = 0;
        printf("%-5d | %-10.2lf | %-10.2lf | %-10.2lf\n", m, interet, principal, reste);
        if (reste <= 0.0) break;
    }
    printf("------------------------------------------------------------\n");
}

/* --------------- Fonctions Transversales --------------- */

int appliquerPaiementSurCredit(int idCredit, double montantPaiement) {
    CreditNode *head = chargerCredits();
    CreditNode *p = rechercherCreditParId(head, idCredit);

    if (!p) {
        printf(ROUGE "Erreur : Credit %d introuvable.\n" RESET, idCredit);
        libererCredits(head);
        return 0;
    }

    if (p->credit.etat == ETAT_CLOTURE) {
        printf(ROUGE "Erreur : Credit deja cloture.\n" RESET);
        libererCredits(head);
        return 0;
    }

    p->credit.montantRestant -= montantPaiement;

    if (p->credit.montantRestant <= 0.0) {
        p->credit.montantRestant = 0.0;
        p->credit.etat = ETAT_CLOTURE;
        printf(VERT "Credit %d entierement solde et cloture.\n" RESET, idCredit);
    }

    int ok = sauvegarderCredits(head);
    libererCredits(head);
    return ok;
}


int importerDemandeCommeCredit(const struct Demande *d) {
    if (!d || d->statut != STATUT_ACCEPTEE || d->taux <= 0) {
        printf(ROUGE "Erreur: Demande invalide ou non acceptee.\n" RESET);
        return 0;
    }
    Credit c;
    c.idCredit = prochainIdCredit();
    c.idClient = d->idClient;
    c.montant = d->montant;
    c.taux = d->taux;
    c.duree = d->duree;
    c.mensualite = calculerMensualite(c.montant, c.taux, c.duree);
    c.montantRestant = c.montant;
    dateCourante(c.dateCreation, DATE_SIZE);
    dateCourante(c.date_debut, DATE_SIZE);
    c.etat = ETAT_ACTIF;

    CreditNode *head = chargerCredits();
    head = ajouterCreditNode(head, c);
    int ok = sauvegarderCredits(head);
    libererCredits(head);

    if (ok) printf(VERT "Demande importee comme Credit ID %d (Mensualite: %.2lf DH).\n" RESET, c.idCredit, c.mensualite);
    return ok;
}

void menuGestionCredits(void) {
    int choix;
    int id;

    do {
        clear();
        espaceCentre();cadre();
        espaceCentre();printf("                            "CYAN"💰  GESTION DES CREDITS  "RESET"        \n");
        espaceCentre();cadre();
        printf(BLANC_GRAS);
        espaceCentre();printf("                    +-----------------------------------------------+\n");
        espaceCentre();printf("                    |                                               |\n");
        espaceCentre();printf("                    |  1. Ajouter un nouveau credit                 |\n");
        espaceCentre();printf("                    |  2. Details d'un credit (Amortissement)       |\n");
        espaceCentre();printf("                    |  3. Modifier un credit                        |\n");
        espaceCentre();printf("                    |  4. Cloturer un credit                        |\n");
        espaceCentre();printf("                    |  5. Afficher tous les credits                 |\n");
        espaceCentre();printf("                    |  6. Afficher credits actifs                   |\n");
        espaceCentre();printf("                    |  7. Credits par client                        |\n");
        espaceCentre();printf("                    |  8. Supprimer un credit                       |\n");
        espaceCentre();printf("                    |  9. Retour au Menu Admin                      |\n");
        espaceCentre();printf("                    +-----------------------------------------------+\n");
        printf(RESET);
        printf("\n");
         espaceCentre(); printf("            ➤ Votre choix : ");


        if (scanf("%d", &choix) != 1) {
            while(getchar() != '\n');
            choix = 0;
        }
        while(getchar() != '\n');

        switch (choix) {
            case 1: ajouterCreditFromInteractive(); break;
            case 2: rechercherEtAfficherCredit(); break;
            case 3:
                printf("ID du credit a modifier : ");
                if (scanf("%d", &id) == 1) modifierCredit(NULL, id);
                while(getchar() != '\n');
                break;
            case 4:
                printf("ID du credit a cloturer : ");
                if (scanf("%d", &id) == 1) cloturerCredit(NULL, id);
                while(getchar() != '\n');
                break;
            case 5: afficherCredits(NULL); break;
            case 6: afficherCreditsActifs(NULL); break;
            case 7: rechercherEtAfficherCreditsParClient(); break;
            case 8: supprimerCreditInteractive(); break;
            case 9: printf("Retour...\n"); break;
            default: printf(ROUGE "Option invalide.\n" RESET); getchar(); break;
        }
    } while (choix != 9);
}
