/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aderby <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/09/02 01:57:38 by aderby            #+#    #+#             */
/*   Updated: 2017/10/27 13:57:47 by aderby           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "RTv1.h"

void    pixel_to_image(t_img *img, int x, int y, int color)
{
	int i;

	if (x < 0 || x > W || y < 0 || y > H)
		return ;
	i = (x) + (y * img->size);
	img->data[i] = color;
}

int		err_handle(char *str)
{
	write(1, str, ft_strlen(str));
	return (-1);
}

int		do_everything(t_wincon *w, void *mlx)
{
	t_img	*i;

	i = (t_img *)ft_memalloc(sizeof(t_img));
	if (!(i->img = mlx_new_image(mlx, W, H)))
		return (err_handle("The image failed to be allocated"));
	if (!(i->data = (int *)mlx_get_data_addr(i->img, &i->bpp, &i->size, &i->e)))
		return (err_handle("The image data failed to be allocated"));
	i->size /= 4;
	w->img = i;
	return (1);
}

double	magnitude(t_coord v)
{
	return ((double)sqrt(v.x * v.x + v.y * v.y + v.z * v.z));
}

void	normalize(t_coord *vector)
{
	double mag;

	mag = magnitude(*vector);
	vector->x /= mag;
	vector->y /= mag;
	vector->z /= mag;
}

t_coord	return_vector(t_coord s_pos, int x, int y)
{
	t_coord vector;

	vector.x = ((((double)x / (double)W) * 2.0) - 1.0) - s_pos.x;
	vector.y = ((((double)y / (double)H) * 2.0) - 1.0) - s_pos.y;
	vector.z = -1 - s_pos.z;
	normalize(&vector);
	return (vector);
}

t_coord	*raycast(t_wincon *w)
{
	int 		x;
	int 		y;
	t_coord		vector;
	t_coord		*vector_array;

	y = -1;
	vector_array = (t_coord*)ft_memalloc(sizeof(t_coord) * W * H);
	while (++y < W)
	{
		x = -1;
		while (++x < H)
		{
			vector = return_vector(w->camera.d1, x, y);
			vector_array[x + (y * W)] = vector;
		}
	}
	return (vector_array);
}

double	dotproduct(t_coord v1, t_coord v2)
{
	return ((double)(v1.x * v2.x + v1.y * v2.y + v1.z * v2.z));
}

double	sphere(t_wincon w, t_coord *v, t_obj s, int *xy)
{
	t_coord	ctoc;
	double	dot[3];
	double	hit[3];
	double	rad2;
	int		i;

	ctoc.x = s.position.x - w.camera.d1.x;
	ctoc.y = s.position.y - w.camera.d1.y;
	ctoc.z = s.position.z - w.camera.d1.z;
	rad2 = s.radius * s.radius;
	i = -1;
	dot[0] = dotproduct(ctoc, v[xy[1] + (xy[0] * W)]);
	dot[1] = dot[0] * dot[0] - dotproduct(ctoc, ctoc) + rad2;
	//dot[1] = dotproduct(ctoc, ctoc) dot[0] * dot[0];
	if (dot[1] < 0)
	{
		return (0.0);
	}
	dot[2] = sqrt(dot[1]);
	hit[0] = dot[0] - dot[2];
	hit[1] = dot[0] + dot[2];
	if (hit[0] > 0.0001 && hit[0] < hit[1])
		hit[2] = hit[0];
	if (hit[1] > 0.0001 && hit[1] < hit[0])
		hit[2] = hit[1];
	return (hit[2]);
}

t_coord	opposite_of_normalize(double dist, t_coord v)
{
	t_coord ret;
	ret.x = v.x * dist;
	ret.y = v.y * dist;
	ret.z = v.z * dist;
	return (ret);
}

t_coord	get_vector(t_coord pt1, t_coord pt2, int b)
{
	t_coord	v;

	v.x = pt1.x - pt2.x;
	v.y = pt1.y - pt2.y;
	v.z = pt1.z - pt2.z;
	if (b)
		normalize(&v);
	return (v);
}

void	add_vector(t_coord *v, t_coord n, double constant)
{
	v->x += n.x * constant;
	v->y += n.y * constant;
	v->z += n.z * constant;
}

int		get_colour_sphere(t_obj *obj, double dist, t_coord v)
{
	t_coord	hit;
	t_coord normal;
	t_coord vec;
	t_coord tolight;
	double	dot;

	hit = opposite_of_normalize(dist, v);
	normal = hit;
	normalize(&normal);
	add_vector(&v, normal, 0.00001);
	tolight = get_vector(hit, obj->lights->position, 1); // create light shortcut list for each object
	vec = get_vector(obj->position, hit, 1);
	dot = dotproduct(vec, tolight);
	if (dot < 0.075)
		dot = 0.075;
	return (((int)(obj->color.r * dot) << 16 | (int)(obj->color.g * dot) << 8 | (int)(obj->color.b * dot)));
}

int		get_colour_cone(t_obj *obj, double dist, t_coord v)
{
	t_coord	hit;
	t_coord normal;
	t_coord vec;
	t_coord tolight;
	t_coord vector;
	double	dot;

	hit = opposite_of_normalize(dist, v);
	normal = hit;
	normalize(&normal);
	add_vector(&v, normal, 0.00001);
	tolight = get_vector(hit, obj->lights->position, 1); // create light shortcut list for each object
	/*distance times the tangent of the angle of the cone maybe divided by 2*/
	vec = get_vector_dist(obj->position, hit);//not object position
	dot = dotproduct(vec, tolight);
	if (dot < 0.075)
		dot = 0.075;
	return (((int)(obj->color.r * dot) << 16 | (int)(obj->color.g * dot) << 8 | (int)(obj->color.b * dot)));
}
//int		get_colour_plane(t_obj *obj)
//{
//}

int		get_colour(t_obj *obj, double dist, t_coord v)
{
	if (obj->type == 's')
		return (get_colour_sphere(obj, dist, v));
	else if (obj->type == 'c')
		return (get_colour_cone(obj, dist, v));
	else
	{
		return (obj->color.r << 16 | obj->color.g << 8 | obj->color.b);
	}
	return (0);
}

double	plane(t_wincon w, t_coord *v, t_obj p, int *xy)
{
	double	dot;
	t_coord	vec;
	t_coord view;
	double dist;

	view = v[xy[1] + (xy[0] * W)];
	dot = dotproduct(p.rotation, view);
	if (dot > 0.000001)
	{
		vec.x = p.position.x - view.x;
		vec.y = p.position.y - view.y;
		vec.z = p.position.z - view.z;
		dist = dotproduct(vec, p.rotation) / dot;
		if (dist >= 0.0)
			return (dist);
	}
	return (0.0);
}

t_coord	quadratic_solver(double a, double b, double c)
{
	t_coord	xy;
	double	deter;
	double	square;

	deter = (b * b) - (4 * a *c);
	if (deter > 0)
	{
		square = sqrt(deter);
		xy.x = -b - (square / 2 * a);
		xy.y = -b + (square / 2 * a);
	}
	else if (deter == 0)
	{
		square = sqrt(deter);
		xy.x = -b - (square / 2 * a);
		xy.y = xy.x;
	}
	else
	{
		xy.x = 0.0;
		xy.y = 0.0;
	}
	return (xy);
}

double	cone_solver(t_obj c, t_coord d, t_coord v, t_coord o)
{
	t_coord abc;
	double	dot;
	t_coord sub;
	t_coord	xy;
	double	cos2;

	xy.x = 0.0;
	xy.y = 0.0;
	xy.z = 0.0;
	cos2 = c.angle * c.angle;
	dot = dotproduct(d, v); // d is the vecor of the view ray and v is the vec of the cone
	sub = get_vector(c.position, o);// o is the origin of the view ray
	abc.x = (dot * dot) - cos2; // c rad is the angle of the cone in radians
	abc.y = 2 * (dot * dotproduct(sub, v) - dotproduct(d, sub) * cos2);
	abc.z = (dotproduct(sub, v) * dotproduct(sub, v)) - (dotproduct(sub, sub) * cos2);
	xy = quadratic_solver(abc.x, abc.y, abc.z);
	if (xy.x > 0.0001 && xy.x < xy.y)
		return (xy.x);
	if (xy.y > 0.0001 && xy.y < xy.x)
		return (xy.y);
	return (0.0);
}

double	cone(t_wincon w, t_coord *v, t_obj c, int *xy)
{
	double dist;

	dist = cone_solver(c, v[xy[1] + (xy[0] * W)], c.rotation, w.camera.d1);
	return (dist);
}

void	draw(t_wincon *w, t_coord *v)
{
	int		xy[2];
	double	dist;
	double	mindist;
	t_obj	*head;
	t_obj	*closest_object;
	t_img	*tmp;

	xy[0] = -1;
	head = w->objects;
	while (++xy[0] < H)
	{
		xy[1] = -1;
		while (++xy[1] < W)
		{
			closest_object = NULL;
			dist = 0.0;
			mindist = 0.0;
	//		printf("%F and %F and %F\n", v[xy[1] + (xy[0] * W)].x, v[xy[1] + (xy[0] * W)].y, v[xy[1] + (xy[0] * W)].z);
			while (w->objects)
			{
				if (w->objects->type == 's')
				{
					dist = sphere(*w, v, *w->objects, xy);
				}
				else if (w->objects->type == 'c')
					dist = cone(*w, v, *w->objects, xy);
				//				else if (w->objects->type == 'y')
				//					dist = cylinder(w, v);
				else if (w->objects->type == 'p')
					dist = plane(*w, v, *w->objects, xy);
				else
					continue ;
				if ((dist < mindist || mindist == 0.0) && (dist > 0.0))
				{
					mindist = dist;
					closest_object = w->objects;
				}
				w->objects = w->objects->next;
			}
			if (closest_object)
			{
				pixel_to_image(w->img, xy[1], xy[0], get_colour(closest_object, mindist, v[xy[1] + (xy[0] * W)]));
			}
			w->objects = head;
		}
	}
}

void	temp(t_wincon *w)
{
	t_obj	*new;
	t_obj	*light;

	new = (t_obj *)ft_memalloc(sizeof(t_obj));
	light = (t_obj *)ft_memalloc(sizeof(t_obj));
//	new->type = 'p';
//	new->position.x = 1.0;
//	new->position.y = 1.0;
//	new->position.z = -10.0;
//	new->rotation.x = 0.0;
//	new->rotation.y = 1.0;
//	new->rotation.z = 0.0;
//	new->color.r = 15;
//	new->color.g = 155;
//	new->color.b = 95;
	new->type = 'c';
	new->radius = 1.5;
	new->angle = cos(15 * M_PI / 180);
	new->position.x = 0.0;
	new->position.y = 0.0;
	new->position.z = -10.0;
	new->rotation.x = 0.0;
	new->rotation.y = 1.0;
	new->rotation.z = 0.0;
	new->color.r = 15;
	new->color.g = 155;
	new->color.b = 95;
//	new->type = 's';
//	new->radius = 1.5;
//	new->position.x = 0.0;
//	new->position.y = 0.0;
//	new->position.z = -10.0;
//	new->color.r = 15;
//	new->color.g = 155;
//	new->color.b = 95;
	light->type = 'l';
	light->radius = 1.5;
	light->position.x = -5.0;
	light->position.y = -5.0;
	light->position.z = 0.0;
	light->color.r = 255;
	light->color.g = 255;
	light->color.b = 255;
	new->lights = light;
	w->objects = new;
}

int		main(int ac, char **av)
{
	t_wincon	*w;
	void		*mlx;
	t_coord		*vector_array;

	w = (t_wincon *)ft_memalloc(sizeof(t_wincon));
	if (ac != 2)
		return (err_handle("Usage: ./RTv1 <scenes/file>"));
	if (!(mlx = mlx_init()))
		return (err_handle("MLX failed to connect"));
	if (!(w->win = mlx_new_window(mlx, W, H, "RTv1")))
		return (err_handle("The window failed to load as you see."));
//if (!parse_data(w, av[1]))
//		return (err_handle(ERROR));
	ft_putstr("asdf\n");
	if (!do_everything(w, mlx))
		return (0);
	w->mlx = mlx;
	ft_putstr("asdfff\n");
	temp(w);
	ft_putstr("3\n");
	mlx_hook(w->win, 2, 0, key_press, w);
	mlx_hook(w->win, 17, 0, exit_program, w);
	mlx_expose_hook(w->win, expose_hook, w);
	ft_putstr("5\n");
	vector_array = raycast(w);
	ft_putstr("6\n");
	draw(w, vector_array);
	ft_putstr("7\n");
	mlx_loop_hook(w->mlx, loop_hook, w);
	//mlx_put_image_to_window(mlx, w.win, w.img->img, 0, 0);
	mlx_loop(mlx);
}
/*
   camera
   {
   position = { -2, 10, 11 }
   }
   light
   {
   position = { 16, 10, 11 }
   color = 0xFFFFFF
   }
   object
   {
   type = sphere
   position = { -15, 1, 50 }
   info
   {
   color = 0xFF0080
   shine = 256.0
   }
   }

   camera
   {
   position = { -2, 10, 11 }
   }
   light
   {
   position = { 16, 10, 11 }
   color = { 255, 255, 255 }
   }
   object
   {
   type = sphere
   position = { -15, 1, 50 }
   rosition = { -15, 1, 50 }
   info
   {
   color = { 255, 255, 255}
   shine = 256
   brightness = 256
   radius = 5
   width = 5
   height = 5
   }
   }*/
