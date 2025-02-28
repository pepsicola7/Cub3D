/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_util.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: peli <peli@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/28 15:39:30 by peli              #+#    #+#             */
/*   Updated: 2025/02/28 15:40:42 by peli             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	map_width(t_data *data)
{
	int		i;
	int		j;
	char	**map;

	i = 0;
	j = 0;
	map = data->map_data->map;
	while (map[i])
	{
		while (map[i][j])
			j++;
		i++;
	}
	data->map_data->width = j - 1;
	// printf("width is %d", j - 1);
}

void	free_map_partial(char **map, int index)
{
	int	i;

	i = 0;
	while (i < index)
	{
		free(map[i]);
		i++;
	}
	free(map);
}

char	**copy_map(char **map, int height)
{
	int		i;
	char	**map_copy;

	i = 0;
	map_copy = ft_calloc(height, sizeof(char *));
	if (!map_copy)
		return (NULL);
	while (map[i])
	{
		map_copy[i] = ft_strdup(map[i]);
		if (!map_copy[i])
		{
			free_map_partial(map_copy, i);
			return (NULL);
		}
		i++;
	}
	map_copy[i] = NULL;
	return (map_copy);
}

void	free_map(char **map)
{
	int	i;

	i = 0;
	while (map[i])
	{
		free(map[i]);
		i++;
	}
	free(map);
}
