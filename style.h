#ifndef STYLE_H
#define STYLE_H

#include <stdio.h>
#include <stdlib.h>

// Couleurs
#define RESET "\033[0m"
#define ROUGE "\033[31m"
#define VERT "\033[32m"
#define JAUNE "\033[33m"
#define BLEU "\033[34m"
#define CYAN "\033[36m"
#define MAGENTA "\033[35m"
#define BLANC_GRAS "\033[1;37m"
#define GRAS "\033[1m"

// Effacer écran
static inline void clear(void) {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}
static inline void espaceCentre() {
    printf("                "); // espace pour centrer
}
// Titre simple
static inline void titre(const char* texte) {
    printf("\n" BLANC_GRAS "======== %s ========" RESET "\n", texte);
}

// Cadre décoratif
static inline void cadre() {
    printf( ""CYAN"======================================================================================="RESET"\n" );
}

// Logo BANQUE stylé
static inline void logoBanque() {

    printf(CYAN );

     espaceCentre();  printf("  ██████   █████  ███    ██  ██████  ██    ██ ███████\n");
     espaceCentre();  printf("  ██   ██ ██   ██ ████   ██ ██    ██ ██    ██ ██     \n");
      espaceCentre(); printf("  ██████  ███████ ██ ██  ██ ██    ██ ██    ██ █████  \n");
      espaceCentre(); printf("  ██   ██ ██   ██ ██  ██ ██ ██ ▄▄ ██ ██    ██ ██     \n");
      espaceCentre(); printf("  ██████  ██   ██ ██   ████  ██████   ██████  ███████\n");

    printf(RESET);
printf("\n");
   espaceCentre(); printf(CYAN "             🏦 GESTION DES CREDITS BANCAIRES\n\n" RESET);
}
static inline void logoAdmin() {

    printf(CYAN);

    espaceCentre(); printf("==========================================================================\n");
    espaceCentre(); printf("                              👨‍💼 CONNEXION ADMIN              \n");
    espaceCentre(); printf("==========================================================================\n");

    printf(RESET);


}
static inline void logoClient() {

    printf(BLEU);

    espaceCentre(); printf("==========================================================================\n");
    espaceCentre(); printf("                           👤 CONNEXION CLIENT              \n");
    espaceCentre(); printf("==========================================================================\n");

    printf(RESET);

    espaceCentre(); printf("                    Consultation et services\n\n");
}

// Menu connexion
static inline int ecranConnexion() {

    int choix;



    printf("\n");
printf(BLANC_GRAS);
        espaceCentre(); printf("         +------------------------------------+\n");

    printf(BLANC_GRAS);
        espaceCentre(); printf("         |   1.👨‍💼 Connexion ADMIN             |\n");

    printf(BLANC_GRAS);
        espaceCentre(); printf("         |   2.👤 Connexion CLIENT            |\n");

    printf(BLANC_GRAS);
        espaceCentre(); printf("         |   0.🚪 Quitter                     |\n");

    printf(BLANC_GRAS);


        espaceCentre(); printf("         +------------------------------------+\n");
printf(RESET);
    printf("\n");

          espaceCentre(); printf("               ➤ Votre choix : ");
    scanf("%d",&choix);

    return choix;
}

#endif
