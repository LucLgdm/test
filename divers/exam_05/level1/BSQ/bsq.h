

#ifndef BSQ_H
#define BSQ_H

# include <fcntl.h>
# include <stdbool.h>
# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>

typedef struct {
	int height;
	int width;
	char **map;
	char empty;
	char obstacle;
	char full;
} t_map;

bool parse(int i, char *file);
void print_map();
void free_map();
int ft_strlen(const char *str);
bool check_map();
void solve();

#endif