#include <stdio.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <signal.h>

void handle_signal(int sig)
{
    if (sig == SIGINT)
    {
        printf("\n"); // Nouvelle ligne propre
        rl_on_new_line();  // Indique à readline qu'on est sur une nouvelle ligne
        rl_replace_line("", 0); // Efface la ligne en cours
        rl_redisplay(); // Réaffiche le prompt
    }
}

int main(void)
{
    char *input;

    signal(SIGINT, handle_signal); // Intercepte Ctrl+C

    while (1)
    {
        input = readline("minishell > ");
        if (!input)
            break; // Il n'y a plus de commandes : Ctrl+D
        if (*input)
            add_history(input); // Garde en memoire les commandes, fleches haut et bas
        if (strcmp(input, "clean") == 0)
		{
			rl_clear_history(); // Efface l'historique
			printf("\033[0;32mHistorique effacé\033[0m\n"); // Ecrit en vert
		}
		free(input);
    }
    return 0;
}
