/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_data.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aderby <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/09/02 14:14:00 by aderby            #+#    #+#             */
/*   Updated: 2017/10/19 02:19:59 by aderby           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "RTv1.h"

int			object_check(t_obj *object)
{
	if (object->type == 's')
	{
		ft_putstr("asdf\n");
		if (object->radius <= 0.000)
			return (-1);
	}
	else
		if (object->width <= 0.000 || object->height <= 0.000)
			return (-1);
	return (1);
}

static int	parse_camera(int fd, char *line, t_wincon *w)
{
	t_cam	camera;
	t_coord	d1;
	t_coord	d2;
	char	**numbers;

	get_next_line(fd, &line);
	get_next_line(fd, &line);
	numbers = ft_strsplits(line, "\t {},=position");
	d1.x = (double)ft_atoi(numbers[0]);
	d1.y = (double)ft_atoi(numbers[1]);
	d1.z = (double)ft_atoi(numbers[2]);
	free(numbers[0]);
	free(numbers[1]);
	free(numbers[2]);
	free(numbers);
	camera.d1 = d1;
	camera.d2 = d2;
	w->camera = camera;
	get_next_line(fd, &line);
	if (line[0] != '}')
		return (-1);
	return (0);
}

static int	parse_light(int fd, char *line, t_wincon *w)
{
	t_wincon	*tmp;
	int			i;

	i = 4;
	tmp = w;
	while (tmp->objects != NULL)
		tmp->objects = tmp->objects->next;
	while (i-- > 0)
	{
		get_next_line(fd, &line);
		if (line[0] == '}')
			break ;
		if (line[0] == '{')
			continue ;
		else if (line[1] == 'c')
			tmp->objects->color = parse_color(line);//unfinished
		else if (line[1] == 'p')
			tmp->objects->position = parse_pos_rot(line);
	}
	if (!tmp->objects->position.x || !tmp->objects->color.r)
		return (-1);
	tmp->objects->type = 'l';
	return (1);
}

static int	parse_object(int fd, char *line, t_wincon *w)
{
	t_wincon	*tmp;
	int			i;

	i = 6;
	tmp = w;
	ft_putstr("asdf\n");
	while (tmp->objects != NULL)
		tmp->objects = tmp->objects->next;
	while (i > 0)
	{
		get_next_line(fd, &line);
		ft_putstr(line);
		if (line[0] == '}')
			break ;
		if (line[0] == '{')
			continue ;
		else if (line[1] == 't')
		{
			ft_putstr("foudn type\n");
			tmp->objects->type = (line[8]);
		}
		else if (line[1] == 'p')
			tmp->objects->position = parse_pos_rot(line);
		else if (line[1] == 'r')
			tmp->objects->rotation = parse_pos_rot(line);
		else if (line[1] == 'i')
			get_info(tmp, line, fd);
	}
	return (object_check(tmp->objects));
}

int			parse_data(t_wincon *w, char *scene)
{
	char	*line;
	int		fd;
	int		x;

	x = 0;
	fd = open(scene, O_RDONLY);
	while (get_next_line(fd, &line) != 0)
	{
		x++;
		ft_putnbr(x);
		if (line[0] == 'c')
		{
			if (!parse_camera(fd, line, w))
				return (-1);
		}
		else if (line[0] == 'l')
		{
			if (!parse_light(fd, line, w))
				return (-1);
		}
		else if (line[0] == 'o')
		{
			ft_putstr("parsing object\n");
			if (!parse_object(fd, line, w))
				return (-1);
		}
		else
			return (-1);
	}
	return (1);
}
