#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "compteClient.h"
#include "style.h"  // Inclus pour les couleurs et fonctions de style

// Déclarations des fonctions externes du module client
extern ClientNode* listerClientsFichier(void);
extern void libererListe(ClientNode *tete);

#define FICHIER_COMPTES "comptesClients.txt"

// ==========================================
// 1. AFFICHAGE ET STYLE
// ==========================================

void afficherCompte(CompteClientNode *c) {
    if (!c) return;

    // --- EN-TÊTE DE LA FICHE ---
    printf("\n");
    espaceCentre(); printf(CYAN "╔══════════════════════════════════════════╗" RESET "\n");
    espaceCentre(); printf(CYAN "║" RESET BLANC_GRAS "            DETAILS DU COMPTE             " RESET CYAN "║" RESET "\n");
    espaceCentre(); printf(CYAN "╠══════════════════════════════════════════╣" RESET "\n");

    // --- CORPS DE LA FICHE ---
    // On aligne les étiquettes à gauche pour un look "formulaire"
    espaceCentre(); printf(CYAN "║" RESET "  ID Client : " BLANC_GRAS "%-28d" RESET CYAN "║" RESET "\n", c->compte.client.ID);

    espaceCentre(); printf(CYAN "║" RESET "  Titulaire : " BLANC_GRAS "%-12s %-15s" RESET CYAN "║" RESET "\n",
                           c->compte.client.prenom, c->compte.client.nom);

    espaceCentre(); printf(CYAN "║" RESET "  CIN       : " BLANC_GRAS "%-28s" RESET CYAN "║" RESET "\n", c->compte.client.cin);

    espaceCentre(); printf(CYAN "║" RESET "  Telephone : " BLANC_GRAS "%-28s" RESET CYAN "║" RESET "\n", c->compte.client.telephone);

    espaceCentre(); printf(CYAN "║" RESET "  Email     : " BLANC_GRAS "%-28s" RESET CYAN "║" RESET "\n", c->compte.client.email);

    // --- SEPARATEUR INTERNE ---
    espaceCentre(); printf(CYAN "╟──────────────────────────────────────────╢" RESET "\n");

    // --- SOLDE (En évidence) ---
    espaceCentre(); printf(CYAN "║" RESET "  SOLDE ACTUEL : " VERT "%-21.2f DH" RESET CYAN " ║" RESET "\n", c->compte.solde);

    // --- PIED DE FICHE ---
    espaceCentre(); printf(CYAN "╚══════════════════════════════════════════╝" RESET "\n");
}

// ==========================================
// 2. VALIDATION ET INTÉGRITÉ
// ==========================================

int validerMontantCompte(float montant) {
    if (montant < 0) return 0;
    const float MAX_MONTANT = 100000000.0f;
    return (montant <= MAX_MONTANT);
}

int compteExiste(int idClient) {
    CompteClientNode *liste = chargerComptesFichier();
    CompteClientNode *c = chercherCompte(liste, idClient);
    int existe = (c != NULL);
    libererListeComptes(liste);
    return existe;
}

int verifierIntegriteComptes(ClientNode *clients, CompteClientNode *comptes) {
    if (!comptes) return 1;
    int erreurs = 0;
    printf("\n" BLEU "Verification d'integrite en cours..." RESET "\n");

    CompteClientNode *c = comptes;
    while (c) {
        Client clientRef = chercherClientParID(c->compte.client.ID);
        if (clientRef.ID == 0) {
            printf(ROUGE " Erreur : Compte ID %d lie a un client inexistant." RESET "\n", c->compte.client.ID);
            erreurs++;
        }
        if (!validerMontantCompte(c->compte.solde)) {
            printf(ROUGE " Erreur : Solde invalide pour client ID %d." RESET "\n", c->compte.client.ID);
            erreurs++;
        }
        c = c->next;
    }

    if (erreurs == 0) printf(VERT " Aucune erreur d'intégrite detectee." RESET "\n");
    else printf(ROUGE "%d erreur(s) d'integrite trouvee(s)." RESET "\n", erreurs);
    return (erreurs == 0);
}

// ==========================================
// 3. LOGIQUE DE LISTE ET FICHIERS
// ==========================================

CompteClientNode* chargerComptesFichier(void) {
    FILE *f = fopen(FICHIER_COMPTES, "r");
    if (!f) return NULL;

    CompteClientNode *tete = NULL, *queue = NULL;
    while (1) {
        CompteClientNode *node = malloc(sizeof(CompteClientNode));
        if (!node) break;

        if (fscanf(f, "%d %s %s %s %s %s %f %s\n",
                   &node->compte.client.ID, node->compte.client.cin,
                   node->compte.client.nom, node->compte.client.prenom,
                   node->compte.client.telephone, node->compte.client.email,
                   &node->compte.solde, node->compte.motDePasse) != 8) {
            free(node);
            break;
        }
        node->next = NULL;
        if (!tete) tete = queue = node;
        else { queue->next = node; queue = node; }
    }
    fclose(f);
    return tete;
}

int sauvegarderComptesFichier(CompteClientNode *tete) {
    FILE *f = fopen(FICHIER_COMPTES, "w");
    if (!f) return 0;

    while (tete) {
        fprintf(f, "%d %s %s %s %s %s %.2f %s\n",
                tete->compte.client.ID, tete->compte.client.cin,
                tete->compte.client.nom, tete->compte.client.prenom,
                tete->compte.client.telephone, tete->compte.client.email,
                tete->compte.solde, tete->compte.motDePasse);
        tete = tete->next;
    }
    fclose(f);
    return 1;
}

CompteClientNode* chercherCompte(CompteClientNode *tete, int idClient) {
    while (tete) {
        if (tete->compte.client.ID == idClient) return tete;
        tete = tete->next;
    }
    return NULL;
}

CompteClientNode* ajouterCompteListe(CompteClientNode *tete, CompteClientNode *compte) {
    if (!tete) return compte;
    CompteClientNode *temp = tete;
    while (temp->next) temp = temp->next;
    temp->next = compte;
    return tete;
}

void libererListeComptes(CompteClientNode *tete) {
    CompteClientNode *tmp;
    while (tete) {
        tmp = tete;
        tete = tete->next;
        free(tmp);
    }
}

// ==========================================
// 4. OPÉRATIONS (DÉPÔT / RETRAIT / SUPPRIMER)
// ==========================================

int deposer(CompteClientNode *compte, float montant) {
    if (!compte || !validerMontantCompte(montant)) return 0;
    compte->compte.solde += montant;
    return 1;
}

int retirer(CompteClientNode *compte, float montant) {
    if (!compte || !validerMontantCompte(montant) || compte->compte.solde < montant) return 0;
    compte->compte.solde -= montant;
    return 1;
}

int supprimerCompte(int idClient) {
    CompteClientNode *liste = chargerComptesFichier();
    if (!liste) return 1;

    CompteClientNode *temp = liste, *prev = NULL;
    int trouve = 0;

    while (temp) {
        if (temp->compte.client.ID == idClient) {
            trouve = 1;
            if (!prev) liste = temp->next;
            else prev->next = temp->next;
            free(temp);
            break;
        }
        prev = temp; temp = temp->next;
    }

    if (trouve) sauvegarderComptesFichier(liste);
    libererListeComptes(liste);
    return trouve;
}

// ==========================================
// 5. MENUS DE GESTION
// ==========================================

void creerCompteMenu(void) {
    int id;
    printf("\n"CYAN "=== CREATION DE COMPTE ===" RESET "\n");
    printf(GRAS "ID client : " RESET);
    scanf("%d", &id);

    if (compteExiste(id)) {
        printf(ROUGE "  Un compte existe deja pour ce client." RESET "\n");
        return;
    }

    Client c = chercherClientParID(id);
    if (c.ID == 0) {
        printf(ROUGE " Client introuvable." RESET "\n");
        return;
    }

    CompteClientNode *node = malloc(sizeof(CompteClientNode));
    node->compte.client = c;
    node->compte.solde = 0.0f;
    printf("Definir mot de passe : ");
    scanf("%49s", node->compte.motDePasse);
    node->next = NULL;

    CompteClientNode *liste = chargerComptesFichier();
    liste = ajouterCompteListe(liste, node);
    sauvegarderComptesFichier(liste);
    libererListeComptes(liste);
    printf(VERT " Compte cree avec succes." RESET "\n");
}

void deposerMenu(void) {
    int id; float montant;
    printf(GRAS "ID client : " RESET);
    scanf("%d", &id);
    CompteClientNode *liste = chargerComptesFichier();
    CompteClientNode *c = chercherCompte(liste, id);
    if (!c) { printf(ROUGE " Compte introuvable." RESET "\n"); libererListeComptes(liste); return; }

    printf("Montant a deposer : ");
    scanf("%f", &montant);
    if (deposer(c, montant)) {
        sauvegarderComptesFichier(liste);
        printf(VERT "Nouveau solde : %.2f DH" RESET "\n", c->compte.solde);
    } else printf(ROUGE "Montant invalide." RESET "\n");
    libererListeComptes(liste);
}

void retirerMenu(void) {
    int id; float montant;
    printf(GRAS "ID client : " RESET);
    scanf("%d", &id);
    CompteClientNode *liste = chargerComptesFichier();
    CompteClientNode *c = chercherCompte(liste, id);
    if (!c) { printf(ROUGE " Compte introuvable." RESET "\n"); libererListeComptes(liste); return; }

    printf("Montant a retirer : ");
    scanf("%f", &montant);
    if (retirer(c, montant)) {
        sauvegarderComptesFichier(liste);
        printf(VERT " Retrait effectue. Nouveau solde : %.2f DH" RESET "\n", c->compte.solde);
    } else printf(ROUGE " Solde insuffisant ou montant invalide." RESET "\n");
    libererListeComptes(liste);
}

void afficherStatistiquesComptes(void) {
    CompteClientNode *liste = chargerComptesFichier();
    if (!liste) return;

    int nb = 0; float total = 0;
    CompteClientNode *tmp = liste;
    while (tmp) { nb++; total += tmp->compte.solde; tmp = tmp->next; }

    printf("\n" GRAS "==========================================" RESET);
    printf("\n" CYAN "          STATISTIQUES COMPTES             " RESET);
    printf("\n" GRAS "==========================================" RESET);
    printf("\n  Nombre de comptes : %d", nb);
    printf("\n  Solde Total       : " VERT "%.2f DH" RESET, total);
    printf("\n  Solde Moyen       : %.2f DH", (nb > 0) ? total/nb : 0);
    printf("\n" GRAS "==========================================" RESET "\n");
    libererListeComptes(liste);
}

void menuGestionComptes(void) {
    int choix;
    do {
        clear();

        // --- EN-TÊTE CYAN ---
        printf(CYAN);
        espaceCentre(); printf("█▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀█\n");
        espaceCentre(); printf("█           GESTION DES COMPTES          █\n");
        espaceCentre(); printf("█▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄█\n");
        printf(RESET);

        printf("\n");

        // --- OPTIONS EN BLANC ---
        printf(BLANC_GRAS);
        espaceCentre(); printf("   [1]  Lister tous les comptes\n");
        espaceCentre(); printf("   [2]  Rechercher un compte (ID)\n");
        espaceCentre(); printf("   [3]  Creer un nouveau compte\n");
        espaceCentre(); printf("   [4]  Modifier un compte existant\n");
        espaceCentre(); printf("   [5]  Effectuer un DEPOT\n");
        espaceCentre(); printf("   [6]  Effectuer un RETRAIT\n");
        espaceCentre(); printf("   [7]  Statistiques des comptes\n");
        espaceCentre(); printf("   [8]  Verifier l'integrite du systeme\n");

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
        espaceCentre(); printf("  ➤    votre choix : ");
        printf(RESET);

        if (scanf("%d", &choix) != 1) { while (getchar() != '\n'); choix = -1; }
        while (getchar() != '\n'); // Nettoyage du buffer

        switch (choix) {
            case 1: {
                CompteClientNode *l = chargerComptesFichier();
                CompteClientNode *t = l;
                while(t) { afficherCompte(t); t = t->next; }
                libererListeComptes(l);
                printf("\nAppuyez sur Entrer pour continuer..."); getchar();
                break;
            }
            case 2: {
                int id;
                printf(CYAN "      ➤ Entrez l'ID du compte : " RESET);
                scanf("%d", &id); getchar();
                CompteClientNode *l = chargerComptesFichier();
                CompteClientNode *c = chercherCompte(l, id);
                if(c) afficherCompte(c);
                else printf(ROUGE "      [!] Compte introuvable.\n" RESET);
                libererListeComptes(l);
                printf("\nAppuyez sur Entrer..."); getchar();
                break;
            }
            case 3: creerCompteMenu(); break;
            case 5: deposerMenu(); break;
            case 6: retirerMenu(); break;
            case 7:
                afficherStatistiquesComptes();
                printf("\nAppuyez sur Entrer..."); getchar();
                break;
            case 8: {
                ClientNode *cl = listerClientsFichier();
                CompteClientNode *co = chargerComptesFichier();
                verifierIntegriteComptes(cl, co);
                libererListe(cl); libererListeComptes(co);
                printf("\nAppuyez sur Entrer..."); getchar();
                break;
            }
            case 0: break;
            default:
                printf(ROUGE "\n      [!] Choix invalide.\n" RESET);
                printf("      Appuyez sur Entrer...");
                getchar();
                break;
        }
    } while (choix != 0);
}
