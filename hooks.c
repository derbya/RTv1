/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hooks.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aderby <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/09/06 06:36:54 by aderby            #+#    #+#             */
/*   Updated: 2017/10/19 15:02:54 by aderby           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "RTv1.h"

int		exit_program(t_wincon *w)
{
	exit(1);
	return (0);
}

int		key_press(int keycode, t_wincon *w)
{
	if (keycode == 53)
		exit(1);
	return (0);
}

int		expose_hook(t_wincon *w)
{
	w->draw = 1;
	return (0);
}

int		loop_hook(t_wincon *w)
{
	if (w->draw == 1)
	{
		w->draw = 0;
		mlx_put_image_to_window(w->mlx, w->win, w->img->img, 0, 0);
	}
	return (0);
}
