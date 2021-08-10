/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_path.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jwoo <jwoo@student.42seoul.kr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/10 10:37:01 by jwoo              #+#    #+#             */
/*   Updated: 2021/08/10 12:03:42 by jwoo             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*parse_path(t_envp *envp)
{
	int	flag;
	int	idx;

	idx = -1;
	flag = -1;
	while (++idx < get_arg_size(envp->envp_list))
	{
		if ((get_equal_idx(envp->envp_list[idx]) == ft_strlen("PATH") \
			&& ft_strncmp(envp->envp_list[idx], "PATH", 4) == 0)
			|| ft_strncmp("PATH", envp->envp_list[idx], 5) == 0)
			flag = idx;
	}
	if (flag == -1)
		return (0);
	return (ft_strdup(envp->envp_list[flag] + 5));
}

int	get_last_slash_idx(char *arg)
{
	int	last;
	int	i;

	last = -1;
	i = -1;
	while (arg[++i])
	{
		if (arg[i] == '/')
			last = i;
	}
	return (last);
}

int	get_file_type(char *path)
{
	struct stat	sb;

	if (stat(path, &sb) == -1)
		return (-1);
	else if ((S_IFMT & sb.st_mode) == S_IFDIR)
		return (1);
	else
		return (0);
}

int	print_file_dir_err(char *arg, char **slash, int type)
{
	write(2, "minish: ", ft_strlen("minish: "));
	write(2, arg, ft_strlen(arg));
	if (type == 0)
	{
		write(2, ": Not a directory\n", ft_strlen(": Not a directory\n"));
		free_two_dimension(slash);
		return (126);
	}
	else if (type == -1)
	{
		write(2, ": No such file or directory\n", \
			ft_strlen(": No such file or directory\n"));
		free_two_dimension(slash);
		return (127);
	}
	else
	{
		write(2, ": is a directory\n", ft_strlen(": is a directory\n"));
		free_two_dimension(slash);
		return (126);
	}
}

int	get_type(int first_slash, char **slash, int *i)
{
	int		type;
	char	*path;

	path = 0;
	if (first_slash == 1)
		path = ft_strjoin_with_free(path, ft_strdup("/"));
	path = ft_strjoin_with_free(path, ft_strdup(slash[0]));
	type = get_file_type(path);
	while (type > 0 && slash[++(*i)])
	{
		path = ft_strjoin_with_free(path, ft_strdup("/"));
		path = ft_strjoin_with_free(path, ft_strdup(slash[*i]));
		type = get_file_type(path);
	}
	free(path);
	return (type);
}

int	file_or_directory(char *arg)
{
	int		slash_first_last[2];
	char	**slash;
	int		i;
	int		type;

	slash_first_last[0] = 0;
	slash_first_last[1] = 0;
	slash = 0;
	i = 0;
	if (arg[0] == '/')
		slash_first_last[0] = 1;
	if (arg[ft_strlen(arg) - 1] == '/')
		slash_first_last[1] = 1;
	slash = ft_split(arg, '/');
	type = get_type(slash_first_last[0], slash, &i);
	if (type == 0 && slash[i + 1] == 0)
		i++;
	if (!type && (slash[i] || (!slash[i] && slash_first_last[1] == 1)))
		return (print_file_dir_err(arg, slash, type));
	else if (type == -1)
		return (print_file_dir_err(arg, slash, type));
	else if (slash[i] == 0 && type == 1)
		return (print_file_dir_err(arg, slash, type));
	free_two_dimension(slash);
	return (0);
}
