#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <terminfo.h>
#include <unistd.h>
#include <string.h>

int main(void) {
    char buffer[1024];
    char *term = getenv("TERM");
    
    // Charger la description du terminal
    if (tgetent(buffer, term) <= 0) {
        perror("Erreur de tgetent");
        exit(1);
    }

    // Vérifier si le terminal supporte le mode couleur
    if (tgetflag("bw")) {
        printf("Le terminal est en noir et blanc.\n");
    }

    // Obtenir le nombre de colonnes
    int cols = tgetnum("co");
    printf("Nombre de colonnes : %d\n", cols);

    // Obtenir la séquence pour déplacer le curseur
    char *move_cursor = tgetstr("cm", &buffer);
    if (move_cursor != NULL) {
        char *move = tgoto(move_cursor, 10, 5);  // Déplacer à (10, 5)
        tputs(move, 1, putchar);  // Déplacer le curseur
        printf("Le curseur a été déplacé.\n");
    }

    return 0;
}
