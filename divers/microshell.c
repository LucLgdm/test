#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>
#include <signal.h>


void print_tab(char **tab) {
    for (int i = 0; tab[i]; i++) {
        printf("%s ", tab[i]);
    }
    printf("\n");
}

void execute_pipe(char **cmd, char **next_cmd, char **env){
    int pipe_fd[2];
    pid_t pid_1, pid_2;

    if (pipe(pipe_fd) == -1) {
        perror("error: fatal");
        exit(1);
    }
    
    pid_1 = fork();
    if (pid_1 == -1) {
        perror("error: fatal");
        exit(1);
    }

    if (pid_1 == 0) {  // Enfant pour cmd1
        dup2(pipe_fd[1], STDOUT_FILENO);
        close(pipe_fd[0]);
        close(pipe_fd[1]);

        if (execve(cmd[0], cmd, env) == -1){
            fprintf(stderr, "error: cannot execute %s\n", cmd[0]);
            exit(1);
        }
    }

    pid_2 = fork();
    if (pid_2 == -1) {
        perror("error: fatal");
        exit(1);
    }

    if (pid_2 == 0) {  // Enfant pour next_cmd
        dup2(pipe_fd[0], STDIN_FILENO);
        close(pipe_fd[0]);
        close(pipe_fd[1]);

        execve(next_cmd[0], next_cmd, env);
        fprintf(stderr, "error: cannot execute %s\n", next_cmd[0]);
        exit(1);
    }
	close(pipe_fd[0]);
	close(pipe_fd[1]);
    
    waitpid(pid_1, NULL, 0);
    waitpid(pid_2, NULL, 0);
    
    kill(pid_1, SIGKILL);
    kill(pid_2, SIGKILL);
}

void execute_command(char **cmd, char **env){
	if (!cmd || !cmd[0])
        return;

	if (strcmp(cmd[0], "cd") == 0) {
		ft_cd(cmd);
		return;
	}
	// Fork and execute the command
	pid_t pid_1 = fork();
	if (pid_1 == -1){
        perror("error: fatal");
        exit(1);
    }

	if (pid_1 == 0){
        execve(cmd[0], cmd, env);
		fprintf(stderr, "error: cannot execute %s\n", cmd[0]);
        exit(1);
	}

    waitpid(pid_1, NULL, 0);
    kill(pid_1, SIGKILL);
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

    if (argv[*i] && (strncmp(argv[*i], ";", 1) == 0 )) {
        (*i)++;
    }

    return cmd;
}

void free_tab(char **tab) {
    for (int i = 0; tab[i]; i++)
        free(tab[i]);
    free(tab);
}


void ft_cd(char **cmd){
	if (cmd[1] == NULL){
		fprintf(stderr, "error: cd: bad arguments\n");
		return;
	}
	if (chdir(cmd[1]) == -1){
		perror("error: cd");
	}
}

int main(int argc, char **argv, char **env) {
    char **cmd;
    int i = 1;

    while (i < argc) {
        cmd = extract_command(argv, &i);
        if (cmd && *cmd) {
            if (argv[i] && strcmp(argv[i], "|") == 0) {
                i++;
                char **next_cmd = extract_command(argv, &i);
                if (next_cmd && *next_cmd) {
                    execute_pipe(cmd, next_cmd, env);
                }
                free_tab(next_cmd);
            } else {
                execute_command(cmd, env);
            }
			free_tab(cmd);
        }
    }
    return 0;
}
