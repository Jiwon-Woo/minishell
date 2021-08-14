/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal_handler.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jwoo <jwoo@student.42seoul.kr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/10 12:03:30 by jwoo              #+#    #+#             */
/*   Updated: 2021/08/14 10:25:50 by jwoo             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	sigint_handler(int signo)
{
	if (signo == SIGINT)
	{
		rl_on_new_line();
		rl_redisplay();
		printf("%c[K\n", 27);
		rl_replace_line("", 0);
		rl_on_new_line();
		rl_redisplay();
		g_status = 1;
	}
	signal(SIGINT, (void *)sigint_handler);
}

void	child_handler(int signo)
{
	if (signo == SIGINT)
	{
		write(2, "\n", 1);
		g_status = 130;
	}
	if (signo == SIGQUIT)
	{
		write(2, "Quit: 3\n", ft_strlen("Quit: 3\n"));
		g_status = 131;
	}
}

void	redirect_handler(int signo)
{
	if (signo == SIGINT)
	{
		printf("\n");
		g_status = 1;
	}
	signal(SIGINT, (void *)sigint_handler);
}

void	set_child_signal(void)
{
	signal(SIGINT, child_handler);
	signal(SIGQUIT, child_handler);
}

void	set_signal(void)
{
	signal(SIGINT, (void *)sigint_handler);
	signal(SIGQUIT, SIG_IGN);
}
