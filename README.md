# Bank Credit Management System (C)

## Overview
This project is a **Bank Credit Management System developed in C**.  
The application simulates a simplified banking environment where administrators and clients can manage credit-related operations.

The system is implemented using **modular programming**, where each module is responsible for a specific banking function.

The application runs in a **console interface** and manages data using files.

---

## Features

### Admin Features
- Add, modify and delete clients
- Manage client accounts
- Process credit requests
- Approve or reject credit applications
- Manage credits
- Manage payments
- Generate financial statistics

### Client Features
- Login to personal space
- View personal information
- View credits
- Submit credit requests
- View payment history

---

## Project Structure
Bank-Credit-Management-System/
│
├── main.c
├── client.c
├── client.h
├── compteClient.c
├── compteClient.h
├── credit.c
├── credit.h
├── demande.c
├── demande.h
├── paiement.c
├── paiement.h
├── statistique.c
├── statistique.h
├── espaceAdmin.c
├── espaceAdmin.h
├── espaceClientComplet.c
├── espaceClientComplet.h
└── style.h


---

## Modules Description

### Client Module
Manages bank clients:
- add client
- update client
- delete client
- search client
- display clients

### Account Module
Manages client bank accounts.

### Credit Request Module
Handles credit requests from clients.

### Credit Module
Manages approved credits and calculates installments.

### Payment Module
Handles loan repayment operations.

### Statistics Module
Generates statistics related to credits, payments, and requests.

---

## Technologies Used

- C Programming Language
- Modular Programming
- File Handling
- Console Interface

---

## Compilation

Compile the program using GCC:


gcc *.c -o banque


---

## Run the Program


./banque


---

## Learning Outcomes

This project helped practice:

- Modular programming in C
- File management
- Software architecture
- Banking system logic

---

## Authors

- Zahra Fouima


Engineering Cycle – Computer Science
