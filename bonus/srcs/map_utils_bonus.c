/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map__utils_bonus.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lpolizzi <lpolizzi@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 20:38:37 by lpolizzi          #+#    #+#             */
/*   Updated: 2025/03/12 12:24:45 by lpolizzi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d_bonus.h"

int	is_invalid_adjacent(char c)
{
	return (c == '0' || c == '2' || c == '3');
}

int	get_map_value(t_data *data, int x, int y)
{
	if (x < 0 || x >= data->map_data->width || y < 0
		|| y >= data->map_data->height)
		return (1);
	return (data->map_data->map_1d[y * data->map_data->width + x]);
}
