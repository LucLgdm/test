#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <stdbool.h>
#include <fcntl.h>

#ifndef BUFFER_SIZE
# define BUFFER_SIZE 42
#endif

int	ft_strclen(char *str, char c){
	int i = 0;

	while(str[i] && str[i] != c)
		i++;
	if (str[i] == c)
		i++;
	return i;
}

bool	ft_strchr(char *str, char c){
	int i = -1;

	while(str[++i]){
		if (str[i] == c)
			return true;
	}
	return false;
}

void	ft_clean(char *buffer){
	int i = 0;
	int j = 0;

	while(buffer[i] != '\n')
		i++;
	while(buffer[i]){
		buffer[j] = buffer[i + 1];
		j++;
		i++;
	}
}

char *ft_strcdup(char *str, char c){
	char *duplicated;
	int length = ft_strclen(str, c);
	int i = -1;

	duplicated = malloc((length + 1) * sizeof(char));
	if (!duplicated)
		return NULL;
	while(++i < length)
		duplicated[i] = str[i];
	duplicated[i] = '\0';
	return duplicated;
}

char *pick_up(char *line, char *buffer){
	char *new_line;
	int i = 0;

	if(!line)
		return ft_strcdup(buffer, '\n');
	
	while(buffer[i] != '\n')
		i++;
	new_line = malloc((ft_strclen(line, '\n') + ft_strclen(buffer, '\n') + 1) * sizeof(char));
	if (!new_line)
		return NULL;
	i = -1;
	while(line[++i])
		new_line[i] = line[i];
	while(*buffer && *buffer != '\n'){
		new_line[i] = *buffer;
		buffer++;
		i++;
	}
	if (*buffer == '\n')
		new_line[i++] = '\n';
	new_line[i] = '\0';
	
	free(line);
	return new_line;
}

char *get_next_line(int fd)
{
    static char buffer[BUFFER_SIZE + 1];
    char *new_line = NULL;
	int bytes_read = 1;

    if (BUFFER_SIZE <= 0 || fd < 0 || read(fd, 0, 0) < 0)
        return NULL;
    
	while (bytes_read > 0){
		if(*buffer){
			new_line = pick_up(new_line, buffer);
			if (!new_line)
				return NULL;
		}
		if (ft_strchr(buffer, '\n')){
			ft_clean(buffer);
			break;
		}
		bytes_read = read(fd, buffer, BUFFER_SIZE);
		buffer[bytes_read] = '\0';
	}
    return new_line;
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