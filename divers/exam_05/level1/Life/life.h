
#ifndef LIFE_H
#define LIFE_H

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>


void print_usage(const char *program_name);
int ft_strlen(char *str);
void initialize_game(int argc, char *argv[]);
int ***get_grid();
void print_grid();
void play();
void free_grid();

#endif
