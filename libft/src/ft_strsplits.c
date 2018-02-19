/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strsplits.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aderby <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/06/14 16:37:55 by aderby            #+#    #+#             */
/*   Updated: 2017/06/14 20:51:08 by aderby           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char		**ft_strsplits(char *s, char *c)
{
	char *chold;
	char *pthold;
	char *ptr;

	chold = c;
	ptr = ft_strdup(s);
	pthold = ptr;
	while (*ptr)
	{
		while (*c)
		{
			if (*ptr == *c)
			{
				*ptr = *chold;
				break ;
			}
			c++;
		}
		c = chold;
		ptr++;
	}
	ptr = pthold;
	s = ft_strdup(ptr);
	ft_strdel(&ptr);
	return (ft_strsplit(s, *chold));
}
