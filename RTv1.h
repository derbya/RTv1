/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RTv1.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aderby <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/09/02 01:24:02 by aderby            #+#    #+#             */
/*   Updated: 2017/10/26 00:23:50 by aderby           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RTV1_H
# define RTV1_H

# include "./libft/inc/libft.h"
# include "./miniLibx/mlx.h"
# include <fcntl.h>
# include <math.h>
# include <stdio.h>

# define W 1000
# define H 1000
# define ERROR "Error: the scene provided is not valid"

typedef struct		s_clr
{
	int				r;
	int				g;
	int				b;
}					t_clr;

typedef struct		s_coord
{
	double			x;
	double			y;
	double			z;
}					t_coord;

typedef struct		s_ray
{
	t_coord			origin;
	t_coord			direction;
	struct s_ray	*next;
}					t_ray;

typedef struct		s_obj
{
	double			radius;
	double			width;
	double			angle;
	double			height;
	double			shine;
	double			brightness;
	char			type;
	t_coord			position;
	t_coord			rotation;
	t_clr			color;
	t_clr			e_color;
	struct s_obj	*next;
	struct s_obj	*lights;
}					t_obj;

typedef struct		s_cam
{
	t_coord			d1;
	t_coord			d2;
	double			aspect_ratio;
	t_ray			ray;
}					t_cam;

typedef struct		s_img
{
	void			*img;
	int				*data;
	int				bpp;
	int				size;
	int				e;
}					t_img;

typedef struct		s_wincon
{
	void			*mlx;
	void			*win;
	t_img			*img;
	t_ray			*ray0;
	/*t_ray			*ray1;
	t_ray			*ray2;
	t_ray			*ray3;
	t_ray			*ray4;
	t_ray			*ray5;
	t_ray			*ray6;
	t_ray			*ray7;*/
	int				draw;
	t_cam			camera;
	t_obj			*objects;
}					t_wincon;

int					parse_data(t_wincon *w, char *scene);
int					key_press(int keycode, t_wincon *w);
int					exit_program(t_wincon *w);
int					expose_hook(t_wincon *w);
int					loop_hook(t_wincon *w);
t_clr				parse_color(char *line);
t_coord				parse_pos_rot(char *line);
void				get_info(t_wincon *tmp, char *line, int fd);
#endif
