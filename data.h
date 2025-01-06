/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   data.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lde-merc <lde-merc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/17 13:53:51 by lde-merc          #+#    #+#             */
/*   Updated: 2025/01/06 17:14:54 by lde-merc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DATA_H
# define DATA_H

# include "mlx/mlx.h"
#include <stdio.h>
#include <stdlib.h>

typedef struct s_circle
{
	int	center_x;
	int	center_y;
	int	radius;
	int	color;
}	t_circle;

typedef struct s_data
{
	void	*img;
	char	*addr;
	int		bits_per_pixel;
	int		line_length;
	int		endian;
}	t_data;


typedef struct s_param
{
	void	*mlx;
	void	*window;
	int		width_w;
	int		height_w;
	t_circle *circle;
	t_data	*data;
} t_param;




// Drawing
void	my_mlx_pixel_put(t_data *data, int x, int y, int color);

void draw_circle(t_circle *circle, t_data *img);

void clear_image(t_param *par);

// Hook
int		close(int keycode, void *mlx, void *window);

int		key_handle(int key, void *param);
// Color
int	create_color(int t, int r, int g, int b);

int get_transparency(int color);

int get_red(int color);

int get_green(int color);

int get_blue(int color);

int add_shade(double distance, int color);

int	get_opposite (int color);


// Exit
void clean_up_and_exit(t_param *par);

#endif