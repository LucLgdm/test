#include "bsq.h"

t_map *get_map() {
	static t_map map;
	return &map;
}

int main(int argc, char **argv) {
	t_map *map = get_map();

	if (argc == 1) {
		if (parse(1, NULL)) {
			solve();
			print_map();
		}
		free_map();
		return 0;
	}

	for(int i = 1; i < argc; ++i) {
		if (parse(2, argv[i])) {
			solve();
			print_map();
		}
		free_map();
		if (i < argc - 1)
			write(1, "\n", 1);
	}
	return 0;
}

bool parse(int argc, char *file) {
	t_map *map = get_map();
	map->map = NULL;
	size_t len = 0;
	int width = 0;
	int line_num = 0;

	FILE *f;
	if (argc == 1) {
		f = stdin;
	} else {
		f = fopen(file, "r");
	}

	if (!f) {
		write(2, "Error: Cannot open file\n", 24);
		return false;
	}

	if (fscanf(f, "%d %c %c %c\n", &map->height, &map->empty, &map->obstacle, &map->full) != 4) {
		write(2, "Error: Invalid map header\n", 26);
		fclose(f);
		return false;
	}
	if (map->empty == map->obstacle || map->empty == map->full || map->obstacle == map->full) {
		write(2, "Error: Invalid header characters\n", 30);
		fclose(f);
		return false;
	}

	if (map->empty < 32 || map->empty > 126 ||
		map->obstacle < 32 || map->obstacle > 126 ||
		map->full < 32 || map->full > 126) {
		write(2, "Error: Invalid header characters\n", 30);
		fclose(f);
		return false;
	}

	line_num = 1;
	map->map = (char **)malloc(sizeof(char *) * (map->height + 1));
	if (!map->map) {
		fprintf(stderr, "Error: Memory allocation failed\n");
		return false;
	}
	for(int i = 0; i < map->height; i++)
		map->map[i] = NULL;

	char *buffer = NULL;
	int bytes_read;
	while((bytes_read = getline(&buffer, &len, f)) != -1) {

		if (buffer[bytes_read - 1] == '\n') {
			buffer[--bytes_read] = '\0';
		} else {
			write(2, "Error: no line break at the end of line\n", 40);
			free(buffer); fclose(f);
			return false;
		}
		if (line_num == 1) {
			map->width = ft_strlen(buffer);
		}

		if (ft_strlen(buffer) != map->width) {
			write(2, "Error: Inconsistent line widths\n", 32);
			free(buffer); fclose(f);
			return false;
		}

		map->map[line_num - 1] = (char *)malloc(sizeof(char) * (map->width + 1));
		if (!map->map[line_num - 1]) {
			write(2, "Error: Memory allocation failed\n", 32);
			free(buffer); fclose(f);
			return false;
		}

		for(int i = 0; i < map->width; i++)
			map->map[line_num - 1][i] = buffer[i];
		map->map[line_num - 1][map->width] = '\0';
		line_num++;
	}
	free(buffer);
	fclose(f);
	map->map[map->height] = NULL;
	return check_map();
}

void print_map() {
	t_map *map = get_map();

	// printf("Map (%dx%d):\n", map->height, map->width);
	// printf("Empty: '%c', Obstacle: '%c', Full: '%c'\n", map->empty, map->obstacle, map->full);
	int i = -1;
	while(map->map[++i]) {
		write(1, map->map[i], map->width);
		write(1, "\n", 1);
	}
}

void free_map() {
	t_map *map = get_map();

	if (!map->map)
		return;
	int i = -1;
	while(map->map[++i]) {
		free(map->map[i]);
	}
	free(map->map);
	map->map = NULL;
	map->height = 0;
	map->width = 0;
	map->empty = 0;
	map->obstacle = 0;
	map->full = 0;
}

int ft_strlen(const char *str) {
	int len = 0;
	while(str[len]) len++;
	return len;
}

bool check_map() {
	t_map *map = get_map();


	if (map->height <= 0 || map->width <= 0) {
		write(2, "Error: Invalid map dimensions\n", 30);
		return false;
	}

	for(int i = 0; i < map->height; i++) {
		printf("segfault ici ? %d\n", i);
		for(int j = 0; j < map->width; j++) {
			if (map->map[i][j] != map->empty && map->map[i][j] != map->obstacle) {
				write(2, "Error: Invalid character in map\n", 32);
				return false;
			}
		}
	}
	printf("segfault ici ?\n");

	return true;
}

void solve() {
	t_map *map = get_map();
	for(int i = 0; i < map->height; i++) {
		for(int j = 0; j < map->width; j++) {
			if (i == 0) {
				map->map[i][j] = map->map[i][j] == map->empty ? '1' : '0';
			} else if (j == 0) {
				map->map[i][j] = map->map[i][j] == map->empty ? '1' : '0';
			} else {
				if (map->map[i][j] == map->obstacle) {
					map->map[i][j] = '0';
				} else if (map->map[i][j] == map->empty) {
					int top = map->map[i - 1][j] - '0';
					int left = map->map[i][j - 1] - '0';
					int diag = map->map[i - 1][j - 1] - '0';
					int min = top < left ? top : left;
					min = min < diag ? min : diag;
					map->map[i][j] = (min + 1) + '0';
				}
			}
		}
	}
	int max_size = 0;
	int max_i = 0;
	int max_j = 0;
	for(int i = 0; i < map->height; i++) {
		for(int j = 0; j < map->width; j++) {
			if (map->map[i][j] - '0' > max_size) {
				max_size = map->map[i][j] - '0';
				max_i = i;
				max_j = j;
			}
		}
	}
	for(int i = max_i; i > max_i - max_size; i--) {
		for(int j = max_j; j > max_j - max_size; j--) {
			map->map[i][j] = map->full;
		}
	}

	for(int i = 0; i < map->height; i++) {
		for(int j = 0; j < map->width; j++) {
			if (map->map[i][j] != map->full) {
				if (map->map[i][j] == '0')
					map->map[i][j] = map->obstacle;
				else
					map->map[i][j] = map->empty;
			}
		}
	}
}

