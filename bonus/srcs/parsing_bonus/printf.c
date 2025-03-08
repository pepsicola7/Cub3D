/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   printf.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: peli <peli@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 14:31:21 by peli              #+#    #+#             */
/*   Updated: 2025/02/28 16:31:10 by peli             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	count_line(t_data *data, char *filename)
{
	char	*line;
	int		count;

	count = 0;
	line = get_next_line(data->map_data->map_fd);
	while (line)
	{
		count++;
		free(line);
		line = get_next_line(data->map_data->map_fd);
	}
	close(data->map_data->map_fd);
	data->map_data->ligne_total = count;
	count_hors_map(data, filename);
}
