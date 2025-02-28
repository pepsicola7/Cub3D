/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_map.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: peli <peli@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/28 15:38:55 by peli              #+#    #+#             */
/*   Updated: 2025/02/28 16:01:00 by peli             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

int	check_element(char **map)
{
	int	i;
	int	j;

	i = 0;
	while (map[i])
	{
		j = 0;
		while (map[i][j] != '\0' && map[i][j] != '\n')
		{
			if (map[i][j] != '1' && map[i][j] != '0' && map[i][j] != 'N'
			&& map[i][j] != 'S' && map[i][j] != 'E' && map[i][j] != 'W' && map[i][j] != ' ')
			{
				printf("Error of the element\n");
				return (0);
			}
			j++;
		}
		i++;
	}
	return (1);
}

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
		printf("Error: Aucun joueur trouvé sur la carte\n");
	else
		printf("Error: Trop de joueurs trouvés (%d)\n", player_count);
	return (0);
}

int check_espace(char **map, t_data *data)
{
	int	i;
	int	j;
	int	line_length;

	i = 0;
	while (map[i])
	{
		j = 0;
		line_length = (int)ft_strlen(map[i]);
		while (map[i][j])
		{
			if (map[i][j] == ' ')
			{
				if (i == 0 || i == data->map_data->height - 1 || j == 0 || j == line_length - 1)
				{
					if ((i > 0 && map[i - 1][j] == '0') || (i < data->map_data->height - 1 && map[i + 1][j] == '0')
						|| (j > 0 && map[i][j - 1] == '0') || (j < line_length - 1 && map[i][j + 1] == '0'))
						return (0);
				}
				else if ((i > 0 && map[i - 1][j] == '0') || (i < data->map_data->height - 1 && map[i + 1][j] == '0')
					|| (j > 0 && map[i][j - 1] == '0') || (j < line_length - 1 && map[i][j + 1] == '0'))
					return (0);
			}
			j++;
		}
		i++;
	}
	return (1);
}

int	check_line_vide(char **map)
{
	int	i;

	i = 0;
	while (map[i])
	{
		if (map[i][0] == '\0' || ft_strcmp(map[i], "\n") == 0)
		{
			printf("Error: empty line detected\n");
			return (0);
		}
		i++;
	}
	return (1);
}

int	check_mur(t_data *data)
{
	char	**map;
	int		x;
	int		y;

	x = data->player_data->pos.x;
	y = data->player_data->pos.y;
	map = copy_map(data->map_data->map, data->map_data->height);
	if (!map)
		return (0);
	flood_fill(map, x, y, data);
	// ft_printf_map(map);
	if (!check_flood(map, data->map_data->height))
	{
		printf("Error: Player is trapped!\n");
		free_map(map);
		return (0);
	}
	free_map(map);
	return (1);
}
