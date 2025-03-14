/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_map.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: peli <peli@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/28 15:38:55 by peli              #+#    #+#             */
/*   Updated: 2025/03/14 17:39:59 by peli             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

int	check_joueur(char **map)
{
	int	i;
	int	j;
	int	player_count;

	i = 0;
	player_count = 0;
	while (map[i])
	{
		j = 0;
		while (map[i][j] != '\0' && map[i][j] != '\n')
		{
			if (map[i][j] == 'E' || map[i][j] == 'S' || map[i][j] == 'N'
				|| map[i][j] == 'W')
				player_count++;
			j++;
		}
		i++;
	}
	if (player_count == 1)
		return (1);
	if (player_count == 0)
		ft_putstr_fd("Error: No player found\n", 2);
	else
		ft_putstr_fd("Error: Too many players found\n", 2);
	return (0);
}

int	check_espace(char **map, t_data *data)
{
	int	i;
	int	j;
	int	len;

	i = -1;
	while (map[++i])
	{
		len = (int)ft_strlen(map[i]);
		j = -1;
		while (map[i][++j])
		{
			if (map[i][j] == ' ')
			{
				if ((i > 0 && map[i - 1] && j < (int)ft_strlen(map[i - 1])
						&& map[i - 1][j] == '0') || (i < data->map_data->height
						- 1 && map[i + 1] && j < (int)ft_strlen(map[i + 1])
						&& map[i + 1][j] == '0') || (j > 0 && map[i][j - 1]
						&& map[i][j - 1] == '0') || (j < len - 1 && map[i][j
						+ 1] && map[i][j + 1] == '0'))
					return (printf("Error : Espace invalide\n"), 0);
			}
		}
	}
	return (1);
}

int	check_line_vide(t_data *data, char **map)
{
	int	i;
	int	last_non_empty;

	i = 0;
	last_non_empty = -1;
	while (map[i] && i < data->map_data->height)
	{
		if (map[i][0] != '\0' && ft_strcmp(map[i], "\n") != 0)
			last_non_empty = i;
		i++;
	}
	i = 0;
	while (i <= last_non_empty)
	{
		if (map[i][0] == '\0' || ft_strcmp(map[i], "\n") == 0)
		{
			ft_putstr_fd("Error: Found empty line in map\n", 2);
			return (0);
		}
		i++;
	}
	return (1);
}

int	check_wall(char **map, int height)
{
	int	i;
	int	j;

	i = 0;
	while (map[i])
	{
		printf ("%s\n", map[i]);
		j = 0;
		if (i == 0 || i == height - 1)
		{
			while (map[i][j])
			{
				if (map[i][j] == '0' || map[i][j] == '0')
					return (0);
				j++;
			}
		}
		else if (map[i][0] == '0' || map[i][ft_strlen(map[i]) - 1] == '0')
			return (0);
		i++;
	}
	return (1);
}

int	check_mur(t_data *data)
{
	char	**map;
	int		x;
	int		y;

	x = data->player->pos.x;
	y = data->player->pos.y;
	map = copy_map(data, data->map_data->map, data->map_data->height);
	if (!map)
		return (0);
	flood_fill(map, y, x, data);
	if (!check_wall(map, data->map_data->height) || !check_flood(map,
			data->map_data->height))
	{
		ft_putstr_fd("Error: The map is not closed!\n", 2);
		free_map(map);
		return (0);
	}
	free_map(map);
	return (1);
}
