#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(void)
{
    struct stat fileStat;
    char *file = "test.txt";
    
	int fd = open(file, O_WRONLY | O_CREAT | O_TRUNC, 0644); // On cree le fichier test.txt
	write(fd, "Hello, world!\n", 14);
	write(fd, "This is a test.\n", 15);
	close(fd);

    if (access(file, F_OK) == 0)
        printf("Le fichier %s existe.\n", file);
    else
        printf("Le fichier %s n'existe pas.\n", file);	
    
    if (access(file, X_OK) == 0)
        printf("Le fichier %s est exécutable.\n", file);
    else
        printf("Le fichier %s n'est pas exécutable.\n", file);

    
    if (stat("test.txt", &fileStat) == 0) // On affiche la taille du fichier en octet
        printf("Taille du fichier : %ld octets\n", fileStat.st_size);
    else
        perror("Erreur lors de stat");
    
	if (unlink("test.txt") == 0)
        printf("Fichier supprimé avec succès.\n");
    else
        perror("Erreur lors de la suppression");
    return 0;
}
