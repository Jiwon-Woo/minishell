/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jwoo <jwoo@student.42seoul.kr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/10 21:42:54 by jwoo              #+#    #+#             */
/*   Updated: 2021/08/11 11:55:07 by jwoo             ###   ########.fr       */
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

int	is_redirection(int flag)
{
	if (flag <= REDIRECT4 && flag >= REDIRECT1)
		return (1);
	return (0);
}

int	is_output_redirect(int flag)
{
	if (flag == REDIRECT2 || flag == REDIRECT4)
		return (1);
	return (0);
}

int	main(int argc, char **argv, char **first_envp)
{
	t_cmd		*cmd;
	char		**line_prompt;
	t_envp		envp;

	if (!argc || !(*argv[0]))
		return (-1);
	init_envp_status(&envp, first_envp);
	set_signal();
	line_prompt = (char **)malloc(sizeof(char *) * 2);
	cmd = (t_cmd *)malloc(sizeof(t_cmd));
	if (!line_prompt || !cmd)
		exit(1);
	line_prompt[1] = make_prompt();
	line_prompt[0] = readline(line_prompt[1]);
	while (line_prompt[0] != NULL)
	{
		handle_line(line_prompt, cmd, &envp);
		fprintf(stderr, "hihi1\n");
		free(line_prompt[0]);
		free(line_prompt[1]);
		line_prompt[1] = make_prompt();
		line_prompt[0] = readline(line_prompt[1]);
	}
	free(line_prompt[1]);
	mini_exit(0, true);
	return (0);
}
