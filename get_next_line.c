/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-adna <mel-adna@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/24 11:26:30 by mel-adna          #+#    #+#             */
/*   Updated: 2024/11/30 22:38:10 by mel-adna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

void	free_str(char **str)
{
	if (*str)
	{
		free(*str);
		*str = NULL;
	}
}

char	*extract_line(char **strline)
{
	char	*newline;
	char	*remaining;
	size_t	i;

	i = 0;
	while ((*strline)[i] && (*strline)[i] != '\n')
		i++;
	if ((*strline)[i] == '\n')
		newline = malloc(i + 2);
	else
		newline = malloc(i + 1);
	if (!newline)
		return (free(*strline), NULL);
	ft_memcpy(newline, *strline, i);
	if ((*strline)[i] == '\n')
		newline[i++] = '\n';
	newline[i] = '\0';
	remaining = ft_strdup(*strline + i);
	if (!remaining)
		return (free(newline), free(*strline), NULL);
	free_str(strline);
	*strline = remaining;
	return (newline);
}

char	*read_and_concat(int fd, char **strline)
{
	char		*bufferstr;
	char		*temp;
	ssize_t		nread;

	bufferstr = malloc(BUFFER_SIZE + 1);
	if (!bufferstr)
		return (NULL);
	nread = read(fd, bufferstr, BUFFER_SIZE);
	while (nread > 0)
	{
		bufferstr[nread] = '\0';
		temp = ft_strjoin(*strline, bufferstr);
		if (!temp)
			return (free(bufferstr), free_str(strline), NULL);
		free_str(strline);
		*strline = temp;
		if (ft_strchr(bufferstr, '\n'))
			break ;
		nread = read(fd, bufferstr, BUFFER_SIZE);
	}
	free(bufferstr);
	if (nread < 0 || **strline == '\0')
		return (free_str(strline), NULL);
	return (*strline);
}

char	*get_next_line(int fd)
{
	static char	*strline = NULL;
	char		*line;

	if (fd < 0 || BUFFER_SIZE <= 0 || read(fd, 0, 0) < 0)
		return (free_str(&strline), NULL);
	if (!strline)
		strline = ft_strdup("");
	line = read_and_concat(fd, &strline);
	if (!line)
		return (NULL);
	return (extract_line(&strline));
}
