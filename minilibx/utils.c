/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lde-merc <lde-merc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/06 16:24:00 by lde-merc          #+#    #+#             */
/*   Updated: 2025/01/07 10:20:41 by lde-merc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "data.h"

void	my_mlx_pixel_put(t_data *data, int x, int y, int color){
	char	*dst;

	dst = data->addr + (y * data->line_length + x * (data->bits_per_pixel / 8));
	*(unsigned int*)dst = color;
}

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


int	close(int keycode, void *mlx, void *window){
	mlx_destroy_window(mlx, window);
	return (0);
}

void draw_circle(t_circle *circle, t_data *img){
	printf("color = 0x%X\n", circle->color);
	for(int x = -circle->radius; x <= circle->radius; x++){
		for(int y = -circle->radius; y <= circle->radius; y++){
			if ((x * x + y * y) <= circle->radius * circle->radius)
				my_mlx_pixel_put(img, circle->center_x + x, circle->center_y + y, circle->color);
		}
	}
}

int		key_handle(int key, void *param){
    t_param *par = (t_param *)param;

    if (key == 65307){
        printf("Closing window with ESC...\n");
        clean_up_and_exit(par);
    }
    
    if (key == 119) {
        clear_image(par);
        par->circle->center_y -= 50;
        par->circle->color = get_opposite(par->circle->color) - 100;
        draw_circle(par->circle, par->data);
    }
    if (key == 115) {
        clear_image(par);
        par->circle->center_y += 50;
        par->circle->color += 1500;
        draw_circle(par->circle, par->data);
    }
    if (key == 97) {
        clear_image(par);
        par->circle->center_x -= 50;
        par->circle->color = get_opposite(par->circle->color) - 100;
        draw_circle(par->circle, par->data);
    }
    if (key == 100) {
        clear_image(par);
        par->circle->center_x += 50;
        par->circle->color += 1500;
        draw_circle(par->circle, par->data);
    }
    
    printf("Key pressed : %d\n", key);
    mlx_put_image_to_window(par->mlx, par->window, par->data->img, 0, 0);
	return (0);
}

void clean_up_and_exit(t_param *par) {
    free(par->circle);
    free(par->data);
    mlx_destroy_window(par->mlx, par->window);
    exit(0);
}

void clear_image(t_param *par) {
    int x, y;

    for (y = 0; y < par->height_w; y++) {
        for (x = 0; x < par->width_w; x++) {
            *((int *)(par->data->addr + (y * par->data->line_length + x * (par->data->bits_per_pixel / 8)))) = 0x00000000;
        }
    }
}
