#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include "mlx.h"

# define k_ESC 65307

typedef struct s_canvas
{
	void	*img;
	char	*addr;
	int		bpp;
	int		ll;
	int		endian;
	int		w;
	int		h;
}	t_canvas;

typedef struct s_graphics_data
{
	void		*mlx;
	void		*win;
	int			ww;
	int			wh;
	int			exit_code;
	t_canvas	canvas;
}	t_graphics_data;

bool	init_graphics(t_graphics_data *gd);
void    cleanup(t_graphics_data *gd);
void	put_pixel(t_canvas *cdata, int x, int y, int color);
int		key_handler(int key, t_graphics_data *gd);
int		key_release(int key, t_graphics_data *gd);
void	show_info(void);

int	main(void)
{
	t_graphics_data	gd;
	char			option;

	show_info();
	scanf("%c", &option);
	if (option != '1' && option != '2')
	{
		printf("Wrong option! Exiting...\n");
		return (1);
	}

	if (!init_graphics(&gd))
		return (2);

	// set up window "x" button
	mlx_hook(gd.win, 17, 0, mlx_loop_end, gd.mlx);

	if (option == '1')
		mlx_key_hook(gd.win, key_handler, &gd);
	else if (option == '2')
		mlx_hook(gd.win, 3, 1L<<1, key_release, &gd);
	
	mlx_loop(gd.mlx);

	cleanup(&gd);
	return (0);
}

int	key_handler(int key, t_graphics_data *gd)
{
	if (key == k_ESC)
		mlx_loop_end(gd->mlx);
	return (1);
}

int	key_release(int key, t_graphics_data *gd)
{
	if (key == k_ESC)
		mlx_loop_end(gd->mlx);
	return (1);
}

void	show_info(void)
{
	printf("How should the exit ESC key be set up?\n");
	printf("Options: \n");
	printf(" 1. via mlx_key_hook\n");
	printf(" 2. via mlx_hook (expect a seg fault !)\n");
}

bool	init_graphics(t_graphics_data *gd)
{
	gd->ww = 800;
	gd->wh = 600;
	gd->mlx = mlx_init();
	if (!gd->mlx)
		return (false);
	gd->win = mlx_new_window(gd->mlx, gd->ww, gd->wh, "cub3D");
	if (!gd->win)
		return (free(gd->mlx), false);
	gd->canvas.img = mlx_new_image(gd->mlx, gd->ww, gd->wh);
	gd->canvas.addr = mlx_get_data_addr(gd->canvas.img, &gd->canvas.bpp, \
		&gd->canvas.ll, &gd->canvas.endian);
	gd->canvas.w = gd->ww;
	gd->canvas.h = gd->wh;
	return (true);
}

void    cleanup(t_graphics_data *gd)
{
	if (gd->canvas.img)
		mlx_destroy_image(gd->mlx, gd->canvas.img);
	if (gd->win)
		mlx_destroy_window(gd->mlx, gd->win);
	if (gd->mlx)
	{
		mlx_destroy_display(gd->mlx);
		free(gd->mlx);
	}
}

void	put_pixel(t_canvas *cdata, int x, int y, int color)
{
	char	*dst;

	dst = cdata->addr + (y * cdata->ll + x * (cdata->bpp / 8));
	*(unsigned int *)dst = color;
}

