#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include <time.h>
#include <math.h>
#include "credit.h"     // obligatoire ici si tu utilises Credit
#include "paiement.h"   // si tu utilises Paiement/PaiementNode
#include "client.h"
#include "compteClient.h"
#include "style.h"
#ifndef FICHIER_PAIEMENTS
#define FICHIER_PAIEMENTS "paiements(1).txt"
#endif



// ====== Fonctions utilitaires ======

// Calculer la mensualit√© d'un cr√©dit donn√©
double calculerMensualiteCredit(Credit credit) {
    // Utiliser la fonction du module cr√©dit
    return calculerMensualite(credit.montant, credit.taux, credit.duree);
}

// ====== Validation ======


int validerDate(const char *date) {
    if (!date || strlen(date) != 10) {
        return 0;
    }

    // Verifier le format AAAA-MM-JJ
    if (date[4] != '-' || date[7] != '-') {
        return 0;
    }

    int annee, mois, jour;
    if (sscanf(date, "%d-%d-%d", &annee, &mois, &jour) != 3) {
        return 0;
    }


    if (annee < 2000 || annee > 2100) return 0;
    if (mois < 1 || mois > 12) return 0;
    if (jour < 1 || jour > 31) return 0;


    int joursParMois[] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };


    if ((annee % 4 == 0 && annee % 100 != 0) || (annee % 400 == 0)) {
        joursParMois[1] = 29;
    }

    if (jour > joursParMois[mois - 1]) {
        return 0;
    }

    return 1;
}

// Valide qu'un montant est positif et raisonnable
int validerMontant(float montant) {
    if (montant <= 0) {
        return 0;
    }

    // Limite maximale raisonnable : 10 millions
    const float MAX_MONTANT = 10000000.0f;
    if (montant > MAX_MONTANT) {
        return 0;
    }

    return 1;
}


int verifierIntegritePaiements(CreditNode *credits, PaiementNode *paiements) {
    if (!paiements) return 1; // Pas de paiements = pas de probleme

    int erreurs = 0;
    PaiementNode *p = paiements;


    int maxIdCredit = 0;
    CreditNode *c = credits;
    while (c) {
        if (c->credit.idCredit > maxIdCredit) {
            maxIdCredit = c->credit.idCredit;
        }
        c = c->suiv;
    }

    // Tableau pour verifier les credits existants
    int *creditsExistants = (int*)calloc(maxIdCredit + 1, sizeof(int));
    if (!creditsExistants) return 0;

    c = credits;
    while (c) {
        if (c->credit.idCredit >= 0 && c->credit.idCredit <= maxIdCredit) {
            creditsExistants[c->credit.idCredit] = 1;
        }
        c = c->suiv;
    }

    // Verifier chaque paiement
    int *idsPaiements = NULL;
    int capaciteIds = 0;
    int nbIds = 0;

    while (p) {
        // Verifier  que le credit reference existe
        if (p->data.IDCredit < 0 || p->data.IDCredit > maxIdCredit ||
            !creditsExistants[p->data.IDCredit]) {
            printf("\n" ROUGE "Erreur integrite : Paiement ID %d reference un credit inexistant (ID: %d)" RESET "\n",
                   p->data.IDPaiement, p->data.IDCredit);
            erreurs++;
        }


        int trouve = 0;
        int i;
        for ( i = 0; i < nbIds; i++) {
            if (idsPaiements[i] == p->data.IDPaiement) {
                trouve = 1;
                break;
            }
        }
        if (trouve) {
            printf("Erreur integrite : IDPaiement duplique (%d)\n", p->data.IDPaiement);
            erreurs++;
        } else {
            // Ajouter l'ID √† la liste
            if (nbIds >= capaciteIds) {
                capaciteIds = capaciteIds == 0 ? 16 : capaciteIds * 2;
                idsPaiements = (int*)realloc(idsPaiements, capaciteIds * sizeof(int));
                if (!idsPaiements) {
                    free(creditsExistants);
                    return 0;
                }
            }
            idsPaiements[nbIds++] = p->data.IDPaiement;
        }

        if (!validerMontant(p->data.montant)) {
            printf("Erreur integrite: Montant invalide pour paiement ID %d (%.2f)\n",
                   p->data.IDPaiement, p->data.montant);
            erreurs++;
        }

        // Verifier les dates
        if (!validerDate(p->data.datePrevue)) {
            printf("Erreur integrite : Date prevue invalide pour paiement ID %d (%s)\n",
                   p->data.IDPaiement, p->data.datePrevue);
            erreurs++;
        }

        if (p->data.datePaiement[0] != '\0' && !validerDate(p->data.datePaiement)) {
            printf("Erreur integrite : Date paiement invalide pour paiement ID %d (%s)\n",
                   p->data.IDPaiement, p->data.datePaiement);
            erreurs++;
        }

        p = p->next;
    }

    free(creditsExistants);
    free(idsPaiements);

    if (erreurs == 0) {
        printf(" Verification d'integrite reussie : aucune erreur detectee.\n");
    } else {
        printf(" %d erreur(s) d'integrite detectee(s).\n", erreurs);
    }

    return erreurs == 0;
}

// ====== Gestion de la liste chainee ======


PaiementNode* ajouterPaiementListe(PaiementNode *tete, Paiement p) {
    PaiementNode *nouveau = (PaiementNode*)malloc(sizeof(PaiementNode));
    if (!nouveau) {
        fprintf(stderr, "Erreur: Impossible d'allouer la m√©moire pour le paiement\n");
        return tete;
    }

    nouveau->data = p;
    nouveau->next = NULL;

    if (!tete) {
        return nouveau;
    }

    // Ajouter en fin de liste
    PaiementNode *tmp = tete;
    while (tmp->next) {
        tmp = tmp->next;
    }
    tmp->next = nouveau;

    return tete;
}


void libererListePaiements(PaiementNode *tete) {
    PaiementNode *tmp;
    while (tete) {
        tmp = tete;
        tete = tete->next;
        free(tmp);
    }
}

void ajouterMois(char *dateStr) {
    int annee, mois, jour;
    sscanf(dateStr, "%d-%d-%d", &annee, &mois, &jour);


    mois++;
    if (mois > 12) {
        mois = 1;
        annee++;
    }

    // Ajuster le jour si necessaire
    int joursParMois[] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };


    if ((annee % 4 == 0 && annee % 100 != 0) || (annee % 400 == 0)) {
        joursParMois[1] = 29;
    }

    if (jour > joursParMois[mois - 1]) {
        jour = joursParMois[mois - 1];
    }


    sprintf(dateStr, "%04d-%02d-%02d", annee, mois, jour);
}
// Fonction pour trouver le prochain ID de paiement disponible
static int prochainIdPaiement(void) {
    PaiementNode *liste = chargerPaiementsFichier();
    int maxId = 0;
    PaiementNode *tmp = liste;

    while (tmp) {
        if (tmp->data.IDPaiement > maxId) {
            maxId = tmp->data.IDPaiement;
        }
        tmp = tmp->next;
    }

    libererListePaiements(liste);
    return maxId + 1;
}

void genererCalendrierPaiements(Credit *credit) {
    if (credit == NULL) return;

    PaiementNode *listeExistante = chargerPaiementsFichier();
    char dateCourante[11];
    strcpy(dateCourante, credit->date_debut);

    // Calcul de l'ID de dÈpart pour les paiements
    int idBase = 1;
    PaiementNode *temp = listeExistante;
    while (temp != NULL) {
        if (temp->data.IDPaiement >= idBase) idBase = temp->data.IDPaiement + 1;
        temp = temp->next;
    }

    for (int i = 0; i < credit->duree; i++) {
        Paiement p;
        p.IDPaiement = idBase + i;
        p.IDCredit = credit->idCredit;
        p.numMois = i + 1;
        p.montant = (float)credit->mensualite; // On utilise la valeur du fichier binaire
        p.montantPenalite = 0.0f;
        strcpy(p.datePrevue, dateCourante);
        strcpy(p.datePaiement, "");
        p.statut = STATUT_ATTENTE;

        listeExistante = ajouterPaiementListe(listeExistante, p);
        ajouterMois(dateCourante); // Passe au mois suivant
    }

    if (sauvegarderPaiementsFichier(listeExistante)) {
        printf(BLANC_GRAS "Echeancier de %d mois genere dans paiements.txt\n" RESET, credit->duree);
    }
    libererListePaiements(listeExistante);
}
int sauvegarderPaiementsFichier(PaiementNode *liste) {
    FILE *f = fopen(FICHIER_PAIEMENTS, "w");
    if (!f) {
        fprintf(stderr, "Erreur : Impossible d'ouvrir %s pour ecriture\n", FICHIER_PAIEMENTS);
        return 0;
    }

    // En-t√™te
    fprintf(f, "%-10s %-10s %-6s %-12s %-15s %-12s %-12s %-6s\n",
            "IDPaiement", "IDCredit", "Mois", "Montant", "MontantPenalite",
            "DatePrevue", "DatePaiement", "Statut");

    // Contenu
    PaiementNode *p;
    for (p = liste; p != NULL; p = p->next) {
        const char *datePaiementAff = (p->data.datePaiement[0] != '\0') ? p->data.datePaiement : "-";
        fprintf(f, "%-10d %-10d %-6d %-12.2f %-15.2f %-12s %-12s %-6d\n",
                p->data.IDPaiement,
                p->data.IDCredit,
                p->data.numMois,
                p->data.montant,
                p->data.montantPenalite,
                p->data.datePrevue,
                datePaiementAff,
                (int)p->data.statut);
    }

    if (fclose(f) != 0) {
        fprintf(stderr, "Erreur : Impossible de fermer le fichier %s\n", FICHIER_PAIEMENTS);
        return 0;
    }
    return 1;
}


// ---------- chargerPaiementsFichier ----------

PaiementNode* chargerPaiementsFichier(void) {
    FILE *f = fopen(FICHIER_PAIEMENTS, "r");
    if (!f) {

        return NULL;
    }

    PaiementNode *tete = NULL;
    PaiementNode *dernier = NULL;
    char ligne[512];

    // Lire la premiere ligne : peut etre un en-tete lisible
    if (!fgets(ligne, sizeof(ligne), f)) {
        fclose(f);
        return NULL;
    }


    char *ptr = ligne;
    while (*ptr && isspace((unsigned char)*ptr)) ptr++;
    if (!isdigit((unsigned char)*ptr) && *ptr != '-' ) {
        // premi√®re ligne en-t√™te -> ignorer
    } else {
        // premi√®re ligne contient des donn√©es -> la traiter
        // repositionner au d√©but de la ligne pour traitement
        // on remplit la structure √† partir de 'ligne' ci-dessus
        Paiement p;
        int statutInt = 0;
        // format : IDPaiement IDCredit numMois montant montantPenalite datePrevue datePaiement statut
        // toutes les colonnes sont s√©par√©es par espaces (dates n'ont pas d'espaces)
        if (sscanf(ligne, "%d %d %d %f %f %10s %10s %d",
                   &p.IDPaiement, &p.IDCredit, &p.numMois,
                   &p.montant, &p.montantPenalite,
                   p.datePrevue, p.datePaiement, &statutInt) == 8) {
            p.statut = (StatutPaiement)statutInt;
            PaiementNode *node = (PaiementNode*)malloc(sizeof(PaiementNode));
            if (node) {
                node->data = p;
                node->next = NULL;
                tete = dernier = node;
            }
        }
    }

    // Lire le reste du fichier
    while (fgets(ligne, sizeof(ligne), f)) {
        // sauter lignes vides
        char *pstart = ligne;
        while (*pstart && isspace((unsigned char)*pstart)) pstart++;
        if (*pstart == '\0' || *pstart == '\n') continue;

        Paiement p;
        int statutInt = 0;
        int nb = sscanf(ligne, "%d %d %d %f %f %10s %10s %d",
                        &p.IDPaiement, &p.IDCredit, &p.numMois,
                        &p.montant, &p.montantPenalite,
                        p.datePrevue, p.datePaiement, &statutInt);
        if (nb < 8) {
            // ligne mal form√©e -> ignorer ou tenter un parse tol√©rant
            continue;
        }
        p.statut = (StatutPaiement)statutInt;

        PaiementNode *node = (PaiementNode*)malloc(sizeof(PaiementNode));
        if (!node) continue; // ignorer si pb memoire
        node->data = p;
        node->next = NULL;

        if (!tete) tete = dernier = node;
        else {
            dernier->next = node;
            dernier = node;
        }
    }

    fclose(f);
    return tete;
}


// ---------- lierPaiementsAuxCredits ----------

void lierPaiementsAuxCredits(CreditNode *credits, PaiementNode *paiements) {
    if (!credits || !paiements) return;

    // R√©initialiser les listes paiements de chaque credit
    CreditNode *c;
    for (c = credits; c != NULL; c = c->suiv) {
        c->credit.paiement = NULL;
    }
   PaiementNode *p;
    // Parcourir la liste des paiements et rattacher chaque paiement au bon cr√©dit
    for (p = paiements; p != NULL; p = p->next) {
        // trouver le credit correspondant
        CreditNode *c = credits;
        while (c != NULL && c->credit.idCredit != p->data.IDCredit) {
            c = c->suiv;
        }
        if (c == NULL) {
            // cr√©dit non trouv√© -> on peut logguer ou ignorer
            continue;
        }

        // cr√©er une nouvelle copie du noeud paiement pour attacher au credit
        PaiementNode *copie = (PaiementNode *)malloc(sizeof(PaiementNode));
        if (!copie) continue; // pb memoire, on passe au suivant
        copie->data = p->data;
        copie->next = NULL;

        // attacher en fin de la liste du credit (pr√©server l'ordre venant du fichier)
        if (c->credit.paiement == NULL) {
            c->credit.paiement = copie;
        } else {
            PaiementNode *q = c->credit.paiement;
            while (q->next) q = q->next;
            q->next = copie;
        }
    }
}


// Fonction pour mettre √† jour seulement le paiement modifi√© dans le fichier
void miseAJourPaiementFichier(Paiement *paiement) {
    FILE *fIn = fopen(FICHIER_PAIEMENTS, "r");
    FILE *fOut = fopen("temp.txt", "w");
    if (!fIn || !fOut) {
        printf("Erreur fichier pour mise A jour du paiement.\n");
        if (fIn) fclose(fIn);
        if (fOut) fclose(fOut);
        return;
    }

    char ligne[256];
    while (fgets(ligne, sizeof(ligne), fIn)) {
        int idPaiement;
        Paiement tmp;
        sscanf(ligne, "%d;%d;%d;%f;%f;%10s;%10s;%d",
               &idPaiement, &tmp.IDCredit, &tmp.numMois, &tmp.montant,
               &tmp.montantPenalite, tmp.datePrevue, tmp.datePaiement, (int*)&tmp.statut);

        if (idPaiement == paiement->IDPaiement) {

            fprintf(fOut, "%d;%d;%d;%.2f;%.2f;%s;%s;%d\n",
                    paiement->IDPaiement, paiement->IDCredit, paiement->numMois,
                    paiement->montant, paiement->montantPenalite,
                    paiement->datePrevue, paiement->datePaiement, paiement->statut);
        } else {

            fputs(ligne, fOut);
        }
    }

    fclose(fIn);
    fclose(fOut);
    remove(FICHIER_PAIEMENTS);
    rename("temp.txt", FICHIER_PAIEMENTS);
}

// Structure pour indexer les paiements par IDCredit (optimisation)
typedef struct {
    int idCredit;
    PaiementNode *paiements;  // Liste des paiements pour ce cr√©dit
} IndexPaiement;

void executerPrelevementsAutomatiques(CreditNode *listeCredits, CompteClientNode *listeComptes, PaiementNode **paiementsEnRetard) {
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    char today[11];
    sprintf(today, "%04d-%02d-%02d", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday);

    PaiementNode *tous = chargerPaiementsFichier();
    if (!tous) return;

    CreditNode *c = listeCredits;
    while (c != NULL) {
        if (c->credit.etat == 0) { // Ignorer les crÈdits clÙturÈs
            c = c->suiv;
            continue;
        }

        PaiementNode *p = tous;
        while (p != NULL) {
            // Si le paiement appartient au crÈdit, est en attente et c'est aujourd'hui (ou passÈ)
            if (p->data.IDCredit == c->credit.idCredit &&
                p->data.statut == STATUT_ATTENTE &&
                strcmp(p->data.datePrevue, today) <= 0) {

                CompteClientNode *compte = chercherCompte(listeComptes, c->credit.idClient);

                if (compte && verifierSolde(compte, p->data.montant)) {
                    // 1. DÈbit du compte
                    effectuerPrelevement(compte, p->data.montant);

                    // 2. Mise ‡ jour statut mÈmoire
                    p->data.statut = STATUT_PAYE;
                    strcpy(p->data.datePaiement, today);
                    p->data.montantPenalite = 0.0f;

                    // 3. SYNCHRO BINAIRE (Mise ‡ jour de credits.bin)
                    appliquerPaiementSurCredit(p->data.IDCredit, (double)p->data.montant);

                    printf(BLANC_GRAS "Prelevement reussi: Credit %d (%.2f DH)\n" RESET, c->credit.idCredit, p->data.montant);
                } else {
                    // …chec du prÈlËvement -> Retard
                    p->data.statut = STATUT_ECHEC;
                    p->data.montantPenalite = PENALITE_RETARD;

                    // Ajouter ‡ la liste des retards pour affichage immÈdiat
                    *paiementsEnRetard = ajouterPaiementListe(*paiementsEnRetard, p->data);
                    printf(ROUGE "Echec prelevement: Credit %d (Solde insuffisant)\n" RESET, c->credit.idCredit);
                }
            }
            p = p->next;
        }
        c = c->suiv;
    }

    // Sauvegarde globale aprËs les modifications
    sauvegarderPaiementsFichier(tous);
    sauvegarderComptesFichier(listeComptes);
    libererListePaiements(tous);
}
void afficherPaiementsEnRetard(PaiementNode *liste) {
    if (!liste) {
        printf("\n"BLANC_GRAS "Aucun paiement en retard.\n",RESET);
        return;
    }

    printf("\n=== LISTE DES PAIEMENTS EN RETARD ===\n");
    printf("-------------------------------------------------------------\n");
    printf("| %-8s | %-8s | %-7s | %-12s | %-12s | %-8s | %-10s |\n",
           "IDPaiem", "IDCre", "Mois", "Date prevue", "Date paiement",
           "Montant", "Penalite");
    printf("-------------------------------------------------------------\n");

    PaiementNode *tmp = liste;
    while (tmp) {
        Paiement p = tmp->data;

        printf("| %-8d | %-8d | %-7d | %-12s | %-12s | %-8.2f | %-10.2f |\n",
               p.IDPaiement,
               p.IDCredit,
               p.numMois,
               p.datePrevue,
               (strlen(p.datePaiement) > 0 ? p.datePaiement : "-"),
               p.montant,
               p.montantPenalite
        );

        tmp = tmp->next;
    }

    printf("----------------------------------------------------------------\n");
}




// Paiement manuel pour un paiement en retard
void paiementManuelMenu(PaiementNode **paiementsEnRetard, CreditNode *listeCredits, CompteClientNode *listeComptes) {
    // 1. On force le rechargement depuis le fichier pour ne rien rater
    PaiementNode *tous = chargerPaiementsFichier();
    PaiementNode *curr = tous;
    int trouveRetard = 0;

    printf("\n" CYAN "--- RECHERCHE DES IMPAYES EN COURS ---" RESET "\n");

    // Affichage rapide des options disponibles pour l'utilisateur
    while (curr) {
        if (curr->data.statut == STATUT_ECHEC || curr->data.statut == STATUT_ATTENTE) {
            printf("[%d] Credit %d - Montant: %.2f DH (Date: %s)\n",
                   curr->data.IDPaiement, curr->data.IDCredit, curr->data.montant, curr->data.datePrevue);
            trouveRetard = 1;
        }
        curr = curr->next;
    }

    if (!trouveRetard) {
        printf(ROUGE "Aucun paiement en retard n'a ete trouve dans le fichier." RESET "\n");
        libererListePaiements(tous);
        return;
    }

    // 2. Saisie de l'ID par l'utilisateur
    int idSaisi;
    printf("\nEntrez l'ID du paiement a regler : ");
    scanf("%d", &idSaisi);

    // 3. Traitement du paiement choisi
    curr = tous;
    while (curr) {
        if (curr->data.IDPaiement == idSaisi) {
            CreditNode *cn = rechercherCreditParId(listeCredits, curr->data.IDCredit);
            CompteClientNode *compte = chercherCompte(listeComptes, cn->credit.idClient);

            float total = curr->data.montant + curr->data.montantPenalite;

            if (compte && verifierSolde(compte, total)) {
                effectuerPrelevement(compte, total);

                curr->data.statut = STATUT_PAYE;
                time_t t = time(NULL);
                strftime(curr->data.datePaiement, 11, "%Y-%m-%d", localtime(&t));

                // SYNCHRO BINAIRE
                appliquerPaiementSurCredit(curr->data.IDCredit, (double)curr->data.montant);

                sauvegarderPaiementsFichier(tous);
                sauvegarderComptesFichier(listeComptes);

                printf(BLANC_GRAS "Paiement effectue ! Nouveau solde : %.2f DH" RESET "\n", compte->compte.solde);
            } else {
                printf(ROUGE "Erreur : Solde insuffisant." RESET "\n");
            }
            break;
        }
        curr = curr->next;
    }
    libererListePaiements(tous);
}


void menuPaiementsEnRetard(PaiementNode **paiementsEnRetard, CreditNode *listeCredits, CompteClientNode *listeComptes) {
    int choix;
    do {
        printf("\n=== GESTION DES PAIEMENTS EN RETARD ===\n");
        printf("1. Afficher tous les paiements en retard\n");
        printf("2. Effectuer un paiement manuel\n");
        printf("0. Retour\n");
        printf("Votre choix : ");

        if (scanf("%d", &choix) != 1) {
            while (getchar() != '\n'); // vider le buffer
            choix = -1;
        }

        switch (choix) {
            case 1:
                afficherPaiementsEnRetard(*paiementsEnRetard);
                break;
            case 2:
                paiementManuelMenu(paiementsEnRetard, listeCredits, listeComptes); // r√®glement manuel par l'admin
                break;
            case 0:
                break;
            default:
                printf("Choix invalide. Reessayez.\n");
        }
    } while (choix != 0);
}



void menuPaiements(CreditNode *listeCredits, CompteClientNode *listeComptes) {
    int choix;
    // On charge les retards existants dans le fichier dËs l'ouverture du menu
    PaiementNode *paiementsEnRetard = NULL;

    // Petite boucle pour scanner le fichier et remplir la liste des retards au dÈbut
    PaiementNode *tous = chargerPaiementsFichier();
    PaiementNode *curr = tous;
    while (curr) {
        if (curr->data.statut == STATUT_ECHEC) {
            paiementsEnRetard = ajouterPaiementListe(paiementsEnRetard, curr->data);
        }
        curr = curr->next;
    }
    libererListePaiements(tous);

    do {
        printf("\n" GRAS "===========================================" RESET);
        printf("\n" CYAN "          GESTION DES PAIEMENTS" RESET);
        printf("\n" GRAS "===========================================" RESET);
        printf("\n1. Afficher les paiements d'un credit");
        printf("\n2. " GRAS "Executer les prelevements automatiques du jour" RESET);
        printf("\n3. " GRAS "Gerer les paiements en retard (paiement manuel)" RESET);
        printf("\n4. Afficher le capital restant du");
        printf("\n5. Ajouter un credit ‡ un client");
        printf("\n6. Verifier l'integrite des paiements");
        printf("\n0. " BLANC_GRAS "Retour" RESET);
        printf("\n-------------------------------------------");
        printf("\nVotre choix : ");

        if (scanf("%d", &choix) != 1) {
            while (getchar() != '\n');
            choix = -1;
        }

        switch (choix) {
            case 1:
                afficherPaiementsCreditMenu(listeCredits);
                break;
            case 2:
                printf("\n" CYAN "Lancement des prelevements..." RESET "\n");
                executerPrelevementsAutomatiques(listeCredits, listeComptes, &paiementsEnRetard);
                // On sauvegarde les comptes immÈdiatement aprËs les prelevements
                sauvegarderComptesFichier(listeComptes);
                break;
            case 3:
                // Appel du sous-menu des retards
                menuPaiementsEnRetard(&paiementsEnRetard, listeCredits, listeComptes);
                break;
            case 4:
                capitalRestantMenu(listeCredits);
                break;
            case 5:
                ajouterCreditMenu(listeCredits);
                break;
            case 6:
                printf("\n" CYAN "Verification de l'integrite..." RESET "\n");
                PaiementNode *pCheck = chargerPaiementsFichier();
                verifierIntegritePaiements(listeCredits, pCheck);
                libererListePaiements(pCheck);
                break;
            case 0:
                printf("\nRetour au menu principal...\n");
                break;
            default:
                printf(ROUGE "Choix invalide !" RESET "\n");
        }
    } while (choix != 0);

    libererListePaiements(paiementsEnRetard);
}

/* Fonctions utilitaires pour v√©rifier solde et effectuer pr√©l√®vement */
 int verifierSolde(CompteClientNode *compte, float montant) {
    if (!compte) return 0;
    return compte->compte.solde >= montant;
}

void effectuerPrelevement(CompteClientNode *compte, float montant) {
    if (compte && montant > 0) {
        compte->compte.solde -= montant;
    }
}

/* Afficher les paiements d'un cr√©dit */
void afficherPaiementsCreditMenu(CreditNode *listeCredits) {
    int idCredit;
    printf("ID du credit : ");
    if (scanf("%d", &idCredit) != 1) {
        while (getchar() != '\n');
        printf("ID invalide.\n");
        return;
    }

    CreditNode *credit = rechercherCreditParId(listeCredits, idCredit);
    if (!credit) {
        printf("Credit introuvable.\n");
        return;
    }

    // Charger les paiements depuis le fichier
    PaiementNode *paiements = chargerPaiementsFichier();
    PaiementNode *tmp = paiements;
    int trouve = 0;

    printf("\n=== PAIEMENTS DU CREDIT %d ===\n", idCredit);
    printf("-------------------------------------------------------------------------------------------\n");
    printf("| %-8s | %-8s | %-7s | %-12s | %-12s | %-8s | %-10s |\n",
           "IDPaiem", "IDCre", "Mois", "Date prevue", "Date paiement",
           "Montant", "Penalite");
    printf("-------------------------------------------------------------------------------------------\n");

    while (tmp) {
        if (tmp->data.IDCredit == idCredit) {
            printf("| %-8d | %-8d | %-7d | %-12s | %-12s | %-8.2f | %-10.2f |\n",
                   tmp->data.IDPaiement,
                   tmp->data.IDCredit,
                   tmp->data.numMois,
                   tmp->data.datePrevue,
                   (strlen(tmp->data.datePaiement) > 0 ? tmp->data.datePaiement : "-"),
                   tmp->data.montant,
                   tmp->data.montantPenalite);
            trouve = 1;
        }
        tmp = tmp->next;
    }

    if (!trouve) {
        printf("Aucun paiement trouv√© pour ce cr√©dit.\n");
    }

    printf("-------------------------------------------------------------------------------------------\n");
    libererListePaiements(paiements);
}

/* Afficher le capital restant d√ª */
void capitalRestantMenu(CreditNode *listeCredits) {
    int idC;
    printf("\nEntrez l'ID du credit pour voir le capital restant : ");
    if (scanf("%d", &idC) != 1) {
        while (getchar() != '\n');
        return;
    }

    // On cherche dans la liste chargÈe depuis credits.bin
    CreditNode *c = rechercherCreditParId(listeCredits, idC);

    if (c != NULL) {
        printf("\n" CYAN "==========================================" RESET);
        printf("\n   SITUATION FINANCIERE DU CREDIT %d", idC);
        printf("\n" CYAN "==========================================" RESET);
        printf("\n Client rattache   : %d", c->credit.idClient);
        printf("\n Montant Initial   : %.2f DH", c->credit.montant);
        printf("\n Montant deja paye : %.2f DH", (c->credit.montant - c->credit.montantRestant));
        printf("\n CAPITAL RESTANT   : " JAUNE "%.2f DH" RESET, c->credit.montantRestant);
        printf("\n Etat du dossier   : %s", (c->credit.etat == 1) ? "ACTIF" : "CL‘TUR…");
        printf("\n" CYAN "==========================================\n" RESET);
    } else {
        printf(ROUGE "Erreur : Aucun credit trouve avec l'ID %d dans le fichier binaire.\n" RESET, idC);
    }
}
/* Ajouter un cr√©dit √† un client */
void ajouterCreditMenu(CreditNode *listeCredits) {
    // Utiliser la fonction existante du module cr√©dita
    ajouterCreditFromInteractive();
}

/* Wrapper pour le menu principal - charge les donn√©es n√©cessaires */
void menuGestionPaiements(void) {
    printf("\n"BLANC_GRAS "Chargement des bases de donnees..." RESET "\n");

    CreditNode *listeCredits = chargerCredits();
    CompteClientNode *listeComptes = chargerComptesFichier();

    if (!listeCredits) {
        printf(ROUGE "Erreur : Impossible de charger les credits ." RESET "\n");
    }

    menuPaiements(listeCredits, listeComptes);

    // CRUCIAL : On sauvegarde tout avant de quitter pour ne rien perdre
    printf("\n" GRAS "Sauvegarde des modifications en cours..." RESET "\n");
    sauvegarderComptesFichier(listeComptes);

    libererCredits(listeCredits);
    libererListeComptes(listeComptes);

    printf(GRAS "Donnees securisÈes. Fermeture du module." RESET "\n");
}
