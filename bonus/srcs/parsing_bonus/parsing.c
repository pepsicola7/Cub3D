/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: peli <peli@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/12 14:43:10 by peli              #+#    #+#             */
/*   Updated: 2025/03/07 12:43:48 by peli             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d_bonus.h"

int	read_map_2(t_data *data, char *filename)
{
	char	*line;
	int		config_count;

	config_count = 0;
	data->map_data->map_fd = open(filename, O_RDONLY);
	line = get_next_line(data->map_data->map_fd);
	if (!help_read_map_2(data, &line, &config_count))
		return (0);
	fill_map(data, line);
	close(data->map_data->map_fd);
	if (!check_description(data))
		return (0);
	return (1);
}

void	count_lines(t_data *data, char **l, int *count)
{
	while (*l)
	{
		if (ft_strcmp(*l, "\n") == 0)
		{
			free(*l);
			(*count)++;
			*l = get_next_line(data->map_data->map_fd);
			continue ;
		}
		if (!(ft_strncmp(*l, "NO", 2) == 0 || ft_strncmp(*l, "SO", 2) == 0
				|| ft_strncmp(*l, "WE", 2) == 0 || ft_strncmp(*l, "EA", 2) == 0
				|| ft_strncmp(*l, "F", 1) == 0 || ft_strncmp(*l, "C", 1) == 0))
		{
			while (*l)
			{
				free(*l);
				*l = get_next_line(data->map_data->map_fd);
			}
			break ;
		}
		(*count)++;
		free(*l);
		*l = get_next_line(data->map_data->map_fd);
	}
}

void	count_hors_map(t_data *data, char *filename)
{
	char	*line;
	int		count;

	count = 0;
	data->map_data->map_fd = open(filename, O_RDONLY);
	line = get_next_line(data->map_data->map_fd);
	count_lines(data, &line, &count);
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
	if (!read_map_1(data, filename) || !data->texture->east
		|| !data->texture->north || !data->texture->south
		|| !data->texture->west || !data->texture->floor
		|| !data->texture->ceiling)
		return (0);
	if (!read_map_2(data, filename))
	{
		ft_putstr_fd("Error: invalid map\n", 2);
		return (0);
	}
	put_map_1d(data);
	init_door_textures(data);
	return (1);
}
