#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>

void	ft_putstr_fd(const char *s, int fd) {
	if (s) {
		write(fd, s, strlen(s));
	}
}

void	free_tab(char **tab) {
	for (int i = 0; tab[i]; i++)
		free(tab[i]);
	free(tab);
}

void ft_cd(char **cmd) {
	if (cmd[1] == NULL) {
		ft_putstr_fd("Error: cd: bad arguments\n", 2);
		return;
	}
	if (chdir(cmd[1]) == -1) {
		perror("Error: cd");
	}
}

void ft_execute_pipe(char **cmd1, char **cmd2, char **env) {
	int pipe_fd[2];
	pid_t pid1, pid2;

	if(pipe(pipe_fd) == -1){
		ft_putstr_fd("Error: fatal\n", 2);
		exit(1);
	}
	pid1 = fork();
	if(pid1 == -1){
		ft_putstr_fd("Error: fatal\n", 2);
		exit(1);
	}
	if(pid1 == 0){
		dup2(pipe_fd[1], STDOUT_FILENO);
		close(pipe_fd[0]);
		close(pipe_fd[1]);
		
		if(strcmp(cmd1[0], "cd") == 0){
			ft_cd(cmd1);
			exit(0);
		}
		if(execve(cmd1[0], cmd1, env) == -1){
			ft_putstr_fd("Error: cannot execute ", 2);
			ft_putstr_fd(cmd1[0], 2);
			ft_putstr_fd("\n", 2);
			exit(1);
		}
	}

	pid2 = fork();
	if (pid2 == -1){
		ft_putstr_fd("Error: fatal\n", 2);
		exit(1);
	}
	if (pid2 == 0){
		dup2(pipe_fd[0], STDIN_FILENO);
		close(pipe_fd[0]);
		close(pipe_fd[1]);

		if(strcmp(cmd2[0], "cd") == 0){
			ft_cd(cmd2);
			exit(0);
		}
		if(execve(cmd2[0], cmd2, env) == -1){
			ft_putstr_fd("Error: cannot execute ", 2);
			ft_putstr_fd(cmd2[0], 2);
			ft_putstr_fd("\n", 2);
			exit(1);
		}
	}
	close(pipe_fd[0]);
	close(pipe_fd[1]);

	waitpid(pid1, NULL, 0);
	waitpid(pid2, NULL, 0);
	kill(pid1, SIGKILL);
	kill(pid2, SIGKILL);
}

void ft_execute_command(char **cmd, char **env){
	if (!cmd || !cmd[0])
		return;

	if (strcmp(cmd[0], "cd") == 0) {
		ft_cd(cmd);
		return;
	}

	pid_t pid = fork();
	if (pid == -1) {
		ft_putstr_fd("Error: fatal\n", 2);
		exit(1);
	}

	if (pid == 0) {
		if (execve(cmd[0], cmd, env) == -1) {
			ft_putstr_fd("Error: cannot execute ", 2);
			ft_putstr_fd(cmd[0], 2);
			ft_putstr_fd("\n", 2);
			exit(1);
		}
	}

	waitpid(pid, NULL, 0);
	kill(pid, SIGKILL);
}



char **ft_extract_command(char **argv, int *i){
	int start = *i;
	int count = 0;

	while(argv[*i] && strcmp(argv[*i], "|") != 0 && strcmp(argv[*i], ";") != 0){
		count++;
		(*i)++;
	}
	char **cmd = malloc((count + 1) * sizeof(char *));
	if (!cmd){
		ft_putstr_fd("Error: fatal\n", 2);
		exit(1);
	}
	for(int j = 0; j < count; j++){
		cmd[j] = strdup(argv[start + j]);
		if(!cmd[j]){
			while(j > 0)
				free(cmd[--j]);
			free(cmd);
			ft_putstr_fd("Error: fatal\n", 2);
			exit(1);
		}
	}
	cmd[count] = NULL;
	if(argv[*i] && strcmp(argv[*i], ";") == 0){
		(*i)++;
	}
	return cmd;
}

int main(int argc, char **argv, char **envp) {
	char **cmd;
	int i = 1;

	while(i < argc){
		cmd = ft_extract_command(argv, &i);
		if (cmd && *cmd){
			if (argv[i] && strcmp(argv[i], "|") == 0){
				i++;
				char **next_cmd = ft_extract_command(argv, &i);
				if  (next_cmd && *next_cmd){
					ft_execute_pipe(cmd, next_cmd, envp);
				}
				free_tab(next_cmd);
			}
			else {
				ft_execute_command(cmd, envp);
			}
			free_tab(cmd);
		}
	}
	return 0;
}
