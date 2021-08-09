#include "minishell.h"

char *parse_path(t_envp *envp)
{
	int flag;
	int idx;

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
	int i;
	last = -1;
	i = -1;
	while (arg[++i])
	{
		if (arg[i] == '/')
			last = i;
	}
	return (last);
}

int get_file_type(char *path)
{
	struct	stat sb;

	if (stat(path, &sb) == -1)
		return (-1);
	else if ((S_IFMT & sb.st_mode) == S_IFDIR)
		return (1);
	else
		return (0);
}

int	file_or_directory(char *arg)
{
	int	first_slash = 0;
	int	last_slash = 0;
	char	**slash;
	char	*path = 0;
	int	i = 0;
	int	type;

	if (arg[0] == '/')
		first_slash = 1;
	if (arg[ft_strlen(arg) - 1] == '/')
		last_slash = 1;
	slash = ft_split(arg, '/');
	if (first_slash == 1)
		path = ft_strjoin_with_free(path, ft_strdup("/"));
	path = ft_strjoin_with_free(path, ft_strdup(slash[0]));
	while ((type = get_file_type(path)) > 0 && slash[++i])
	{
		path = ft_strjoin_with_free(path, ft_strdup("/"));
		path = ft_strjoin_with_free(path, ft_strdup(slash[i]));
	}
	free(path);
	if (type == 0 && slash[i + 1] == 0)
		i++;
	if ((slash[i] != 0 && type == 0) || (slash[i] == 0 && type == 0 && last_slash == 1))
	{
		write(2, "minish: ", ft_strlen("minish: "));
		write(2, arg, ft_strlen(arg));
		write(2, ": Not a directory\n", ft_strlen(": Not a directory\n"));
		free_two_dimension(slash);
		return (126);
		// exit(20);
	}
	else if (type == -1)
	{
		write(2, "minish: ", ft_strlen("minish: "));
		write(2, arg, ft_strlen(arg));
		write(2, ": No such file or directory\n", ft_strlen(": No such file or directory\n"));
		free_two_dimension(slash);
		return (127);
	}
	else if (slash[i] == 0 && type == 1)
	{
		write(2, "minish: ", ft_strlen("minish: "));
		write(2, arg, ft_strlen(arg));
		write(2, ": is a directory\n", ft_strlen(": is a directory\n"));
		free_two_dimension(slash);
		return(126);
	}
	free_two_dimension(slash);
	return (0);
}
