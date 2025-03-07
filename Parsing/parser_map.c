/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_map.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: peli <peli@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/18 15:16:27 by peli              #+#    #+#             */
/*   Updated: 2025/03/07 12:23:32 by peli             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

char	*replace_line(char *line, int width)
{
	int		j;
	char	*new_line;
	int		len;

	if (!line)
		return (NULL);
	len = ft_strlen(line);
	if (width < len)
		width = len;
	new_line = ft_calloc(width + 1, sizeof(char));
	if (!new_line)
		return (NULL);
	j = -1;
	while (++j < width && line[j])
	{
		if (line[j] == '\t' || line[j] == ' ')
			new_line[j] = '1';
		else
			new_line[j] = line[j];
	}
	while (j < width)
		new_line[j++] = '1';
	new_line[j] = '\0';
	return (new_line);
}


void	replace_map(t_data *data)
{
	char	**map;
	int		i;
	char	*new_line;

	i = 0;
	map = data->map_data->map;
	while (map[i])
	{
		new_line = replace_line(map[i], data->map_data->width);
		if (!new_line)
			return ;
		free(map[i]);
		map[i] = new_line;
		i++;
	}
}

void	flood_fill(char **map, int x, int y, t_data *data)
{
	int	height = data->map_data->height;
	int	width = data->map_data->width;

	if (x < 0 || x >= height || y < 0 || y >= width)
		return;
	if (map[x][y] == '1' || map[x][y] == 'F')
		return;
	if (map[x][y] != '0')
		return;
	map[x][y] = 'F';
	flood_fill(map, x, y + 1, data);
	flood_fill(map, x, y - 1, data);
	flood_fill(map, x + 1, y, data);
	flood_fill(map, x - 1, y, data);
}

int	check_flood(char **map, int height)
{
	int	i;
	int	j;
	int	line_length;

	i = -1;
	while (map[++i])
	{
		j = -1;
		line_length = ft_strlen(map[i]);
		{
			if (map[i][j] == 'F')
			{
				if (j == 0 || j == line_length - 1 || i == 0 || i == height - 1)
					return (0);
				if (map[i][j + 1] == '0' || map[i][j - 1] == '0' ||
					map[i + 1][j] == '0' || map[i - 1][j] == '0')
					return (0);
			}
		}
	}
	return (1);
}

int	check_joueur_extra(char **map, t_data *data)
{
	int	i, j, len;

	i = -1;
	while (map[++i])
	{
		len = (int)ft_strlen(map[i]);
		j = -1;
		while (map[i][++j])
		{
			if (ft_strchr("NSEW", map[i][j]))
			{
				if ((i == 0 || i == data->map_data->height - 1 || j == 0 || j >= len - 1) ||
					(i > 0 && (!map[i - 1] || !map[i - 1][j] || map[i - 1][j] == ' ')) ||
					(i < data->map_data->height - 1 && (!map[i + 1] || !map[i + 1][j] || map[i + 1][j] == ' ')) ||
					(j > 0 && (!map[i][j - 1] || map[i][j - 1] == ' ')) ||
					(j < len - 1 && (!map[i][j + 1] || map[i][j + 1] == ' ')))
					return (0);
			}
		}
	}
	return (1);
}


int	check_description(t_data *data)
{
	char	**map;

	map_width(data);
	map = data->map_data->map;
	if (!check_element(map))
		return (0);
	if (!check_joueur(map))
		return (0);
	if (!check_espace(map, data))
		return (0);
	if (!check_line_vide(map))
		return (0);
	position_player(data);
	if (!check_mur(data))
		return (0);
	if (!check_joueur_extra(map, data))
		return (0);
	replace_map(data);
	return (1);
}
