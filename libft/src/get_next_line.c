/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aderby <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/06/24 13:42:56 by aderby            #+#    #+#             */
/*   Updated: 2017/07/01 13:16:51 by aderby           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/libft.h"

static t_list		*ft_list_get(t_list **saved, int fd)
{
	t_list	*list;

	list = *saved;
	while (list)
	{
		if ((int)list->content_size == fd)
			return (list);
		list = list->next;
	}
	list = ft_lstnew("\0", fd);
	ft_lstadd(saved, list);
	list = *saved;
	return (list);
}

static int			ft_append(char **line, char *content, int i)
{
	while (content[i])
	{
		if (content[i] == '\n')
			break ;
		i++;
	}
	if (!(*line = ft_strnew(i)))
		return (-1);
	if (!(*line = ft_strjoin(*line, ft_strsub(content, 0, (i)))))
		return (-1);
	return (i);
}

int					get_next_line(const int fd, char **line)
{
	char			readto[BUFF_SIZE + 1];
	static t_list	*saved;
	t_list			*nline;
	int				i;
	int				len;

	if (line == NULL || fd < 0 || read(fd, readto, 0) < 0)
		return (-1);
	nline = ft_list_get(&saved, fd);
	if (!(*line = ft_strnew(0)))
		return (-1);
	while ((i = read(fd, readto, BUFF_SIZE)))
	{
		readto[i] = '\0';
		if (!(nline->content = ft_strjoin(nline->content, readto)))
			return (-1);
		if (ft_strchr(readto, '\n'))
			break ;
	}
	if (i < 1 && !ft_strlen(nline->content))
		return (0);
	len = ft_append(line, nline->content, 0);
	(len < (int)ft_strlen(nline->content)) ? nline->content += len + 1
		: ft_strclr(nline->content);
	return (1);
}
