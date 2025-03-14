/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: peli <peli@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 14:31:21 by peli              #+#    #+#             */
/*   Updated: 2025/03/14 17:01:31 by peli             ###   ########.fr       */
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

int	check_element(char **map)
{
	int	i;
	int	j;

	i = 0;
	while (map[i])
	{
		j = 0;
		while (map[i][j] != '\0' && map[i][j] != '\n')
		{
			if (map[i][j] != '1' && map[i][j] != '0' && map[i][j] != 'N'
			&& map[i][j] != 'S' && map[i][j] != 'E'
			&& map[i][j] != 'W' && map[i][j] != ' ')
			{
				ft_putstr_fd("Error: Invalid element found in map\n", 2);
				return (0);
			}
			j++;
		}
		i++;
	}
	return (1);
}

void	free_map(char **map)
{
	int	i;

	i = 0;
	while (map[i])
	{
		free(map[i]);
		i++;
	}
	free(map);
}

int	check_description(t_data *data)
{
	char	**map;

	map_width(data);
	map = data->map_data->map;
	if (!check_element(map))
		return (0);
	if (!check_joueur(map))
		return (0);
	if (!check_espace(map, data))
		return (0);
	if (!check_line_vide(data, map))
		return (0);
	position_player(data);
	if (!check_mur(data))
		return (0);
	if (!check_joueur_extra(map, data))
		return (0);
	replace_map(data);
	return (1);
}

int	help_read_map_2(t_data *data, char **l, int *config_count)
{
	while (*l)
	{
		if (ft_strcmp(*l, "\n") == 0 && *config_count < 6)
		{
			free(*l);
			*l = get_next_line(data->map_data->map_fd);
			continue ;
		}
		if (ft_strncmp(*l, "NO", 2) == 0 || ft_strncmp(*l, "SO", 2) == 0
			|| ft_strncmp(*l, "WE", 2) == 0 || ft_strncmp(*l, "EA", 2) == 0
			|| ft_strncmp(*l, "F", 1) == 0 || ft_strncmp(*l, "C", 1) == 0)
			(*config_count)++;
		else
		{
			if (*config_count < 6)
			{
				ft_putstr_fd("Error: missing configuration in map file\n", 2);
				return (free(*l), 0);
			}
			break ;
		}
		free(*l);
		*l = get_next_line(data->map_data->map_fd);
	}
	return (1);
}
