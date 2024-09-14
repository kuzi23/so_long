#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

typedef struct s_game
{
    char    **map;
    char    **lines;
    int     lines_total;
    int     player;
    int     exit;
    int     collectibles;
    int     wall;
    int     to_eat;
}   t_game;

typedef struct s_mini
{

} t_mini;

void error_report(const char *msg) {
    perror(msg);
    exit(EXIT_FAILURE);
}

void storing_lines(t_game *game, int fd)
{
    char *line;
    int i;

    i = 0;
    game->lines_total = 0;
    game->lines = NULL;
    while ((line = get_next_line(fd)) != NULL)
    {
        game->lines = (char **)realloc(game->lines, sizeof(char *) * (game->lines_total + 1));
        if (!game->lines)
        {
            error_report("map error");
        }
        game->lines[game->lines_total] = line;
        game->lines_total++;
    }
    game->lines = (char **)realloc(game->lines, sizeof(char *) * (game->lines_total + 1));
    game->lines[game->lines_total] = NULL;
    close(fd);
}

void mid_line_check(char *str, t_game *game)
{
    int i;
    int length;

    i = 1;
    length = strlen(str);
    if (str[0] != '1' || str[length - 1] != '1')
        return;
    while (str[i] != '\0')
    {
        if (str[i] == 'P')
            game->player++;
        else if (str[i] == 'E')
            game->exit++;
        else if (str[i] == 'C')
            game->to_eat++;
        i++;// aha hari agategteko ko gushyiraho ko kugirango igihe yaba ari indi charachetera itandukanye niz bahite bakora break
    }
}

int first_lines_check(char *str)
{
    int i;

    i = 0;
    while (str[i] != '\0')
    {
        if (str[i] != '1')
            return 0;
        i++;
    }
    return 1;
}

char *parse_map(const char *map_file)
{
    int i;
    int fd;
    t_game imirongo;

    i = 0;
    fd = open(map_file, O_RDONLY);
    if (fd == -1)
    {
        perror("Error opening file");
        return NULL;
    }
    storing_lines(&imirongo, fd);

    while (i < imirongo.lines_total)
    {
        if (i != 0 && i != (imirongo.lines_total - 1))
            mid_line_check(imirongo.lines[i], &imirongo);
        else if (i == 0 || i == (imirongo.lines_total - 1))
        {
            if (!first_lines_check(imirongo.lines[i])) // harimo ikosa ukuntu ariko ntabidanje "first_line ni int kandi iri gukoreshwa nka void"
                write(1, "wall not sealed\n", 16);
        }
        i++;
    }

    // Free allocated memory( nnaka chat gpt)
    for (i = 0; i < imirongo.lines_total; i++)
        free(imirongo.lines[i]);
    free(imirongo.lines);

    return (char *)map_file;
}



int	main(int argc, char **argv)
{
	int	i;
	t_game *game;

	i = 0;
	if (argc != 2)
		error_report("must be only 2 arguments");
	game->map = parse_map(argv[1]);
	//check_map(game->map);
    start_game(game);
}

void check_shape(t_game *game)
{
    int i;
    int items_init;
    int j;

    i = 0;
    items_init = 0;
    while (game->lines[0][items_init] != '\0')
    {
        items_init++;
    }
    while (i < game->lines_total)
    {
        j = 0;
        while (game->lines[i][j] != '\0')
        {
            j++;
        }

        if (j != items_init)
        {
            printf("Error: The map is not rectangular at line %d.\n", i + 1);
            return; // Exit the function if the shape is invalid
        }
        i++;
    }

    printf("The map has a valid rectangular shape.\n");
}

void are_unique_on_map(t_game *game)
{
    int i;

    i = 0;
    while (game->lines[game->lines_total--][i] != '\0')// iyo ushatse gukora kuri iyi lines total ihita ihindura iriya wabonaga hejuru kbsa
    {
        if (game->lines[game->lines_total][i] == '1')
            game->wall++;
        if (game->lines[game->lines_total][i] == 'c')
            game->collectibles++;/// any other character izajyamo itari izi igomba kuzaba ariyo space..
        i++;
    }

    if(game->player != 1 && game->exit != 1)
        printf("many player characters or not included");

    if (game->collectibles < 0)
        printf("no collectibles found");
}

void are_unique_accessible(char)
{


}

void *check_map(t_game *game)
{
    check_shape(game);
    are_unique_on_map(game);
    are_unique_accesible(game);
}