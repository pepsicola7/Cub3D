/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: peli <peli@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/12 14:43:10 by peli              #+#    #+#             */
/*   Updated: 2025/03/09 15:23:40 by peli             ###   ########.fr       */
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
			continue ;
		}
		if (ft_strncmp(line, "NO", 2) == 0 || ft_strncmp(line, "SO", 2) == 0
			|| ft_strncmp(line, "WE", 2) == 0 || ft_strncmp(line, "EA", 2) == 0
			|| ft_strncmp(line, "F", 1) == 0 || ft_strncmp(line, "C", 1) == 0)
			config_count++;
		else
		{
			if (config_count < 6)
			{
				free(line);
				ft_putstr_fd("Error: missing configuration in map file\n", 2);
				return (0);
			}
			break ;
		}
		free(line);
		line = get_next_line(data->map_data->map_fd);
	}
	fill_map(data, line);
	close(data->map_data->map_fd);
	if (!check_description(data))
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
			continue ;
		}
		if (!(ft_strncmp(line, "NO", 2) == 0 || ft_strncmp(line, "SO", 2) == 0
				|| ft_strncmp(line, "WE", 2) == 0 || ft_strncmp(line, "EA",
					2) == 0 || ft_strncmp(line, "F", 1) == 0 || ft_strncmp(line,
					"C", 1) == 0))
		{
			while (line)
			{
				free(line);
				line = get_next_line(data->map_data->map_fd);
			}
			break ;
		}
		count++;
		free(line);
		line = get_next_line(data->map_data->map_fd);
	}
	data->map_data->height = data->map_data->ligne_total - count;
}

void	put_map_1d(t_data *data)
{
	int	i;
	int	j;
	int	k;

	i = 0;
	k = 0;
	data->map_data->map_1d = ft_calloc(sizeof(char), data->map_data->width
			* data->map_data->height);
	while (i < data->map_data->height)
	{
		j = 0;
		while (j < data->map_data->width)
		{
			if (data->map_data->map[i][j] == 'N'
				|| data->map_data->map[i][j] == 'S'
				|| data->map_data->map[i][j] == 'E'
				|| data->map_data->map[i][j] == 'W')
				data->map_data->map_1d[k] = '0';
			else
				data->map_data->map_1d[k] = data->map_data->map[i][j];
			j++;
			k++;
		}
		i++;
	}
}

int	parsing(t_data *data, char *filename)
{
	data->map_data = ft_calloc(sizeof(t_map), 1);
	data->texture = ft_calloc(sizeof(t_texture_data), 1);
	data->map_data->map_fd = open(filename, O_RDONLY);
	if (data->map_data->map_fd < 0)
	{
		ft_putstr_fd("Error: map file not found\n", 2);
		return (0);
	}
	count_line(data, filename);
	read_map_1(data, filename);
	if (!data->texture->east || !data->texture->north || !data->texture->south
		|| !data->texture->west || !data->texture->floor_color || !data->texture->ceiling_color)
		return (0);
	if (!read_map_2(data, filename))
	{
		ft_putstr_fd("Error: invalid map\n", 2);
		return (0);
	}
	put_map_1d(data);
	return (1);
}
