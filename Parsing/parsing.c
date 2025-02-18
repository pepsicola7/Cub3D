/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: peli <peli@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/12 14:43:10 by peli              #+#    #+#             */
/*   Updated: 2025/02/18 16:00:06 by peli             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

int	read_map_2(t_data *data)
{
	int		i;
	char	*line;

	i = 0;
	line = NULL;
	while (get_next_line(data->map_data->map_fd))
	{
		line = get_next_line(data->map_data->map_fd);
		if (ft_strncmp(line, "C ", 2) == 0)
		{
			free(line);
			line = get_next_line(data->map_data->map_fd);
			while (line == "\n")
			{
				free(line);
				line = get_next_line(data->map_data->map_fd);
				free(line);
			}
			while (line = get_next_line(data->map_data->map_fd))
			{
				data->map_data->map[i] = line;
				i++;
				free(line);
			}
		}
		free (line);
	}
	if (check_description(data))
		return (0);
	return (1);
}

int	parsing(t_data *data, char *filename)
{
	data->map_data->map_fd = open(filename, O_RDONLY);
	if (data->map_data->map_fd == -1)
		return (0);
	read_map_1(data); // pour le texture et les couleur;
	if (!data->texture_data->floor_color || !data->texture_data->ceiling_color)
	{
		printf("The forme of the color incorrect\n");
		return (0);
	}
	if (!data->texture_data->east || !data->texture_data->north
		|| !data->texture_data->south || !data->texture_data->west)
	{
		printf("The forme of the texture incorrect\n");
		return (0);
	}
	read_map_2(data); // pour la description;
	return (1);
}
