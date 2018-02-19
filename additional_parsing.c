/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   additional_parsing.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aderby <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/09/02 15:46:40 by aderby            #+#    #+#             */
/*   Updated: 2017/09/03 13:20:02 by aderby           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "RTv1.h"

t_clr		parse_color(char *line)
{
	char	**numbers;
	t_clr	color;

	numbers = ft_strsplits(line, "\tcolor= {},");
	color.r = ft_atoi(numbers[0]);
	color.g = ft_atoi(numbers[1]);
	color.b = ft_atoi(numbers[2]);
	free(numbers[0]);
	free(numbers[1]);
	free(numbers[2]);
	free(numbers);
	return (color);
}

t_coord		parse_pos_rot(char *line)
{
	t_coord posrot;
	char	**numbers;

	numbers = ft_strsplits(line, "\t,={}position rotation");
	posrot.x = (double)ft_atoi(numbers[0]);
	posrot.y = (double)ft_atoi(numbers[1]);
	posrot.z = (double)ft_atoi(numbers[2]);
	free(numbers[0]);
	free(numbers[1]);
	free(numbers[2]);
	free(numbers);
	return (posrot);
}

void		get_info(t_wincon *tmp, char *line, int fd)
{
	while (get_next_line(fd, &line))
	{
		if (line[0] == '}')
			break ;
		if (line[0] == '{')
			continue ;
		else if (line[2] == 'c')
			tmp->objects->color = parse_color(line);
		else if (line[2] == 's')
			tmp->objects->shine = (double)ft_atoi(line + 11);
		else if (line[2] == 'b')
			tmp->objects->brightness = (double)ft_atoi(line + 15);
		else if (line[2] == 'w')
			tmp->objects->width = (double)ft_atoi(line + 10);
		else if (line[2] == 'h')
			tmp->objects->height = (double)ft_atoi(line + 11);
		else if (line[2] == 'r')
			tmp->objects->radius = (double)ft_atoi(line + 11);
	}
}
