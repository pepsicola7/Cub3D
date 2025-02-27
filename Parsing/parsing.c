/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: peli <peli@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/12 14:43:10 by peli              #+#    #+#             */
/*   Updated: 2025/02/27 16:35:10 by peli             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	fill_map(t_data *data, char *line)
{
	int	i;

	i = 0;
	while (line && ft_strcmp(line, "\n") == 0) // Ignore les lignes vides initiales
	{
		free(line);
		line = get_next_line(data->map_data->map_fd);
	}
	data->map_data->map = ft_calloc(data->map_data->height + 1, sizeof(char *));
	while (line)
	{
		int len = ft_strlen(line);
		if (len > 0 && line[len - 1] == '\n')
			line[len - 1] = '\0';
		data->map_data->map[i] = ft_strdup(line);
		free(line);
		line = get_next_line(data->map_data->map_fd);
		i++;
	}
	data->map_data->map[i] = NULL;
	// ft_printf_map(data->map_data->map);
}

int	read_map_2(t_data *data, char *filename)
{
	int		i;
	char	*line;
	int		config_count;

	i = 0;
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

int	parsing(t_data *data, char *filename)
{
	data->map_data = ft_calloc(sizeof(t_map), 1);
	data->texture_data = ft_calloc(sizeof(t_map), 1);
	data->map_data->map_fd = open(filename, O_RDONLY);
	if (data->map_data->map_fd < 0)
	{
		printf("Error: Invalid map file\n");
		return (0);
	}
	count_line(data, filename);
	// read_map_1(data, filename);
	// if (!data->texture_data->floor_color || !data->texture_data->ceiling_color)
	// {
	// 	printf("The forme of the color incorrect\n");
	// 	return (0);
	// }
	// if (!data->texture_data->east || !data->texture_data->north
	// 	|| !data->texture_data->south || !data->texture_data->west)
	// {
	// 	printf("The forme of the texture incorrect\n");
	// 	return (0);
	// }
	read_map_2(data, filename);
	return (1);
}
