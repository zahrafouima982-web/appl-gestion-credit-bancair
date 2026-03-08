# 🏦 Système de Gestion des Crédits Bancaires - ENSAH

Ce projet est une application de **Core Banking** développée en **langage C**. [span_0](start_span)[span_1](start_span)Elle permet d'automatiser le cycle complet des crédits bancaires, de la gestion des comptes clients à l'analyse statistique des performances financières de la banque[span_0](end_span)[span_1](end_span).

## 👤 Informations du Projet
* **[span_2](start_span)Réalisé par** : Zahra fouima[span_2](end_span)
* **[span_3](start_span)Établissement** : École Nationale des Sciences Appliquées d'Al Hoceima (ENSAH)[span_3](end_span)
* **[span_4](start_span)Encadré par** : Mr. BAHRI, Abdelkhalak[span_4](end_span)
* **[span_5](start_span)Année Universitaire** : 2025/2026[span_5](end_span)

## 🚀 Fonctionnalités Principales

### 👨‍💼 Espace Administrateur (Gestion & Contrôle)
* **[span_6](start_span)Authentification Sécurisée** : Accès protégé par login et mot de passe[span_6](end_span).
* **[span_7](start_span)[span_8](start_span)Gestion Clientèle** : Ajout, modification, suppression et recherche de clients via listes chaînées dynamiques[span_7](end_span)[span_8](end_span).
* **[span_9](start_span)[span_10](start_span)Traitement des Crédits** : Analyse des dossiers, approbation ou refus des demandes de prêt[span_9](end_span)[span_10](end_span).
* **[span_11](start_span)[span_12](start_span)Tableau de Bord** : Visualisation des statistiques globales (taux de remboursement, capital prêté)[span_11](end_span)[span_12](end_span).

### 👤 Espace Client (Consultation & Demande)
* **[span_13](start_span)[span_14](start_span)Suivi de Compte** : Consultation du solde et des informations personnelles[span_13](end_span)[span_14](end_span).
* **[span_15](start_span)[span_16](start_span)[span_17](start_span)Gestion des Prêts** : Dépôt de nouvelles demandes de crédit et suivi en temps réel de l'état (En attente/Acceptée/Refusée)[span_15](end_span)[span_16](end_span)[span_17](end_span).
* **[span_18](start_span)[span_19](start_span)[span_20](start_span)Échéancier** : Consultation de l'historique des paiements et du capital restant dû[span_18](end_span)[span_19](end_span)[span_20](end_span).

## 🏗 Architecture Modulaire
[span_21](start_span)[span_22](start_span)Le projet adopte une approche de **programmation modulaire** pour garantir une maintenance aisée et une séparation claire des responsabilités[span_21](end_span)[span_22](end_span).

| Module | Fichiers | Rôle Principal |
| :--- | :--- | :--- |
| **Main** | `main.c` | [span_23](start_span)[span_24](start_span)Point d'entrée et menu principal[span_23](end_span)[span_24](end_span). |
| **Admin** | `espaceAdmin.c/h` | [span_25](start_span)[span_26](start_span)Logique de gestion et authentification admin[span_25](end_span)[span_26](end_span). |
| **Client** | `client.c/h` | [span_27](start_span)[span_28](start_span)Gestion de la structure Client et listes chaînées[span_27](end_span)[span_28](end_span). |
| **Comptes** | `compteClient.c/h` | [span_29](start_span)[span_30](start_span)Gestion des soldes et de la sécurité financière[span_29](end_span)[span_30](end_span). |
| **Crédit** | `credit.c/h` | [span_31](start_span)[span_32](start_span)Moteur financier (calcul des mensualités et intérêts)[span_31](end_span)[span_32](end_span). |
| **Paiement** | `paiement.c/h` | [span_33](start_span)[span_34](start_span)Suivi des remboursements et prélèvements[span_33](end_span)[span_34](end_span). |
| **Stats** | `statistique.c/h` | [span_35](start_span)[span_36](start_span)Analyse de rentabilité et rapports financiers[span_35](end_span)[span_36](end_span). |
| **Style** | `style.h` | [span_37](start_span)[span_38](start_span)Interface utilisateur (couleurs ANSI, nettoyage écran)[span_37](end_span)[span_38](end_span). |

## 💾 Gestion des Données
[span_39](start_span)[span_40](start_span)L'application assure la **persistance des données** via des fichiers texte (`.txt`), permettant de conserver les informations des clients et l'historique des crédits entre chaque session[span_39](end_span)[span_40](end_span).

## 🛠️ Installation et Utilisation

1. **Cloner le projet** :
   ```bash
   git clone [https://github.com/votre-username/votre-depot.git](https://github.com/votre-username/votre-depot.git)
