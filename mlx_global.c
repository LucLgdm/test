/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mlx_global.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lde-merc <lde-merc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/06 15:59:19 by lde-merc          #+#    #+#             */
/*   Updated: 2025/01/06 17:09:07 by lde-merc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "data.h"

int height_screen, width_screen;


int main(){
    t_param     param;
    t_data      *data = malloc(sizeof(t_data));
    t_circle    *circle = malloc(sizeof(t_circle));


    param.circle = circle;
    param.data = data;
    // Initialisation
    param.mlx = mlx_init();
	mlx_get_screen_size(param.mlx, &width_screen, &height_screen);
    param.width_w = width_screen / 2;
    param.height_w = height_screen / 2;
    
    // Creation of the window
    param.window = mlx_new_window(param.mlx, param.width_w, param.height_w, "so_long");
    
    // Creation of the image
    param.data->img = mlx_new_image(param.mlx, param.width_w, param.height_w);
    param.data->addr = mlx_get_data_addr(param.data->img, &param.data->bits_per_pixel, &param.data->line_length,
			&param.data->endian);

    int color = create_color(255, 0, 0, 0);
	// Background
	for(int x = 0; x <= param.width_w; x++){
		for(int y = 0; y <= param.height_w; y++){
			my_mlx_pixel_put(param.data, x, y, color);
		}
	}

    param.circle->center_x = param.width_w / 2;
    param.circle->center_y = param.height_w / 2;
    param.circle->radius = 50;
    param.circle->color = create_color(255, 150, 231, 86);

    draw_circle(param.circle, param.data);
    mlx_put_image_to_window(param.mlx, param.window, param.data->img, 0, 0);
    
    mlx_key_hook(param.window, key_handle, &param);
    
    mlx_loop(param.mlx);
    return 0;
}
