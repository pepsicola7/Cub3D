/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lpolizzi <lpolizzi@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 20:38:37 by lpolizzi          #+#    #+#             */
/*   Updated: 2025/03/11 20:38:38 by lpolizzi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

int	get_map_value(t_data *data, int x, int y)
{
	if (x < 0 || x >= data->map_data->width || y < 0
		|| y >= data->map_data->height)
		return (1);
	return (data->map_data->map_1d[y * data->map_data->width + x]);
}
