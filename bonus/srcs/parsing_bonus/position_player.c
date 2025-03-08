/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   position_player.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: peli <peli@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/28 16:03:09 by peli              #+#    #+#             */
/*   Updated: 2025/02/28 16:20:18 by peli             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	direction_ns(t_data *data, char direction)
{
	if (direction == 'N')
	{
		data->player->dir.x = 0;
		data->player->dir.y = -1;
		data->player->plane.x = 0.66;
		data->player->plane.y = 0;
	}
	if (direction == 'S')
	{
		data->player->dir.x = 0;
		data->player->dir.y = 1;
		data->player->plane.x = -0.66;
		data->player->plane.y = 0;
	}
}

void	direction_ew(t_data *data, char direction)
{
	if (direction == 'E')
	{
		data->player->dir.x = 1;
		data->player->dir.y = 0;
		data->player->plane.x = 0;
		data->player->plane.y = 0.66;
	}
	if (direction == 'W')
	{
		data->player->dir.x = -1;
		data->player->dir.y = 0;
		data->player->plane.x = 0;
		data->player->plane.y = -0.66;
	}
}

void	position_player(t_data *data)
{
	char	**map;
	int		i;
	int		j;
	char	direction;

	i = 0;
	map = data->map_data->map;
	data->player = ft_calloc(1, sizeof(t_player));
	init_player_jump(data->player);
	while (map[i])
	{
		j = 0;
		while (map[i][j])
		{
			if (map[i][j] =='N' || map[i][j] =='S' || map[i][j] =='E' || map[i][j] =='W')
			{
				data->player->pos.y = i + 0.5;
				data->player->pos.x = j + 1.5;
				direction = map[i][j];
				direction_ns(data, direction);
				direction_ew(data, direction);
				return ;
			}
			j++;
		}
		i++;
	}
}
