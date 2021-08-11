/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_exec.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jwoo <jwoo@student.42seoul.kr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/11 09:27:25 by jwoo              #+#    #+#             */
/*   Updated: 2021/08/11 10:00:57 by jwoo             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	with_path(char **arg_arr, t_envp *envp)
{
	int	stat;

	stat = file_or_directory(arg_arr[0]);
	if (stat != 0)
	{
		return (stat);
	}
	else
	{
		if (execve(arg_arr[0], arg_arr, envp->envp_list) == -1)
		{
			exit (1);
		}
	}
	return (0);
}

int	exec_file(char *path, char **arg_arr, t_envp *envp)
{
	if (execve(path, arg_arr, envp->envp_list) == -1)
	{
		if (path != 0)
		{
			free(path);
			path = 0;
		}
		return (1);
	}
	if (path != 0)
	{
		free(path);
		path = 0;
	}
	return (0);
}

char	*get_path(char **env_path, char *arg)
{
	char		*path;
	char		*temp;
	int			i;
	struct stat	sb;

	path = ft_strdup(arg);
	i = -1;
	while (env_path != 0 && arg != 0 && env_path[++i])
	{
		if (path != 0)
			free(path);
		temp = ft_strjoin("/", arg);
		path = ft_strjoin(env_path[i], temp);
		if (temp != 0)
			free(temp);
		temp = 0;
		if (stat(path, &sb) == 0 && (sb.st_mode & S_IXUSR))
			break ;
	}
	free_two_dimension(env_path);
	return (path);
}

int	without_path(char **arg_arr, t_envp *envp)
{
	char		*path;
	char		*parsepath;
	char		**env_path;
	struct stat	sb;

	parsepath = parse_path(envp);
	env_path = ft_split(parsepath, ':');
	path = get_path(env_path, arg_arr[0]);
	if (parsepath != 0)
		free(parsepath);
	if (stat(path, &sb) == 0 && (sb.st_mode & S_IXUSR))
		return (exec_file(path, arg_arr, envp));
	else
	{
		if (path != 0)
			free(path);
		write(2, "minish: ", ft_strlen("minish: "));
		write(2, arg_arr[0], ft_strlen(arg_arr[0]));
		write(2, ": command not found\n", ft_strlen(": command not found\n"));
		return (127);
	}
	return (0);
}
