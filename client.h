#ifndef CLIENT_H
#define CLIENT_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#define CLIENT_FILE "client(1).txt"

typedef struct {
    int ID;
    char cin[20];
    char nom[50];
    char prenom[50];
    char telephone[11];
    char email[50];
} Client;

typedef struct ClientNode {
    Client data;
    struct ClientNode *next;
} ClientNode;

void ajouterClient(void);
void modifierClient(void);
void supprimerClient(void);
void rechercherClient(void);
void listerClients(void);
void trierClients(void);
ClientNode* insererTrieCritere(ClientNode *listeTriee, ClientNode *nouvelElement, int critere);
void menuGestionClients(void);

// Cherche un client par ID et retourne la structure Client
Client chercherClientParID(int idClient);


#endif
