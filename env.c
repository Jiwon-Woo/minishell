#include "minishell.h"

int	get_equal_idx(char *env_line)
{
	int i;

	i = -1;
	while(env_line[++i])
	{
		if (env_line[i] == '=')
			return (i);
	}
	return (-1);
}

char *get_value(char *key, char **envp)
{
	int i;
	int equal_idx;

	i = -1;
	while (envp[++i])
	{
		equal_idx = get_equal_idx(envp[i]);
		if (ft_strlen(key) == equal_idx && \
			ft_strncmp(envp[i], key, equal_idx) == 0)
			return (ft_strdup(envp[i] + (ft_strlen(key) + 1)));
	}
	return (ft_strdup(""));
}

void	envp_add(t_envp *envp, char *content)
{
	int size;
	char **old_envp;
	int i;

	size = get_arg_size(envp->envp_list);
	old_envp = envp->envp_list;
	envp->envp_list = (char **)malloc(sizeof(char *) * (size + 2));
	if (!envp->envp_list)
		exit(1);
	i = -1;
	while (old_envp[++i])
		envp->envp_list[i] = old_envp[i];
	free(old_envp);
	envp->envp_list[i++] = ft_strdup(content);
	envp->envp_list[i] = 0;
}

void	envp_substitute(t_envp *envp, char *content, int idx)
{
	free(envp->envp_list[idx]);
	envp->envp_list[idx] = ft_strdup(content);
}

void	print_export(t_envp *envp)
{
	int	i;
	int size;

	i = -1;
	size = get_arg_size(envp->envp_list);
	while (++i < size)
	{
		int equal_idx = get_equal_idx(envp->envp_list[envp->sort_idx[i]]);
		if (equal_idx == -1)
			equal_idx = ft_strlen(envp->envp_list[envp->sort_idx[i]]);
		write(1, "declare -x ", ft_strlen("declare -x "));
		write(1, envp->envp_list[envp->sort_idx[i]], equal_idx + 1);
		if (equal_idx != ft_strlen(envp->envp_list[envp->sort_idx[i]]))
			write(1, "\"", 1);
		write(1, envp->envp_list[envp->sort_idx[i]] + equal_idx + 1, \
			ft_strlen(envp->envp_list[envp->sort_idx[i]] + equal_idx + 1));
		if (equal_idx != ft_strlen(envp->envp_list[envp->sort_idx[i]]))
			write(1, "\"", 1);
		write(1, "\n", 1);
	}
}

void with_eq(char **arg_arr, t_envp *envp, int i)
{
	int j;
	int key_last;
	int input_key_last;

	input_key_last = get_equal_idx(arg_arr[i]);
	j = -1;
	while (envp->envp_list[++j])
	{
		key_last = get_equal_idx(envp->envp_list[j]);
		if (key_last < 0)
			key_last = ft_strlen(envp->envp_list[j]);
		if (key_last == input_key_last && ft_strncmp(envp->envp_list[j], \
			arg_arr[i], key_last) == 0)
		{
			envp_substitute(envp, arg_arr[i], j);
			break ;
		}
	}
	if (envp->envp_list[j] == 0)
		envp_add(envp, arg_arr[i]);
}

void	without_eq(char **arg_arr, t_envp *envp, int i)
{
	int j;
	int key_last;
	int input_key_last;

	input_key_last = ft_strlen(arg_arr[i]);
	j = -1;
	while (envp->envp_list[++j])
	{
		key_last = get_equal_idx(envp->envp_list[j]);
		if (key_last < 0)
			key_last = ft_strlen(envp->envp_list[j]);
		if (key_last == input_key_last && \
			ft_strncmp(envp->envp_list[j], arg_arr[i], key_last) == 0)
			break ;
	}
	if (envp->envp_list[j] == 0)
		envp_add(envp, arg_arr[i]);
}

int	env_validation(char *name)
{
	int i;

	i = 0;
	if (!name)
		return (-1);
	if (!((name[i] == '_') || ft_isalpha(name[i])))
		return (-1);
	i++;
	while(name[i] && name[i] != '=')
	{
		if (!(ft_isdigit(name[i]) || ft_isalpha(name[i]) || name[i] == '_'))
			return (-1);
		i++;
	}
	return (0);
}

int	export_without_arg(char **arg_arr)
{
	int size;
	int i;

	size = get_arg_size(arg_arr);
	i = 0;
	while (++i < size)
	{
		if (ft_strlen(arg_arr[i]) > 0)
			return (0);
	}
	return (1);
}

void	arr_swap(int *arr, int i, int j)
{
	int	temp;

	temp = arr[i];
	arr[i] = arr[j];
	arr[j] = temp;
}

void	sort_envp_idx(t_envp *envp)
{
	int	i;
	int j;
	int	size;
	int	*sort;

	size = get_arg_size(envp->envp_list);
	sort = (int *)malloc(sizeof(int) * size);
	if (!sort)
		exit(1);
	i = -1;
	while (++i < size)
		sort[i] = i;
	i = -1;
	while (++i < size)
	{
		j = i;
		while (++j < size)
			if (ft_strncmp(envp->envp_list[sort[i]], envp->envp_list[sort[j]],
					ft_strlen(envp->envp_list[sort[i]])) > 0)
				arr_swap(sort, i, j);
	}
	if (envp->sort_idx != 0)
		free(envp->sort_idx);
	envp->sort_idx = sort;
}

char **get_env_ptr(char *key, char **envp)
{
	int i;
	int equal_idx;

	i = -1;

	while (envp[++i])
	{
		equal_idx = get_equal_idx(envp[i]);
		if (ft_strlen(key) == equal_idx && ft_strncmp(envp[i], key, equal_idx) == 0)
			return (&envp[i]);
	}
	return (0);
}

void	init_envp(t_envp *envp, char **first_envp)
{
	int	i;
	int	size;

	size = get_arg_size(first_envp);
	envp->envp_list = (char **)malloc(sizeof(char *) * (size + 1));
	if (!envp->envp_list)
		exit(1);
	envp->sort_idx = 0;
	i = -1;
	while (++i < size)
		envp->envp_list[i] = ft_strdup(first_envp[i]);
	envp->envp_list[size] = 0;
	sort_envp_idx(envp);
}