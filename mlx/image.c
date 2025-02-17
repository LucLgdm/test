/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   image.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lde-merc <lde-merc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/07 09:59:08 by lde-merc          #+#    #+#             */
/*   Updated: 2025/01/07 10:52:26 by lde-merc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <mlx.h>
#include <stdlib.h>
#include <stdio.h>

int main() {
    void *mlx;
    void *win;
    void **img;
    int *img_width, *img_height;
    char **path;


    img_height = malloc(3 * sizeof(int));
    img_width = malloc(3 * sizeof(int));
    
    path = malloc(3 * sizeof(char *));
    for (int i = 0; i < 3; i++){
        path[i] = malloc(50 * sizeof(char));
    }

    path[0] = "mlx/test/open.xpm";
    path[1] = "mlx/test/open24.xpm";
    path[2] = "mlx/test/open30.xpm";

    // Initialisation de MiniLibX
    mlx = mlx_init();
    if (!mlx) {
        fprintf(stderr, "Failed to initialize MLX\n");
        return (1);
    }

    // Création de la fenêtre
    win = mlx_new_window(mlx, 800, 600, "XPM Display");
    if (!win) {
        fprintf(stderr, "Failed to create window\n");
        return (1);
    }

    // Chargement de l'image XPM
    img = malloc(3 * sizeof(void *));
    for (int i = 0; i < 3; i++){
         img[i] = malloc (sizeof(void));
         img[i] = mlx_xpm_file_to_image(mlx, path[i], &img_width[i], &img_height[i]);
    }
    
    if (!img) {
        fprintf(stderr, "Failed to load image\n");
        return (1);
    }
    
    // Affichage de l'image
    for (int i = 0; i < 3; i++){
        mlx_put_image_to_window(mlx, win, img[i], i * 200, i * 200);
    }

    // Boucle d'événements
    mlx_loop(mlx);

    return (0);
}
