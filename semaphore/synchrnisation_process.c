#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

/*************************************************************
 * We create a child process and we write a different message
 * for the parent and the child. The parent do its thing
 * when the child is over
 *************************************************************/
/*
int main(){
	pid_t pid = fork();

	if (pid == -1){
		perror("fork");
		return 1;
	}else if (pid == 0){
		printf("Je suis le fils, mon PID est %d\nJe dors 2 secondes...\n", getpid());
		sleep(2);
		printf("Enfant terminé\n");
	}else{
		printf("Le parent attend l'enfant...\n");
		waitpid(pid, NULL, 0);
		printf("L'enfant a fini, le parent continue !\n");
		sleep(1);
		printf("Je suis le père, mon PID est %d\n", getpid());
		for(int i = 0; i < 10; i++){
			printf("%i\n", i + 1);
		}
	}
}
*/
/******************************************************************
 * Here we create 2 children from one parent. The parent waits for
 * the children to finish before doing its thing
 *******************************************************************/
int main(){
	pid_t child1, child2;

	printf("Le parent attend les enfants...\n");
	child1 = fork();
	if (child1 == -1){
		perror("fork");
		return 1;
	}else if (child1 == 0){
		printf("Je suis le fils 1, mon PID est %d\nJe dors 2 secondes...\n", getpid());
		sleep(2);
		printf("Enfant 1 terminé\n");
		return 0;
	}
	child2 = fork();
	if (child2 == -1){
		perror("fork");
		return 1;
	}else if (child2 == 0){
		printf("Je suis le fils 2, mon PID est %d\nJe dors 2 secondes...\n", getpid());
		sleep(2);
		printf("Enfant 2 terminé\n");
		return 0;
	}

	waitpid(child1, NULL, 0);
	waitpid(child2, NULL, 0);
	printf("Les enfants ont fini, le parent continue !\n");
	for(int i = 1; i <= 10; i++) printf("%i\n", i);
	return 0;
}
