#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>


void print_tab(char **tab) {
    for (int i = 0; tab[i]; i++) {
        printf("%s ", tab[i]);
    }
    printf("\n");
}


void execute_command(char **cmd, char **env){
	if (!cmd || !cmd[0])
        return;
	
	pid_t pid = fork();
	if (pid == -1){
        perror("error: fatal");
        exit(1);
    }

	if (pid == 0){
		execve(cmd[0], cmd, env);
		fprintf(stderr, "error: cannot execute %s\n", cmd[0]);
        exit(1);
	} else {
		waitpid(pid, NULL, 0);
	}
}

char **extract_command(char **argv, int *i) {
    int start = *i;
    int count = 0;

    while (argv[*i] && strncmp(argv[*i], ";", 1) != 0 && strncmp(argv[*i], "|", 1) != 0) {
        (*i)++;
        count++;
    }

    char **cmd = malloc((count + 1) * sizeof(char *));
    if (!cmd)
        return NULL;

    for (int j = 0; j < count; j++) {
        cmd[j] = strdup(argv[start + j]);
        if (!cmd[j]) {
            while (j > 0)
                free(cmd[--j]);
            free(cmd);
            return NULL;
        }
    }
    cmd[count] = NULL;

    if (argv[*i] && (strncmp(argv[*i], ";", 1) == 0 || strncmp(argv[*i], "|", 1) == 0)) {
        (*i)++;
    }

    return cmd;
}

void free_tab(char **tab) {
    for (int i = 0; tab[i]; i++)
        free(tab[i]);
    free(tab);
}

int main(int argc, char **argv, char **env) {
    char **cmd;
    int i = 1;

    while (i < argc) {
        cmd = extract_command(argv, &i);
        if (cmd && *cmd) {
            execute_command(cmd, env);
			free_tab(cmd);
        }
    }
    return 0;
}
