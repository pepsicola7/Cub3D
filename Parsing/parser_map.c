/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_map.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: peli <peli@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/18 15:16:27 by peli              #+#    #+#             */
/*   Updated: 2025/02/25 17:01:12 by peli             ###   ########.fr       */
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
			if (map[i][j] == 'E' || map[i][j] == 'S' || map[i][j] == 'N' || map[i][j] == 'W')
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
			&& map[i][j] != 'S' && map[i][j] != 'E' && map[i][j] != 'W')
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
		{
			j++;
		}
		i++;
	}
	data->map_data->width = j - 1;
	printf("width is %d", j - 1);
}

void	remplace_map(t_data *data)
{
	char	**map;
	int		i;

	i = 0;
	map == data->map_data->map;
	if (ft_strlen(map[i]) != data->map_data->width)
	
}

int	check_description(t_data *data)
{
	char	**map;

	map_width(data);
	remplace_map(data);
	map = data->map_data->map;
	if (!check_element(map))
		return (0);
	if (!check_joueur(map))
		return (0);
	return (1);
}
