/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jwoo <jwoo@student.42seoul.kr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/10 21:42:54 by jwoo              #+#    #+#             */
/*   Updated: 2021/08/12 17:11:11 by jwoo             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*make_prompt(void)
{
	char	*prompt;
	char	*buffer;

	buffer = getcwd(NULL, 0);
	prompt = ft_strjoin(buffer, "$ ");
	if (buffer != 0)
		free(buffer);
	buffer = 0;
	return (prompt);
}

void	minishell(char **first_envp)
{
	t_cmd		*cmd;
	char		**line_prompt;
	t_envp		envp;
	t_fd		*fd;

	init_envp_status(&envp, first_envp);
	set_signal();
	line_prompt = (char **)malloc(sizeof(char *) * 2);
	cmd = (t_cmd *)malloc(sizeof(t_cmd));
	fd = (t_fd *)malloc(sizeof(t_fd));
	if (!line_prompt || !cmd || !fd)
		exit(1);
	line_prompt[1] = make_prompt();
	line_prompt[0] = readline(line_prompt[1]);
	while (line_prompt[0] != NULL)
	{
		handle_line(line_prompt, cmd, &envp, fd);
		free(line_prompt[0]);
		free(line_prompt[1]);
		line_prompt[1] = make_prompt();
		line_prompt[0] = readline(line_prompt[1]);
	}
	free(line_prompt[1]);
	mini_exit(0, true);
}

int	main(int argc, char **argv, char **envp)
{
	if (!argc || !(*argv[0]))
		return (-1);
	minishell(envp);
	return (0);
}
