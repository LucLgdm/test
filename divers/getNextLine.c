#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <stdbool.h>
#include <fcntl.h>

#ifndef BUFFER_SIZE
# define BUFFER_SIZE 42
#endif


bool ft_strchr(char *str, char c){
	int i = -1;
	while(str[++i]){
		if (str[i] == c)
			return true;
	}
	return false;
}


void ft_clean(char *buffer){
	int i = 0, j = 0;

	while(buffer[i] != '\n')
		i++;
	while(buffer[i])
	{
		buffer[j] = buffer[i + 1];
		i++;
		j++;
	}
}

int ft_strclen(char *str, char c){
	int i = 0;

	while(str[i] && str[i] != c)
		i++;
	if(str[i] == c)
		i++;
	return i;
}

char *ft_strcdup(char *str, char c){
	int length = ft_strclen(str, c);
	char *duplicated = malloc((length + 1) * sizeof(char));
	if (!duplicated)
		return NULL;
	for(int i = 0; i < length; i++)
		duplicated[i] = str[i];
	duplicated[length] = 0;
	return duplicated;
}

char *ft_strcjoin(char * line, char *buffer, char c){
	if(!line)
		return(ft_strcdup(buffer, c));
	
	char *new_line = malloc((ft_strclen(line, c) + ft_strclen(buffer, c) + 1) * sizeof(char));
	if (!new_line)
		return NULL;
	int i = -1;

	while(line[++i])
		new_line[i] = line[i];
	while(*buffer && *buffer != c){
		new_line[i] = *buffer;
		i++;
		buffer++;
	}
	if(*buffer == c)
		new_line[i++] = c;
	new_line[i] = 0;
	
	free(line);
	return(new_line);
}

char *get_next_line(int fd){
	static char buffer[BUFFER_SIZE + 1];
	char *line = NULL;
	int	bytes_read = 1;

	if(fd < 0 || BUFFER_SIZE <= 0 || read(fd, 0, 0) < 0)
		return NULL;
	
	while(bytes_read > 0){
		if (*buffer){
			line = ft_strcjoin(line, buffer, '\n');
			if (!line)
				return NULL;
		}
		if (ft_strchr(buffer, '\n')){
			ft_clean(buffer);
			break;
		}
		bytes_read = read(fd, buffer, BUFFER_SIZE);
		buffer[bytes_read] = 0;
	}
	return line;
}



int main(int argc, char **argv)
{
    if (argc != 2)
    {
        printf("Usage: %s <file>\n", argv[0]);
        return 1;
    }

    int fd = open(argv[1], O_RDONLY);
    if (fd == -1)
    {
        perror("open");
        return 1;
    }

    char *line;
    while ((line = get_next_line(fd)) != NULL)
    {
        printf("%s", line);
        free(line);
    }

    close(fd);
    return 0;
}