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
#include <stdio.h>

int		height, width;

void	my_mlx_pixel_put(t_data *data, int x, int y, int color){
	char	*dst;

	dst = data->addr + (y * data->line_length + x * (data->bits_per_pixel / 8));
	*(unsigned int*)dst = color;
}

void	square(t_data *img, int origin_x, int origin_y, int largeur){
	for(int x = 0; x <= largeur; x++){
		for(int y = 0; y <= largeur; y++){
			my_mlx_pixel_put(img, origin_x + x, origin_y + y, 0x00FF0000);
		}
	}
}

void circle(t_data *img, int center_x, int center_y, float r){
	for(int x = -r; x <= r; x++){
		for(int y = -r; y <= r; y++){
			if ((x * x + y * y) <= r * r)
				my_mlx_pixel_put(img, center_x + x, center_y + y, 0x000000FF);
		}
	}
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

	printf("heigth = %i, width = %i\nheigth_w = %i, width_w = %i\n", height, width, height_w, width_w);

	square(&img, width_w/2 + 150, height_w/2 - 100, 200);
	square(&img, width_w/2 - 350, height_w/2 - 100, 200);
	square(&img, width_w/2 - 100, height_w/2 + 150, 200);
	square(&img, width_w/2 - 100, height_w/2 - 350, 200);
	circle(&img, width_w/2, height_w/2, 100);

	mlx_put_image_to_window(mlx, window, img.img, 0, 0);
    mlx_loop(mlx);	
	return (0);
}
