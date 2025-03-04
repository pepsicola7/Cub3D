/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: peli <peli@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/12 14:43:10 by peli              #+#    #+#             */
/*   Updated: 2025/02/28 16:30:50 by peli             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

int	read_map_2(t_data *data, char *filename)
{
	char	*line;
	int		config_count;

	config_count = 0;
	data->map_data->map_fd = open(filename, O_RDONLY);
	line = get_next_line(data->map_data->map_fd);
	while (line)
	{
		if (ft_strcmp(line, "\n") == 0 && config_count < 6)
		{
			free(line);
			line = get_next_line(data->map_data->map_fd);
			continue;
		}
		if (ft_strncmp(line, "NO", 2) == 0 || ft_strncmp(line, "SO", 2) == 0 ||
			ft_strncmp(line, "WE", 2) == 0 || ft_strncmp(line, "EA", 2) == 0 ||
			ft_strncmp(line, "F", 1) == 0 || ft_strncmp(line, "C", 1) == 0)
			config_count++;
		else
		{
			if (config_count < 6)
			{
				free(line);
				ft_putstr_fd("Error: missing configuration in map file\n", 2);
				return (0);
			}
			break;
		}
		free(line);
		line = get_next_line(data->map_data->map_fd);
	}
	fill_map(data, line);
	close(data->map_data->map_fd);
	if (check_description(data))
		return (0);
	return (1);
}
void	count_hors_map(t_data *data, char *filename)
{
	char	*line;
	int		count;

	count = 0;
	data->map_data->map_fd = open(filename, O_RDONLY);
	line = get_next_line(data->map_data->map_fd);
	while (line)
	{
		if (ft_strcmp(line, "\n") == 0)
		{
			free(line);
			count++;
			line = get_next_line(data->map_data->map_fd);
			continue;
		}
		if (!(ft_strncmp(line, "NO", 2) == 0 || ft_strncmp(line, "SO", 2) == 0 ||
			ft_strncmp(line, "WE", 2) == 0 || ft_strncmp(line, "EA", 2) == 0 ||
			ft_strncmp(line, "F", 1) == 0 || ft_strncmp(line, "C", 1) == 0))
		{
			while (line)
			{
				free(line);
				line = get_next_line(data->map_data->map_fd);
			}
			break;
		}
		count++;
		free(line);
		line = get_next_line(data->map_data->map_fd);
	}
	data->map_data->height = data->map_data->ligne_total - count;
}

int	parsing(t_data *data, char *filename)
{
	data->map_data = ft_calloc(sizeof(t_map), 1);
	data->texture = ft_calloc(sizeof(t_map), 1);
	data->map_data->map_fd = open(filename, O_RDONLY);
	if (data->map_data->map_fd < 0)
	{
		printf("Error: Invalid map file\n");
		return (0);
	}
	count_line(data, filename);
	read_map_1(data, filename);
	if (!data->texture->floor_color || !data->texture->ceiling_color)
	{
		printf("The forme of the color incorrect\n");
		return (0);
	}
	if (!data->texture->east || !data->texture->north
		|| !data->texture->south || !data->texture->west)
	{
		printf("The forme of the texture incorrect\n");
		return (0);
	}
	read_map_2(data, filename);
	return (1);
}
