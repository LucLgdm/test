// /* ************************************************************************** */
// /*                                                                            */
// /*                                                        :::      ::::::::   */
// /*   mlx.c                                              :+:      :+:    :+:   */
// /*                                                    +:+ +:+         +:+     */
// /*   By: lde-merc <lde-merc@student.42.fr>          +#+  +:+       +#+        */
// /*                                                +#+#+#+#+#+   +#+           */
// /*   Created: 2024/12/17 06:38:34 by lde-merc          #+#    #+#             */
// /*   Updated: 2025/01/06 11:33:05 by lde-merc         ###   ########.fr       */
// /*                                                                            */
// /* ************************************************************************** */

#include "data.h"

int		height, width;

int	create_color(int t, int r, int g, int b){
	return (t << 24) | (r << 16) | (g << 8) | b;
}

int get_transparency(int color) {
	return (color >> 24) & 0xFF;
}

int get_red(int color) {
    return (color >> 16) & 0xFF;
}

int get_green(int color) {
    return (color >> 8) & 0xFF;
}

int get_blue(int color) {
    return color & 0xFF;
}

int add_shade(double distance, int color){
	int t = get_transparency(color);
	int r = get_red(color) * (1 - distance);
	int g = get_green(color) * (1 - distance);
	int b = get_blue(color) * (1 - distance);
	return create_color(t, r, g, b);
}

int	get_opposite (int color){
	int t = get_transparency(color);
	int r = 255 - get_red(color);
	int g = 255 - get_green(color);
	int b = 255 - get_blue(color);
	return create_color(t, r, g, b);
}

void	my_mlx_pixel_put(t_data *data, int x, int y, int color){
	char	*dst;

	dst = data->addr + (y * data->line_length + x * (data->bits_per_pixel / 8));
	*(unsigned int*)dst = color;
}

void	square(t_data *img, int origin_x, int origin_y, int largeur, int color){
	printf("color = 0x%X\n", color);
	for(int x = 0; x <= largeur; x++){
		for(int y = 0; y <= largeur; y++){
			my_mlx_pixel_put(img, origin_x + x, origin_y + y, color);
		}
	}
}

void circle(t_data *img, int center_x, int center_y, float r, int color){
	printf("color = 0x%X\n", color);
	for(int x = -r; x <= r; x++){
		for(int y = -r; y <= r; y++){
			if ((x * x + y * y) <= r * r)
				my_mlx_pixel_put(img, center_x + x, center_y + y, color);
		}
	}
}

int	close(int keycode, void *mlx, void *window)
{
	mlx_destroy_window(mlx, window);
	return (0);
}

int	main(void){
	void    *mlx;
	void	*window;
    t_data 	img;
	int		height_w;
	int		width_w;
	
    mlx = mlx_init();
	mlx_get_screen_size(mlx, &width, &height);
	height_w = height/2;
	width_w = width/2;
	window = mlx_new_window(mlx, width_w, height_w, "so_long");
    img.img = mlx_new_image(mlx, width_w, height_w);
	img.addr = mlx_get_data_addr(img.img, &img.bits_per_pixel, &img.line_length,
			&img.endian);

	int color = create_color(255, 200, 200, 200);
	// Background
	for(int x = 0; x <= width_w; x++){
		for(int y = 0; y <= height_w; y++){
			my_mlx_pixel_put(&img, x, y, color);
		}
	}

	// color = create_color(255, 50, 150, 125);
	// square(&img, width_w/2 + 150, height_w/2 - 100, 200, color);
	// int color2 = add_shade(0.5, color);
	// square(&img, width_w/2 - 350, height_w/2 - 100, 200, color2);
	// color2 = add_shade(0.75, color);
	// square(&img, width_w/2 - 100, height_w/2 + 150, 200, color2);
	// color2 = get_opposite(color);
	// square(&img, width_w/2 - 100, height_w/2 - 350, 200, color2);
	// color2 = add_shade(0.25, color);
	// circle(&img, width_w/2, height_w/2, 100, color2);

	mlx_put_image_to_window(mlx, window, img.img, 0, 0);

	mlx_hook(window, 2, 1L<<0, close, &img);
    mlx_loop(mlx);

	return (0);
}
