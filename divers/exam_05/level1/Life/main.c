
#include "life.h"

static int width;
static int height;
static int generations;

int main(int argc, char *argv[]) {
	if (argc != 4) {
		print_usage(argv[0]);
		return 1;
	}
	width = atoi(argv[1]);
	height = atoi(argv[2]);
	generations = atoi(argv[3]);
	initialize_game(argc, argv);
	print_grid();
	play();
	free_grid();
	
	return 0;
}

int ***get_grid() {
	static int **grid = NULL;
	return &grid;
}

void initialize_game(int argc, char *argv[]) {
	int ***gridp = get_grid();
	*gridp = (int **)malloc(height * sizeof(int *));
	if (!(*gridp)) return;
	for(int i = 0; i < height; i++) {
		(*gridp)[i] = (int *)calloc(width, sizeof(int));
		if (!(*gridp)[i]) return;
	}

	int **grid = *gridp;
	int x = 0, y = 0, pen = 1;
	char buffer[1024];
	int bytes_read;
	while ((bytes_read = read(0, buffer, sizeof(buffer))) > 0) {
		for(int i = 0; i < bytes_read; i++) {
			if (buffer[i] == 'x')
				pen *= -1;
			else if (buffer[i] == 'w')
				y--;
			else if (buffer[i] == 'a')
				x--;
			else if (buffer[i] == 's')
				y++;
			else if (buffer[i] == 'd')
				x++;
			
			if (x >= 0 && x < width && y >= 0 && y < height && pen < 0) 
				grid[y][x] = 1;
		}
	}
}

void play() {
	for(int i = 0; i < generations; i++) {
		int ***gridp = get_grid();
		int **grid = *gridp;
		int **new_grid = (int **)malloc(height * sizeof(int *));
		for(int j = 0; j < height; j++) {
			new_grid[j] = (int *)malloc(width * sizeof(int));
			for(int k = 0; k < width; k++) {
				new_grid[j][k] = grid[j][k];
			}
		}

		for(int y = 0; y < height; y++) {
			for(int x = 0; x < width; x++) {
				int neighbors = 0;
				for(int dy = -1; dy <= 1; dy++) {
					for(int dx = -1; dx <= 1; dx++) {
						if (dy == 0 && dx == 0) continue;
						int ny = y + dy;
						int nx = x + dx;
						if (ny >= 0 && ny < height && nx >= 0 && nx < width) {
							neighbors += grid[ny][nx];
						}
					}
				}

				if (grid[y][x] == 1) {
					new_grid[y][x] = (neighbors == 2 || neighbors == 3);
				} else {
					new_grid[y][x] = (neighbors == 3);
				}
			}
		}

		for(int k = 0; k < height; k++) {
			for(int j = 0; j < width; j++) {
				grid[k][j] = new_grid[k][j];
			}
			free(new_grid[k]);
		}
		free(new_grid);

		print_grid();
		putchar('\n');
	}
}

void print_grid() {
	int pen = 1;
	int **grid = *(get_grid());

	for(int i = 0; i < height; i++) {
		int x = 0;
		for(int j = 0; j < width; j++) {
			if (grid[i][j] == 1)
				x = j;
			}
		if (x) {
			for(int j = 0; j <= x; j++) {
				putchar(grid[i][j] == 1 ? 'O' : ' ');
			}
			if (grid[i][x]) putchar(' ');
		}
		putchar('\n');
	}
}

void free_grid() {
	int **grid = *(get_grid());
	
	for(int i = 0; i < height; i++) {
		free(grid[i]);
	}
	free(grid);
	*(get_grid()) = NULL;
}

void print_usage(const char *program_name) {
	char *usage = "Usage: ";
	int c = ft_strlen(usage);
	for(int i = 0; i < c; i++) {
		putchar(usage[i]);
	}
	for(int i = 0; program_name[i] != '\0'; i++) {
		putchar(program_name[i]);
	}
	char *args = " <width> <height> <generations>\n";
	c = ft_strlen(args);
	for(int i = 0; i < c; i++) {
		putchar(args[i]);
	}
}

int ft_strlen(char *str) {
	int len = 0;
	while (str[len] != '\0') {
		len++;
	}
	return len;
}
