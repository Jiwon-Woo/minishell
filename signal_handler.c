#include "minishell.h"

void	child_sigquit_handler(int signo)
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

void	child_sigint_handler(int signo)
{
	if (signo == SIGINT)
	{
		write(2, "\n", 1);
		g_status = 130;
	}
	signal(SIGINT, (void *)sigint_handler);
}

void	redirect_sigint_handler(int signo)
{
	if (signo == SIGINT)
	{
		printf("\n");
		g_status = 1;
	}
	signal(SIGINT, (void *)sigint_handler);
}

void	set_signal()
{
	signal(SIGINT, (void *)sigint_handler);
	signal(SIGQUIT, SIG_IGN);
}

void	sigquit_handler(int signo)
{
	if (signo == SIGINT)
	{
		signal(SIGINT, sigquit_handler);
		g_status = 130;
	}
	if (signo == SIGQUIT)
	{
		signal(SIGQUIT, sigquit_handler);
		g_status = 131;
	}
}