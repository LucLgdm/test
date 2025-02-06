#include <stdio.h>
#include <unistd.h>

/*************************************************************
 * We create a child process and we write a different message
 * for the parent and the child
 * Result :
 * Je suis le père, mon PID est 2701
 * Je suis le fils, mon PID est 2702
 * 
 * Each process has its own memory space
 *************************************************************/
int main(){
	pid_t pid = fork();

	if (pid == -1){
		perror("fork");
		return 1;
	}else if (pid == 0){
		printf("Je suis le fils, mon PID est %d\n", getpid());
	}else{
		printf("Je suis le père, mon PID est %d\n", getpid());
	}
}
