/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_file_type.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jwoo <jwoo@student.42seoul.kr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/10 10:37:01 by jwoo              #+#    #+#             */
/*   Updated: 2021/08/10 20:51:33 by jwoo             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
	if (path != 0)
		free(path);
	path = 0;
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
