#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <stdbool.h>

#ifndef BUFFER_SIZE
# define BUFFER_SIZE 42
#endif

char	*get_next_line(int fd);
char	*ft_read_file(int fd, char *buffer);
char	*ft_pick_line(char *buffer);
char	*ft_update(char *buffer);
int		ft_strlen(char *str);
char	*ft_free_and_join(char *str, char *buffer);
char	*ft_strchr(char *str, char c);

int main(void)
{
    char *line;

    printf("Type some text (Ctrl+D to end):\n");

    while ((line = get_next_line(0)) != NULL)
    {
        printf("You typed: %s", line);
        free(line);
    }

    printf("\nEnd of input.\n");

    return 0;
}

char *get_next_line(int fd)
{
    static char *buffer;
    char *line;

    if (fd < 0 || read(fd, NULL, 0) == -1 || BUFFER_SIZE <= 0)
        return NULL;
    
    buffer = ft_read_file(fd, buffer);
    if (!buffer || !*buffer)
        return (free(buffer), buffer = NULL, NULL);

    line = ft_pick_line(buffer);
    buffer = ft_update(buffer);

    return (line);
}

char *ft_read_file(int fd, char *buffer)
{
    char *tmp;
    int bytes_read;

    if (!buffer)
    {
        buffer = malloc(1);
        if (!buffer)
            return NULL;
        buffer[0] = '\0';
    }
    
    tmp = malloc((BUFFER_SIZE + 1) * sizeof(char));
    if (!tmp)
        return (NULL);

    bytes_read = 1;
    while (bytes_read > 0 && !ft_strchr(buffer, '\n'))
    {
        bytes_read = read(fd, tmp, BUFFER_SIZE);
        if (bytes_read == -1)
        {
            free(tmp);
            return (NULL);
        }
        tmp[bytes_read] = '\0';
        buffer = ft_free_and_join(buffer, tmp);
    }

    free(tmp);
    return (buffer);
}

char *ft_pick_line(char *buffer)
{
    int i = 0;
    while (buffer[i] && buffer[i] != '\n')
        i++;
    
    char *newline = malloc((buffer[i] == '\n' ? i + 2 : i + 1) * sizeof(char));
    if (!newline)
        return NULL;

    i = 0;
    while (buffer[i] && buffer[i] != '\n')
    {
        newline[i] = buffer[i];
        i++;
    }
    if (buffer[i] == '\n')
        newline[i++] = '\n';
    newline[i] = '\0';

    return (newline);
}

char *ft_update(char *buffer)
{
    int i = 0;
    while (buffer[i] && buffer[i] != '\n')
        i++;

    if (!buffer[i])
        return (free(buffer), NULL);

    char *output = malloc((ft_strlen(buffer) - i) * sizeof(char));
    if (!output)
        return NULL;

    int j = 0;
    i++;
    while (buffer[i])
        output[j++] = buffer[i++];
    output[j] = '\0';

    free(buffer);
    return (output);
}

int ft_strlen(char *str)
{
    int i = 0;
    while (str[i])
        i++;
    return (i);
}

char *ft_free_and_join(char *buffer, char *str)
{
    int len_buffer = ft_strlen(buffer);
    int len_str = ft_strlen(str);
    char *joined = malloc((len_buffer + len_str + 1) * sizeof(char));
    if (!joined)
        return NULL;

    int i = 0, j = 0;
    while (i < len_buffer)
    {
        joined[i] = buffer[i];
        i++;
    }
    while (j < len_str)
    {
        joined[i + j] = str[j];
        j++;
    }
    joined[i + j] = '\0';

    free(buffer);
    return (joined);
}

char *ft_strchr(char *str, char c)
{
    int i = 0;
    while (str[i])
    {
        if (str[i] == c)
            return (&str[i]);
    i++;
    }
    return NULL;
}
