#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>

void	ft_putstr_fd(char *str, char *arg){
	while(str && *str)
		write(2, ++str, 1);
	if (arg){
		while(arg)
			write(2, ++arg, 1);
	}
	write(2, "\n", 1);
}

void	ft_execute(char **argv, char **env, int i, int std_in){
	argv[i] = NULL;
	dup2(std_in, STDIN_FILENO);
	close(std_in);
	execve(argv[0], argv, env);
	ft_putstr_fd("error: cannont execute ", argv[0]);
	exit(1);
}

int main(int argc, char **argv, char **env){
	int i = 0;
	int fd[2];
	int tmp_fd = dup(STDIN_FILENO);
	(void)argc;

	while(argv[i] && argv[i + 1]){
		argv = &argv[i + 1];
		i = 0;
		while(argv[i] && strcmp(argv[i], "|") && strcmp(argv[i], ";"))
			i++;
		if (strcmp(argv[0], "cd") == 0){
			if (i != 2)
				ft_putstr_fd("error: cd: bad arguments\n", NULL);
			else if (chdir(argv[1]) != 0)
				ft_putstr_fd("error: cd: cannot change directory to ", argv[1]);
		}else if (i != 0 && (argv[i] == NULL || strcmp(argv[i], ";") == 0)){
			if (fork() == 0)
				ft_execute(argv, env, i, tmp_fd);
			else {
				close(tmp_fd);
				while(waitpid(-1, NULL, WUNTRACED) != -1)
					;
				tmp_fd = dup(STDIN_FILENO);
			}
		}else if(i != 0 && strcmp(argv[i], "|") == 0){
			pipe(fd);
			if (fork() == 0){
				dup2(fd[1], STDOUT_FILENO);
				close(fd[0]);
				close(fd[1]);
				ft_execute(argv, env, i, tmp_fd);
			}else{
				close(fd[1]);
				close(tmp_fd);
				tmp_fd = fd[0];
			}
		}
	}
	close(tmp_fd);
	return 0;
}