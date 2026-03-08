# 👤 Rapport Technique Détaillé : Espace Client

## 📑 Table des Matières

1. [Vue d'Ensemble](#1-vue-densemble)
2. [Architecture de l'Espace Client](#2-architecture-de-lespace-client)
3. [Authentification et Sécurité](#3-authentification-et-sécurité)
4. [Fonctionnalités Détaillées](#4-fonctionnalités-détaillées)
5. [Consultation des Informations](#5-consultation-des-informations)
6. [Opérations sur le Compte](#6-opérations-sur-le-compte)
7. [Consultation des Crédits](#7-consultation-des-crédits)
8. [Consultation des Paiements](#8-consultation-des-paiements)
9. [Interface Utilisateur](#9-interface-utilisateur)
10. [Exemples d'Utilisation](#10-exemples-dutilisation)

---

## 1. Vue d'Ensemble

### 1.1 Objectif de l'Espace Client

L'espace client permet aux clients de :
- Consulter leurs informations personnelles
- Consulter leur compte bancaire (solde, historique)
- Consulter leurs crédits actifs et clôturés
- Consulter l'historique de leurs paiements
- Effectuer des dépôts et retraits sur leur compte
- Suivre l'état de leurs crédits

### 1.2 Différences avec l'Espace Admin

| Fonctionnalité | Espace Admin | Espace Client |
|----------------|--------------|---------------|
| Création de clients | ✅ | ❌ |
| Création de comptes | ✅ | ❌ |
| Création de crédits | ✅ | ❌ |
| Prélèvements automatiques | ✅ | ❌ |
| Consultation informations | ✅ | ✅ |
| Consultation compte | ✅ | ✅ |
| Consultation crédits | ✅ | ✅ |
| Consultation paiements | ✅ | ✅ |
| Dépôt/Retrait | ✅ | ✅ |
| Modification données | ✅ | ❌ (limité) |

### 1.3 Contraintes de Sécurité

- **Authentification requise** : L'accès nécessite un identifiant et un mot de passe
- **Accès limité** : Un client ne peut accéder qu'à ses propres données
- **Pas de modification** : Les clients ne peuvent pas modifier leurs informations personnelles
- **Pas de création** : Les clients ne peuvent pas créer de crédits ou comptes

---

## 2. Architecture de l'Espace Client

### 2.1 Structure du Menu Principal

```
┌─────────────────────────────────────┐
│      ESPACE CLIENT                  │
├─────────────────────────────────────┤
│  1. Consulter mes informations      │
│  2. Consulter mon compte            │
│  3. Consulter mes crédits           │
│  4. Consulter mes paiements         │
│  5. Effectuer un dépôt              │
│  6. Effectuer un retrait            │
│  0. Retour                          │
└─────────────────────────────────────┘
```

### 2.2 Flux d'Authentification

```
┌─────────────────────────────────────┐
│  Demande ID Client                  │
└─────────────────────────────────────┘
              │
              ▼
┌─────────────────────────────────────┐
│  Demande Mot de Passe               │
└─────────────────────────────────────┘
              │
              ▼
┌─────────────────────────────────────┐
│  Vérification                       │
│  ├─ Client existe ?                 │
│  └─ Mot de passe correct ?          │
└─────────────────────────────────────┘
              │
      ┌───────┴───────┐
      │               │
      ▼               ▼
┌──────────┐    ┌──────────┐
│  SUCCÈS  │    │  ÉCHEC   │
└──────────┘    └──────────┘
      │               │
      ▼               ▼
┌──────────┐    ┌──────────┐
│  Accès   │    │  Refus   │
│  Autorisé│    │  Accès   │
└──────────┘    └──────────┘
```

### 2.3 Structure de Données Utilisées

L'espace client utilise les mêmes structures que l'espace admin :
- `Client` : Informations personnelles
- `CompteClient` : Informations du compte bancaire
- `Credit` : Informations des crédits
- `Paiement` : Informations des paiements

---

## 3. Authentification et Sécurité

### 3.1 Processus d'Authentification

```c
int authentifierClient(int idClient, const char *motDePasse) {
    // 1. Charger tous les comptes
    CompteClientNode *comptes = chargerComptesFichier();
    
    // 2. Chercher le compte du client
    CompteClientNode *compte = chercherCompte(comptes, idClient);
    
    if (!compte) {
        libererListeComptes(comptes);
        return 0;  // Client introuvable
    }
    
    // 3. Vérifier le mot de passe
    if (strcmp(compte->compte.motDePasse, motDePasse) != 0) {
        libererListeComptes(comptes);
        return 0;  // Mot de passe incorrect
    }
    
    libererListeComptes(comptes);
    return 1;  // Authentification réussie
}
```

### 3.2 Menu d'Authentification

```c
int menuAuthentificationClient(void) {
    int idClient;
    char motDePasse[50];
    
    printf("\n=== AUTHENTIFICATION CLIENT ===\n");
    printf("ID Client : ");
    scanf("%d", &idClient);
    
    printf("Mot de passe : ");
    // Masquer le mot de passe à l'affichage (si possible)
    scanf("%s", motDePasse);
    
    if (authentifierClient(idClient, motDePasse)) {
        printf("✓ Authentification réussie.\n");
        return idClient;
    } else {
        printf("✗ Authentification échouée. ID ou mot de passe incorrect.\n");
        return -1;
    }
}
```

### 3.3 Sécurité des Données

**Restrictions d'Accès** :
- Un client ne peut accéder qu'à ses propres données
- Vérification systématique de l'ID client à chaque opération
- Pas d'accès aux données d'autres clients

**Protection du Mot de Passe** :
- Stockage en clair dans le fichier (amélioration possible : hash)
- Masquage à la saisie (si supporté par le terminal)
- Limitation du nombre de tentatives (à implémenter)

---

## 4. Fonctionnalités Détaillées

### 4.1 Menu Principal Client

```c
void menuEspaceClient(int idClient) {
    int choix;
    
    do {
        printf("\n=== ESPACE CLIENT ===\n");
        printf("Bienvenue, Client ID %d\n", idClient);
        printf("─────────────────────────────────────\n");
        printf("1. Consulter mes informations\n");
        printf("2. Consulter mon compte\n");
        printf("3. Consulter mes crédits\n");
        printf("4. Consulter mes paiements\n");
        printf("5. Effectuer un dépôt\n");
        printf("6. Effectuer un retrait\n");
        printf("0. Retour\n");
        printf("Votre choix : ");
        
        if (scanf("%d", &choix) != 1) {
            while (getchar() != '\n');
            choix = -1;
        }
        
        switch (choix) {
            case 1:
                consulterInformationsClient(idClient);
                break;
            case 2:
                consulterCompteClient(idClient);
                break;
            case 3:
                consulterCreditsClient(idClient);
                break;
            case 4:
                consulterPaiementsClient(idClient);
                break;
            case 5:
                deposerClient(idClient);
                break;
            case 6:
                retirerClient(idClient);
                break;
            case 0:
                break;
            default:
                printf("Choix invalide. Réessayez.\n");
        }
    } while (choix != 0);
}
```

---

## 5. Consultation des Informations

### 5.1 Affichage des Informations Personnelles

```c
void consulterInformationsClient(int idClient) {
    // Charger les clients
    ClientNode *clients = listerClientsFichier();
    
    // Chercher le client
    ClientNode *clientNode = rechercherClientParID(clients, idClient);
    
    if (!clientNode) {
        printf("Erreur : Client introuvable.\n");
        libererListe(clients);
        return;
    }
    
    Client c = clientNode->data;
    
    printf("\n=== MES INFORMATIONS ===\n");
    printf("─────────────────────────────────────\n");
    printf("ID Client      : %d\n", c.ID);
    printf("CIN            : %s\n", c.cin);
    printf("Nom            : %s\n", c.nom);
    printf("Prénom         : %s\n", c.prenom);
    printf("Téléphone      : %s\n", c.telephone);
    printf("Email          : %s\n", c.email);
    printf("─────────────────────────────────────\n");
    
    libererListe(clients);
}
```

### 5.2 Format d'Affichage

```
┌─────────────────────────────────────┐
│      MES INFORMATIONS               │
├─────────────────────────────────────┤
│ ID Client      : 1                  │
│ CIN            : AB123456           │
│ Nom            : Alami               │
│ Prénom         : Ahmed              │
│ Téléphone      : +212612345678      │
│ Email          : ahmed.alami@email.com│
└─────────────────────────────────────┘
```

---

## 6. Opérations sur le Compte

### 6.1 Consultation du Compte

```c
void consulterCompteClient(int idClient) {
    // Charger les comptes
    CompteClientNode *comptes = chargerComptesFichier();
    
    // Chercher le compte
    CompteClientNode *compte = chercherCompte(comptes, idClient);
    
    if (!compte) {
        printf("Erreur : Compte introuvable.\n");
        libererListeComptes(comptes);
        return;
    }
    
    printf("\n=== MON COMPTE ===\n");
    printf("─────────────────────────────────────\n");
    printf("Client         : %s %s\n",
           compte->compte.client.prenom,
           compte->compte.client.nom);
    printf("ID Client      : %d\n", compte->compte.client.ID);
    printf("Solde actuel   : %.2f DH\n", compte->compte.solde);
    printf("─────────────────────────────────────\n");
    
    // Afficher les crédits actifs pour contexte
    CreditNode *credits = chargerCredits();
    CreditNode *c = credits;
    int nbCreditsActifs = 0;
    double mensualiteTotale = 0.0;
    
    while (c) {
        if (c->credit.idClient == idClient && c->credit.etat == 1) {
            nbCreditsActifs++;
            mensualiteTotale += c->credit.mensualite;
        }
        c = c->suiv;
    }
    
    if (nbCreditsActifs > 0) {
        printf("\n=== INFORMATIONS CRÉDITS ===\n");
        printf("Nombre de crédits actifs : %d\n", nbCreditsActifs);
        printf("Mensualité totale        : %.2f DH\n", mensualiteTotale);
        
        if (compte->compte.solde < mensualiteTotale) {
            printf("⚠ Attention : Votre solde est inférieur à la mensualité totale.\n");
        }
    }
    
    libererCredits(credits);
    libererListeComptes(comptes);
}
```

### 6.2 Effectuer un Dépôt

```c
void deposerClient(int idClient) {
    float montant;
    
    printf("\n=== DÉPÔT ===\n");
    printf("Montant à déposer (DH) : ");
    
    if (scanf("%f", &montant) != 1) {
        while (getchar() != '\n');
        printf("Erreur : Montant invalide.\n");
        return;
    }
    
    // Validation du montant
    if (montant <= 0) {
        printf("Erreur : Le montant doit être positif.\n");
        return;
    }
    
    if (montant > MAX_MONTANT) {
        printf("Erreur : Montant trop élevé (maximum : %.2f DH).\n", MAX_MONTANT);
        return;
    }
    
    // Charger les comptes
    CompteClientNode *comptes = chargerComptesFichier();
    CompteClientNode *compte = chercherCompte(comptes, idClient);
    
    if (!compte) {
        printf("Erreur : Compte introuvable.\n");
        libererListeComptes(comptes);
        return;
    }
    
    // Vérifier que le nouveau solde ne dépasse pas la limite
    float nouveauSolde = compte->compte.solde + montant;
    if (nouveauSolde > MAX_MONTANT) {
        printf("Erreur : Le solde maximum serait dépassé.\n");
        libererListeComptes(comptes);
        return;
    }
    
    // Effectuer le dépôt
    float soldeAvant = compte->compte.solde;
    compte->compte.solde += montant;
    
    // Sauvegarder
    if (!sauvegarderComptesFichier(comptes)) {
        printf("Erreur : Impossible de sauvegarder.\n");
        compte->compte.solde = soldeAvant;  // Restaurer en cas d'erreur
        libererListeComptes(comptes);
        return;
    }
    
    printf("\n✓ Dépôt effectué avec succès !\n");
    printf("Solde avant : %.2f DH\n", soldeAvant);
    printf("Montant déposé : %.2f DH\n", montant);
    printf("Nouveau solde : %.2f DH\n", compte->compte.solde);
    
    libererListeComptes(comptes);
}
```

### 6.3 Effectuer un Retrait

```c
void retirerClient(int idClient) {
    float montant;
    
    printf("\n=== RETRAIT ===\n");
    printf("Montant à retirer (DH) : ");
    
    if (scanf("%f", &montant) != 1) {
        while (getchar() != '\n');
        printf("Erreur : Montant invalide.\n");
        return;
    }
    
    // Validation du montant
    if (montant <= 0) {
        printf("Erreur : Le montant doit être positif.\n");
        return;
    }
    
    // Charger les comptes
    CompteClientNode *comptes = chargerComptesFichier();
    CompteClientNode *compte = chercherCompte(comptes, idClient);
    
    if (!compte) {
        printf("Erreur : Compte introuvable.\n");
        libererListeComptes(comptes);
        return;
    }
    
    // Vérifier le solde suffisant
    if (compte->compte.solde < montant) {
        printf("Erreur : Solde insuffisant.\n");
        printf("Solde actuel : %.2f DH\n", compte->compte.solde);
        printf("Montant demandé : %.2f DH\n", montant);
        
        // Avertissement si proche de la mensualité
        CreditNode *credits = chargerCredits();
        double mensualiteTotale = 0.0;
        CreditNode *c = credits;
        while (c) {
            if (c->credit.idClient == idClient && c->credit.etat == 1) {
                mensualiteTotale += c->credit.mensualite;
            }
            c = c->suiv;
        }
        libererCredits(credits);
        
        if (mensualiteTotale > 0) {
            printf("⚠ Attention : Votre mensualité totale est de %.2f DH.\n", mensualiteTotale);
        }
        
        libererListeComptes(comptes);
        return;
    }
    
    // Effectuer le retrait
    float soldeAvant = compte->compte.solde;
    compte->compte.solde -= montant;
    
    // Sauvegarder
    if (!sauvegarderComptesFichier(comptes)) {
        printf("Erreur : Impossible de sauvegarder.\n");
        compte->compte.solde = soldeAvant;  // Restaurer en cas d'erreur
        libererListeComptes(comptes);
        return;
    }
    
    printf("\n✓ Retrait effectué avec succès !\n");
    printf("Solde avant : %.2f DH\n", soldeAvant);
    printf("Montant retiré : %.2f DH\n", montant);
    printf("Nouveau solde : %.2f DH\n", compte->compte.solde);
    
    libererListeComptes(comptes);
}
```

---

## 7. Consultation des Crédits

### 7.1 Affichage des Crédits du Client

```c
void consulterCreditsClient(int idClient) {
    CreditNode *credits = chargerCredits();
    
    if (!credits) {
        printf("Erreur : Impossible de charger les crédits.\n");
        return;
    }
    
    // Filtrer les crédits du client
    CreditNode *creditsClient = NULL;
    CreditNode *tmp = credits;
    int nbCredits = 0;
    
    while (tmp) {
        if (tmp->credit.idClient == idClient) {
            nbCredits++;
        }
        tmp = tmp->suiv;
    }
    
    if (nbCredits == 0) {
        printf("\nVous n'avez aucun crédit enregistré.\n");
        libererCredits(credits);
        return;
    }
    
    printf("\n=== MES CRÉDITS ===\n");
    printf("Nombre total de crédits : %d\n", nbCredits);
    printf("─────────────────────────────────────────────────────────────────────\n");
    printf("| %-8s | %-12s | %-12s | %-12s | %-8s | %-8s |\n",
           "ID", "Montant", "Restant", "Mensualité", "Durée", "État");
    printf("─────────────────────────────────────────────────────────────────────\n");
    
    tmp = credits;
    int creditsActifs = 0;
    int creditsClotures = 0;
    double montantTotalEmprunte = 0.0;
    double montantTotalRestant = 0.0;
    double mensualiteTotale = 0.0;
    
    while (tmp) {
        if (tmp->credit.idClient == idClient) {
            printf("| %-8d | %-12.2lf | %-12.2lf | %-12.2lf | %-8d | %-8s |\n",
                   tmp->credit.idCredit,
                   tmp->credit.montant,
                   tmp->credit.montantRestant,
                   tmp->credit.mensualite,
                   tmp->credit.duree,
                   tmp->credit.etat == 1 ? "ACTIF" : "CLOTURE");
            
            montantTotalEmprunte += tmp->credit.montant;
            montantTotalRestant += tmp->credit.montantRestant;
            
            if (tmp->credit.etat == 1) {
                creditsActifs++;
                mensualiteTotale += tmp->credit.mensualite;
            } else {
                creditsClotures++;
            }
        }
        tmp = tmp->suiv;
    }
    
    printf("─────────────────────────────────────────────────────────────────────\n");
    
    // Résumé
    printf("\n=== RÉSUMÉ ===\n");
    printf("Crédits actifs : %d\n", creditsActifs);
    printf("Crédits clôturés : %d\n", creditsClotures);
    printf("Montant total emprunté : %.2lf DH\n", montantTotalEmprunte);
    printf("Montant total restant : %.2lf DH\n", montantTotalRestant);
    printf("Mensualité totale (crédits actifs) : %.2lf DH\n", mensualiteTotale);
    
    double montantPaye = montantTotalEmprunte - montantTotalRestant;
    double tauxRemboursement = (montantTotalEmprunte > 0) ?
        (montantPaye / montantTotalEmprunte) * 100.0 : 0.0;
    printf("Taux de remboursement global : %.2lf %%\n", tauxRemboursement);
    
    libererCredits(credits);
}
```

### 7.2 Détail d'un Crédit Spécifique

```c
void consulterDetailCreditClient(int idClient, int idCredit) {
    CreditNode *credits = chargerCredits();
    CreditNode *creditNode = rechercherCreditParId(credits, idCredit);
    
    if (!creditNode) {
        printf("Erreur : Crédit introuvable.\n");
        libererCredits(credits);
        return;
    }
    
    // Vérifier que le crédit appartient au client
    if (creditNode->credit.idClient != idClient) {
        printf("Erreur : Ce crédit ne vous appartient pas.\n");
        libererCredits(credits);
        return;
    }
    
    Credit c = creditNode->credit;
    
    printf("\n=== DÉTAIL DU CRÉDIT ID %d ===\n", idCredit);
    printf("─────────────────────────────────────\n");
    printf("Montant initial      : %.2lf DH\n", c.montant);
    printf("Montant restant      : %.2lf DH\n", c.montantRestant);
    printf("Taux d'intérêt       : %.2lf %%\n", c.taux);
    printf("Durée                : %d mois\n", c.duree);
    printf("Mensualité           : %.2lf DH\n", c.mensualite);
    printf("Date de création     : %s\n", c.dateCreation);
    printf("Date de début        : %s\n", c.date_debut);
    printf("État                 : %s\n", c.etat == 1 ? "ACTIF" : "CLOTURE");
    
    double montantPaye = c.montant - c.montantRestant;
    double tauxRemboursement = (c.montant > 0) ?
        (montantPaye / c.montant) * 100.0 : 0.0;
    
    printf("Montant payé         : %.2lf DH\n", montantPaye);
    printf("Taux de remboursement: %.2lf %%\n", tauxRemboursement);
    printf("─────────────────────────────────────\n");
    
    // Afficher les paiements de ce crédit
    PaiementNode *paiements = chargerPaiementsFichier();
    PaiementNode *p = paiements;
    int nbPayes = 0, nbEnAttente = 0, nbEchoues = 0;
    
    printf("\n=== PAIEMENTS ===\n");
    printf("| %-4s | %-12s | %-12s | %-12s | %-8s |\n",
           "Mois", "Date prévue", "Date paiement", "Montant", "Statut");
    printf("─────────────────────────────────────────────────────────────\n");
    
    while (p) {
        if (p->data.IDCredit == idCredit) {
            const char *statutStr;
            if (p->data.statut == STATUT_PAYE) {
                statutStr = "PAYE";
                nbPayes++;
            } else if (p->data.statut == STATUT_ATTENTE) {
                statutStr = "ATTENTE";
                nbEnAttente++;
            } else {
                statutStr = "ECHEC";
                nbEchoues++;
            }
            
            printf("| %-4d | %-12s | %-12s | %-12.2f | %-8s |\n",
                   p->data.numMois,
                   p->data.datePrevue,
                   (strlen(p->data.datePaiement) > 0 ? p->data.datePaiement : "-"),
                   p->data.montant,
                   statutStr);
        }
        p = p->next;
    }
    
    printf("─────────────────────────────────────────────────────────────\n");
    printf("Payés : %d | En attente : %d | Échoués : %d\n",
           nbPayes, nbEnAttente, nbEchoues);
    
    libererListePaiements(paiements);
    libererCredits(credits);
}
```

---

## 8. Consultation des Paiements

### 8.1 Affichage de Tous les Paiements

```c
void consulterPaiementsClient(int idClient) {
    // Charger les crédits du client pour obtenir les IDs de crédits
    CreditNode *credits = chargerCredits();
    
    // Créer une liste des IDs de crédits du client
    int *idsCredits = NULL;
    int nbCredits = 0;
    int capacite = 0;
    
    CreditNode *c = credits;
    while (c) {
        if (c->credit.idClient == idClient) {
            if (nbCredits >= capacite) {
                capacite = (capacite == 0) ? 16 : capacite * 2;
                idsCredits = realloc(idsCredits, capacite * sizeof(int));
            }
            idsCredits[nbCredits++] = c->credit.idCredit;
        }
        c = c->suiv;
    }
    
    if (nbCredits == 0) {
        printf("\nVous n'avez aucun crédit, donc aucun paiement.\n");
        libererCredits(credits);
        return;
    }
    
    // Charger tous les paiements
    PaiementNode *paiements = chargerPaiementsFichier();
    
    // Filtrer les paiements des crédits du client
    PaiementNode *p = paiements;
    int nbPaiements = 0;
    
    printf("\n=== MES PAIEMENTS ===\n");
    printf("─────────────────────────────────────────────────────────────────────────────\n");
    printf("| %-4s | %-8s | %-12s | %-12s | %-12s | %-8s | %-10s |\n",
           "Mois", "Crédit", "Date prévue", "Date paiement", "Montant", "Statut", "Pénalité");
    printf("─────────────────────────────────────────────────────────────────────────────\n");
    
    int nbPayes = 0, nbEnAttente = 0, nbEchoues = 0;
    float montantTotalPaye = 0.0;
    float montantTotalEnAttente = 0.0;
    float montantTotalEchoue = 0.0;
    float penalitesTotales = 0.0;
    
    while (p) {
        // Vérifier si ce paiement appartient à un crédit du client
        int appartient = 0;
        for (int i = 0; i < nbCredits; i++) {
            if (p->data.IDCredit == idsCredits[i]) {
                appartient = 1;
                break;
            }
        }
        
        if (appartient) {
            nbPaiements++;
            
            const char *statutStr;
            if (p->data.statut == STATUT_PAYE) {
                statutStr = "PAYE";
                nbPayes++;
                montantTotalPaye += p->data.montant;
            } else if (p->data.statut == STATUT_ATTENTE) {
                statutStr = "ATTENTE";
                nbEnAttente++;
                montantTotalEnAttente += p->data.montant;
            } else {
                statutStr = "ECHEC";
                nbEchoues++;
                montantTotalEchoue += p->data.montant;
                penalitesTotales += p->data.montantPenalite;
            }
            
            printf("| %-4d | %-8d | %-12s | %-12s | %-12.2f | %-8s | %-10.2f |\n",
                   p->data.numMois,
                   p->data.IDCredit,
                   p->data.datePrevue,
                   (strlen(p->data.datePaiement) > 0 ? p->data.datePaiement : "-"),
                   p->data.montant,
                   statutStr,
                   p->data.montantPenalite);
        }
        
        p = p->next;
    }
    
    printf("─────────────────────────────────────────────────────────────────────────────\n");
    
    // Résumé
    printf("\n=== RÉSUMÉ ===\n");
    printf("Total des paiements : %d\n", nbPaiements);
    printf("Paiements payés : %d (%.2f DH)\n", nbPayes, montantTotalPaye);
    printf("Paiements en attente : %d (%.2f DH)\n", nbEnAttente, montantTotalEnAttente);
    printf("Paiements échoués : %d (%.2f DH)\n", nbEchoues, montantTotalEchoue);
    printf("Pénalités totales : %.2f DH\n", penalitesTotales);
    
    // Avertissements
    if (nbEchoues > 0) {
        printf("\n⚠ Attention : Vous avez %d paiement(s) en échec.\n", nbEchoues);
        printf("Ces paiements doivent être réglés pour éviter des pénalités supplémentaires.\n");
    }
    
    if (montantTotalEnAttente > 0) {
        CompteClientNode *comptes = chargerComptesFichier();
        CompteClientNode *compte = chercherCompte(comptes, idClient);
        
        if (compte) {
            if (compte->compte.solde < montantTotalEnAttente) {
                printf("\n⚠ Attention : Votre solde (%.2f DH) est inférieur aux paiements en attente (%.2f DH).\n",
                       compte->compte.solde, montantTotalEnAttente);
                printf("Pensez à faire un dépôt avant les dates d'échéance.\n");
            }
        }
        
        libererListeComptes(comptes);
    }
    
    free(idsCredits);
    libererListePaiements(paiements);
    libererCredits(credits);
}
```

### 8.2 Filtrage par Statut

```c
void consulterPaiementsParStatut(int idClient, StatutPaiement statut) {
    // Même logique que consulterPaiementsClient mais avec filtre supplémentaire
    // sur le statut
}
```

---

## 9. Interface Utilisateur

### 9.1 Messages et Couleurs

L'espace client utilise le même système de couleurs que l'espace admin :
- **Vert** : Opérations réussies
- **Rouge** : Erreurs
- **Jaune** : Avertissements
- **Cyan** : Informations
- **Magenta** : Titres

### 9.2 Format des Tableaux

Tous les tableaux suivent un format cohérent avec :
- En-têtes alignés
- Bordures visuelles
- Colonnes de largeur fixe
- Données formatées

---

## 10. Exemples d'Utilisation

### 10.1 Scénario Complet : Session Client

```
1. Authentification
   └─> ID Client : 1
   └─> Mot de passe : pass123
   └─> ✓ Authentification réussie

2. Consultation du compte
   └─> Solde actuel : 5000 DH
   └─> 2 crédits actifs
   └─> Mensualité totale : 2059.12 DH

3. Consultation des crédits
   └─> Crédit ID 1 : 12000 DH, restant : 8000 DH, ACTIF
   └─> Crédit ID 3 : 5000 DH, restant : 3000 DH, ACTIF

4. Consultation des paiements
   └─> 24 paiements au total
   └─> 18 payés, 4 en attente, 2 échoués
   └─> ⚠ Attention : 2 paiements en échec

5. Dépôt de 3000 DH
   └─> Solde avant : 5000 DH
   └─> Nouveau solde : 8000 DH
   └─> ✓ Dépôt effectué avec succès

6. Consultation des paiements en attente
   └─> 4 paiements à venir
   └─> Dates : 2024-04-15, 2024-05-15, 2024-06-15, 2024-07-15
   └─> Montant total : 2059.12 DH
   └─> Solde suffisant ✓
```

### 10.2 Scénario : Gestion d'un Paiement en Retard

```
1. Client consulte ses paiements
   └─> Découvre 1 paiement en échec (Crédit ID 1, Mois 3)

2. Client consulte son compte
   └─> Solde actuel : 500 DH
   └─> Montant du paiement en échec : 1029.56 DH
   └─> Solde insuffisant

3. Client effectue un dépôt
   └─> Montant déposé : 1000 DH
   └─> Nouveau solde : 1500 DH
   └─> ✓ Dépôt effectué

4. Client informe l'admin (ou attend le prochain prélèvement automatique)
   └─> Le paiement sera traité lors du prochain prélèvement automatique
   └─> Ou l'admin peut effectuer un paiement manuel
```

---

## 11. Points Techniques Importants

### 11.1 Isolation des Données

- **Vérification systématique** : Chaque fonction vérifie que l'ID client correspond
- **Pas d'accès croisé** : Un client ne peut pas accéder aux données d'un autre client
- **Filtrage** : Toutes les requêtes sont filtrées par ID client

### 11.2 Performance

- **Chargement optimisé** : Les données sont chargées une seule fois par session
- **Filtrage en mémoire** : Le filtrage se fait après chargement
- **Libération mémoire** : Toutes les structures sont libérées après utilisation

### 11.3 Sécurité

- **Authentification obligatoire** : Pas d'accès sans authentification
- **Vérification d'appartenance** : Vérification que les données appartiennent au client
- **Validation des montants** : Tous les montants sont validés avant traitement

---

Ce rapport détaille tous les aspects de l'espace client, de l'authentification aux opérations sur le compte, en passant par la consultation des crédits et paiements.

