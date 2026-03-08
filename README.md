# 🏦 Système de Gestion de Crédits Bancaires - ENSAH

Ce projet est une application complète en **Langage C** développée pour automatiser la gestion des opérations bancaires. Elle propose une interface structurée pour les administrateurs et les clients.

## 👤 Réalisé par :
* **Zahra Fouima**
* **Encadré par** : Mr. BAHRI, Abdelkhalak
* **Établissement** : ENSA d'Al Hoceima (2025/2026)

---

## 📂 Architecture Modulaire du Projet
L'application est découpée en modules spécifiques pour une gestion optimale du code :

### 🕹️ Cœur du Système
* **`main.c`** : Point d'entrée, gestion du lancement et menus principaux.
* **`style.h`** : Fonctions d'affichage, couleurs et ergonomie de la console.

### 👥 Gestion des Utilisateurs et Comptes
* **`client.c / .h`** : Fonctions de base liées aux clients.
* **`compteClient.c / .h`** : Gestion détaillée des comptes bancaires et soldes.
* **`authentification.c / .h`** : (Si présent) Sécurisation des accès aux espaces.

### 🏢 Espaces Dédiés
* **`espaceAdmin.c / .h`** : Interface et outils réservés à l'administrateur de la banque.
* **`espaceClientComplet.c / .h`** : Interface utilisateur permettant au client de gérer son compte.

### 💰 Gestion des Crédits
* **`demande.c / .h`** : Gestion des formulaires et du dépôt des demandes de crédit.
* **`credit.c / .h`** : Moteur de calcul financier (mensualités, taux d'intérêt).
* **`paiement.c / .h`** : Suivi des remboursements et historique des transactions.

### 📊 Analyse
* **`statistique.c / .h`** : Génération de rapports et statistiques sur l'activité bancaire.

---

## 🛠️ Compilation
Pour compiler l'intégralité du projet avec GCC :
```bash
gcc *.c -o gestion_credit
