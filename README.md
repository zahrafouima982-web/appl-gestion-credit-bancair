# 🏦 Système de Gestion de Crédits Bancaires - ENSAH

Ce projet est une application robuste développée en **Langage C**. Elle permet d'automatiser la gestion des clients, le traitement des demandes de crédits et le suivi des remboursements au sein d'une agence bancaire.

## 👤 Réalisé par
* **Zahra Fouima**
* **Encadré par** : Mr. BAHRI, Abdelkhalak
* **Année Universitaire** : 2025/2026

---

## 🚀 Fonctionnalités Principales

### 🔐 Espace Administrateur
* **Gestion des comptes** : Création, modification et suppression des comptes clients.
* **Validation** : Étude et approbation des demandes de crédits.
* **Reporting** : Consultation des statistiques globales de la banque.

### 👥 Espace Client
* **Consultation** : Visualisation du solde et des informations personnelles.
* **Demandes** : Formulation de nouvelles demandes de crédit.
* **Suivi** : Historique des paiements et capital restant dû.

---

## 📂 Architecture Modulaire du Code
Le projet est structuré de manière professionnelle avec une séparation claire des responsabilités :

* **`main.c`** : Point d'entrée de l'application et menu principal.
* **`compteClient.c / .h`** : Logique de gestion des comptes bancaires.
* **`credit.c / .h`** : Moteur financier (calcul d'intérêts, mensualités).
* **`paiement.c / .h`** : Gestion des transactions et échéanciers.
* **`statistique.c / .h`** : Analyse des données et rapports d'activité.
* **`style.h`** : Fonctions d'affichage pour une interface console ergonomique.

---

## 🛠️ Installation et Exécution
1. Clonez le dépôt :
   ```bash
   git clone [https://github.com/zahrafouima982-web/appl-gestion-credit-bancair.git](https://github.com/zahrafouima982-web/appl-gestion-credit-bancair.git)
