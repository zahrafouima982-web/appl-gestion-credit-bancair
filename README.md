# 🏦 Système de Gestion de Crédits Bancaires — ENSAH

Ce projet consiste en le développement d'une application de gestion bancaire robuste et modulaire en **Langage C**, réalisée dans le cadre du cycle ingénieur. L'application automatise les opérations critiques liées aux clients, aux comptes, aux crédits et aux remboursements.

## 👤 Réalisé par :
* **Zahra Fouima**
* **Encadré par** : Mr. BAHRI, Abdelkhalak
* **Établissement** : ENSA d'Al Hoceima (2025/2026)

---

## ✨ Fonctionnalités Principales

### 👤 Espace Administrateur
* **Authentification sécurisée** : Accès protégé pour l'administration.
* **Gestion des Clients** : Ajout, modification, suppression et recherche de clients.
* **Gestion des Comptes** : Création et suivi des soldes bancaires et des informations personnelles.
* **Traitement des Crédits** : Analyse, approbation ou refus des demandes de prêt.
* **Statistiques Globales** : Rapports sur le capital prêté et la rentabilité.

### 🔑 Espace Client
* **Consultation personnelle** : Accès sécurisé aux informations de profil et de solde.
* **Suivi de Crédit** : Consultation de l'état des crédits et de l'historique des paiements.
* **Demande de Crédit** : Possibilité pour le client de soumettre une demande de prêt.

---

## 🏗️ Architecture du Projet

L'application adopte une approche de **programmation modulaire** pour une maintenance et une clarté optimales.

* **Point d'entrée** : `main.c`.
* **Gestion Clients** : `client.c/.h`, `compteClient.c/.h`.
* **Gestion Crédits** : `demande.c/.h`, `credit.c/.h`, `paiement.c/.h`.
* **Espaces Dédiés** : `espaceAdmin.c/.h`, `espaceClientComplet.c/.h`.
* **Utilitaires & Analyse** : `style.h`, `statistique.c/.h`, `authentification.c/.h`.

---

## 🛠️ Spécifications Techniques

* **Langage** : C.
* **Persistance** : Stockage des données via des fichiers texte (`.txt`).
* **Interface** : Mode console ergonomique avec codes couleurs et nettoyage d'écran.

---

## 🚀 Installation et Compilation

1. **Clonage du projet** :
   ```bash
   git clone [https://github.com/zahrafouima982-web/appl-gestion-credit-bancair.git](https://github.com/zahrafouima982-web/appl-gestion-credit-bancair.git)
