/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clara <clara@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/23 19:14:09 by mgourlai          #+#    #+#             */
/*   Updated: 2024/01/30 19:03:30 by clara            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../cub3d.h"

int	minimap_to_map(t_game *game)
{
	int	i;

	i = 0;
	game->map = malloc(sizeof(int) * ((game->info_map.y + 3)
				* (game->info_map.longest + 2)));
	if (!game->minimap)
		return (-1);
	while (i < game->maps)
	{
		if (game->minimap[i] == 2)
			game->map[i] = 1;
		else
			game->map[i] = game->minimap[i];
		i++;
	}
	return (0);
}

int	connect_to_exec(t_game *game)
{
	game->tn = texture_path(game->info_map.file[game->info_map.no]);
	game->ts = texture_path(game->info_map.file[game->info_map.so]);
	game->tw = texture_path(game->info_map.file[game->info_map.we]);
	game->te = texture_path(game->info_map.file[game->info_map.ea]);
	game->mapy = game->info_map.y + 3;
	game->mapx = game->info_map.longest + 2;
	game->maps = game->mapy * game->mapx;
	game->fcolor = game->info_map.hexf;
	game->ccolor = game->info_map.hexc;
	if (minimap_to_map(game) == -1)
		return (-1);
	return (0);
}

void	initstruct(t_game *game, char **argv)
{
	int	fd;

	fd = open(argv[1], O_RDONLY);
	game->info_map.linecount = ft_get_linecount(fd);
	close(fd);
	game->info_map.map_start = 0;
	game->info_map.map_end = 0;
	game->info_map.longest = 0;
	game->info_map.no = -1;
	game->info_map.so = -1;
	game->info_map.we = -1;
	game->info_map.ea = -1;
	game->info_map.floor = 0;
	game->info_map.hexf = 0;
	game->info_map.ceiling = 0;
	game->info_map.hexc = 0;
	game->info_map.x = 0;
	game->info_map.y = 0;
}

// parsing core
int	parsing(t_game *game, char **argv)
{
	initstruct(game, argv);
	if (convert_file(game, argv) == -1)
		return (error("couldn't convert file, look out for tabs\n"));
	printf("--file converted\n");
	if (check_file(game, argv) == -1)
	{
		ft_error_frees(game, 1);
		return (-1);
	}
	if (maps(game) == -1)
		return (-1);
	return (0);
}

int	main(int argc, char **argv)
{
	t_game	game;

	if (argc != 2)
		return (1);
	if (parsing(&game, argv) == -1)
		return (1);
	if (connect_to_exec(&game) == -1)
		return (1);
	game.win_x = 960;
	game.win_y = 540;
	init_game(&game);
	game.window.mlx = mlx_init();
	game.window.win = mlx_new_window (game.window.mlx,
			game.win_x,
			game.win_y, "Club 3D");
	ft_raycasting(&game);
	mlx_hook(game.window.win, 2, 1L << 0, move_player, &game);
	mlx_hook(game.window.win, 17, 1L << 9, closew, &game);
	mlx_loop(game.window.mlx);
}
