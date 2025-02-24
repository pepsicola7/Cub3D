/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_map.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: peli <peli@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/18 15:16:27 by peli              #+#    #+#             */
/*   Updated: 2025/02/24 19:39:12 by peli             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

int	check_map_start(char **map)
{
	int	i;

	i = 0;
	while (map[0][i])
	{
		while(map[0][i] == 32 | map[0][i] == 9)
			i++;
		while(map[0][i])
		{
			// printf("%c\n", map[0][i]);
			if (map[0][i] != '1' && map[0][i] != '\n')
			{
				printf("Error in the first line of the map");
				return (0);
			}
			i++;
		}
	}
	return (1);
}

int	check_description(t_data *data)
{
	char	**map;

	map = data->map_data->map;
	if(!check_map_start(map))
		return (0);
	return (1);
}
