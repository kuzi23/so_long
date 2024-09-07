/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkwizera <mkwizera@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/10 20:31:29 by mkwizera          #+#    #+#             */
/*   Updated: 2024/09/07 19:44:46 by mkwizera         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "get_next_line/get_next_line.h"
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>


typedef struct s_game
{
	char	**map;// kubika umubare wimirongo iri muri map
    char	**lines; //kunika imirono ya map ari byo bidufasha kumanya 
    int		lines_total;
    int		player;// P
    int		exit;// E
    int		to_eat;
}	t_game;

void  *storing_lines(t_game game,int fd)
{
    char *line;
    int    i;

    i = 0;
    game.lines_total = 0;
    game.lines = NULL;
    while (line = get_next_line(fd) != "\0")
    {
        game.lines = (char *)malloc(sizeof(char *) * (game.lines_total + 1));//kyuko tutazi ibyo tugiye kwakira uko bingana niyo mpamvu dukoze
        if (!game.lines)
        {
            error_report("map error"); // iyi function ngomba nayo kuyikora kbsa niyo kudisplaying errors 
            break;
        }
        game.lines[game.lines_total] = line;
        game.lines_total++;
    }
    game.lines[game.lines_total] = '\0'; 
    free(game.lines);
}

void	mid_line_check(t_game game,char *str)
{
    int i;
    int length;

    i = 1;
    length = strlen(str);
    if(str[0] != 49 && str[length -1] != 49) //nuguchecking ko character yambere ndetse niyanyuma isa 1
        return (-1);
    while (str[i] != '\0')
    {
		if (str[i] == 'P')
			game.player += 1;
        else if (str[i] == 'E')
			game.exit += 1;
        else if (str[i] == 'C')
			game.to_eat += 1;
        i++;
    }
}

int first_lines_check(char *str)
{
    int  i;

    i = 0;
    while(str[i] != '\0')
    {
        if(str[i] != '1')
        {
            return (0);
            break ;
        }
        i++;    
    }
    return (1);
}

char	*parse_map(char *map)
{
	int	i;
	int	fd;
	t_game imirongo;


	i = 0;
    fd = open(map, O_RDONLY); // aha turiguhindura map yaje ari string muri file kugira ngo tubashe gukora manipulation
    if (fd == -1) {
        perror("Error opening file");
        return (1);
    }
	storing_lines(imirongo,fd);// irahita ya updating ziriya boro dufite muri t_game lines, lines_total
    close(fd);
     
    while (i <= imirongo.lines_total) //kunyura mu mirongo dukora checkup
    {
        if (i != 0 && i!= imirongo.lines_total)
            mid_line_check(imirongo.lines[i]);
        else if (i == 1 || i == (imirongo.lines_total -1))
            if (!(first_lines_check(imirongo.lines[i])))
                write(1, "wall not sealed\n", 16); 
        i++;
    }
    return (map);
}

int	main(int argc, char **argv)
{
	int	i;
	t_game game;

	i = 0;
	if (argc != 2)
		error_report("must be only 2 arguments");
	game.map = parse_map(argv[1]);
	check_map(game.map);
}

// #include "minilibx-linux/mlx.h"

// typedef struct s_data
// {
//     void *img;
//     char *addr;
//     int  bits_per_pixel;
//     int  line_length;
//     int  endian;
// }   t_data;

// void my_mlx_pixel_put(t_data *data, int x, int y, int color)
// {
//     char *dst;
    
//     dst = data->addr + (y * data->line_length + x * (data->bits_per_pixel / 8));
//     *(unsigned int*)dst = color;
// }

// int main(void)
// {
//     void *mlx;
//     void *mlx_win;
//     t_data  img;

//     mlx = mlx_init();
//     mlx_win = mlx_new_window(mlx, 1000, 1000, "Hello world");
//     img.img =  mlx_new_image(mlx, 1000, 1000);
//     img.addr = mlx_get_data_addr(img.img, &img.bits_per_pixel, 
//     &img.line_length, &img.endian);
//     my_mlx_pixel_put(&img, 100, 100, 0x00FF0000);
//     mlx_put_image_to_window(mlx, mlx_win, img.img, 0, 0);
//     mlx_loop(mlx);
// }
