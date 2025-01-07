/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   image.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lde-merc <lde-merc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/07 09:59:08 by lde-merc          #+#    #+#             */
/*   Updated: 2025/01/07 10:27:23 by lde-merc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <mlx.h>
#include <stdlib.h>
#include <stdio.h>

int main() {
    void *mlx;
    void *win;
    void *img;
    int img_width, img_height;


    char *path1 = "mlx/test/open.xpm";
    char *path2 = "mlx/test/open24.xpm";
    char *path3 = "mlx/test/open30.xpm";
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
    img = mlx_xpm_file_to_image(mlx, path3, &img_width, &img_height);
    if (!img) {
        fprintf(stderr, "Failed to load image\n");
        return (1);
    }
    
    // Affichage de l'image
    mlx_put_image_to_window(mlx, win, img, 100, 100);

    // Boucle d'événements
    mlx_loop(mlx);

    return (0);
}
