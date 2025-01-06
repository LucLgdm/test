/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mlx_hook.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lde-merc <lde-merc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/06 13:41:26 by lde-merc          #+#    #+#             */
/*   Updated: 2025/01/06 16:44:21 by lde-merc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "data.h"
#include <stdio.h>
#include <stdlib.h>


int close_window(void) {
    printf("Closing window...\n");
    exit(0);
}

int	key_handle(int keycode, void *param){
    t_param *data = (t_param *)param;

    if (keycode == 65307)
        close_window();
    
    if (keycode == 65362) { // Flèche haut pour augmenter la taille
        mlx_destroy_window(data->mlx, data->window);
        data->width += 100;
        data->height += 100;
        data->window = mlx_new_window(data->mlx, data->width, data->height, "Window Resized");
    }
    
    if (keycode == 65364) { // Flèche bas pour diminuer la taille
        mlx_destroy_window(data->mlx, data->window);
        data->width -= 100;
        data->height -= 100;
        data->window = mlx_new_window(data->mlx, data->width, data->height, "Window Resized");
    }
    
    printf("Key pressed : %d\n", keycode);

	return (0);
}

int mouse_enter(){
    printf("Hello !\n");
}

int mouse_exit(){
    printf("Bye !\n");
}

int	main(void){
	t_param param;
    
    param.width = 800;
    param.height = 600;
    
    param.mlx = mlx_init();
	param.window = mlx_new_window(param.mlx, param.width, param.height, "so_long");

    // mlx_hook(window, event, mask, handler_function, param);
     // handle keyboard
	mlx_key_hook(param.window, key_handle, &param);
    // handle close by cross
    mlx_hook(param.window, 17, 1L<<19, close_window, NULL);
    // handle mouse
    mlx_hook(param.window, 7, 1L<<4, mouse_enter, NULL);
    mlx_hook(param.window, 8, 1L<<5, mouse_exit, NULL);
    
    mlx_loop(param.mlx);

	return (0);
}

/****************************************************************************
 * Lorsqu'une window est ouverte, les hook sont associes a cette window.
 * Lorsqu'on la ferme, il faut relancer des hooks pour la nouvelle fenetre.
 ****************************************************************************/