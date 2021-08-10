#include "minishell.h"

int	mini_export(char **arg_arr, t_envp *envp)
{
	int	i;
	int	ret;

	if (export_without_arg(arg_arr))
	{
		print_export(envp);
		return (0);
	}
	ret = 0;
	i = 1;
	while (arg_arr[i])
	{
		if (env_validation(arg_arr[i]) == -1)
		{
			if (ft_strlen(arg_arr[i]) > 0)
			{
				write(2, "minish: export: `", ft_strlen("minish: export: `"));
				write(2, arg_arr[i], ft_strlen(arg_arr[i]));
				write(2, "\': not a valid identifier\n", \
					ft_strlen("\': not a valid identifier\n"));
				ret = 1;
			}
		}
		else if (get_equal_idx(arg_arr[i]) > 0)
			with_eq(arg_arr, envp, i);
		else
			without_eq(arg_arr, envp, i);
		i++;
	}
	sort_envp_idx(envp);
	return (ret);
}

int	mini_pwd(void)
{
	char	*pwd;

	pwd = getcwd(NULL, 0);
	printf("%s\n", pwd);
	free(pwd);
	pwd = 0;
	return (0);
}

int	mini_env(char **arg, t_envp *envp)
{
	int		i;
	int		size;
	char	*error;
	int		equal_idx;

	error = "minish: env: too many arguments\n";
	if (arg[1] != 0)
	{
		write(2, error, ft_strlen(error));
		return (1);
	}
	i = -1;
	size = get_arg_size(envp->envp_list);
	while (++i < size)
	{
		equal_idx = get_equal_idx(envp->envp_list[i]);
		if (equal_idx != -1)
			printf("%s\n", envp->envp_list[i]);
	}
	return (0);
}

int	mini_cd(char **arg, t_envp *envp)
{
	int		ret;
	char	**pwd;
	char	*home;
	char	*buf;

	ret = 0;
	home = get_value("HOME", envp->envp_list);
	if (arg[1] == 0)
		ret = chdir(home);
	else
		ret = chdir(arg[1]);
	if (ret != -1)
	{
		pwd = get_env_ptr("PWD", envp->envp_list);
		buf = getcwd(NULL, 0);
		free(*pwd);
		*pwd = ft_strjoin("PWD=", buf);
		free(buf);
		buf = 0;
		free(home);
		home = 0;
		return (0);
	}
	else if (arg[1] == 0)
		file_or_directory(home);
	else
		file_or_directory(arg[1]);
	free(home);
	home = 0;
	return (1);
}

int	mini_exit(char **arg_arr, bool is_parent)
{
	int	ret;
	int	error;
	char	*error_messeage;

	error = 0;
	if (arg_arr == 0 || arg_arr[1] == 0)
	{
		if (is_parent == true)
			ret = g_status;
		else
			ret = 0;
	}
	else
		error = ft_atoi(arg_arr[1], &ret);
	if (is_parent == true)
	{
		write(2, "exit\n", 5);
	}
	if (error == -1)
	{
		write(2, "minish: exit: ", ft_strlen("minish: exit: "));
		write(2, arg_arr[1], ft_strlen(arg_arr[1]));
		write(2, ": numeric argument required\n", ft_strlen(": numeric argument required\n"));
		ret = 255;
	}
	else if (get_arg_size(arg_arr) > 2)
	{
		error_messeage = "minish: exit: too many arguments\n";
		write(2, error_messeage, ft_strlen(error_messeage));
		ret = 1;
	}
	exit (((int)(char)(ret)));
}

int	get_flag(t_envp *envp, char **arg, int i)
{
	int	flag;
	int	j;

	j = -1;
	flag = -1;
	while (++j < get_arg_size(envp->envp_list))
		if ((get_equal_idx(envp->envp_list[j]) == ft_strlen(arg[i]) \
			&& ft_strncmp(envp->envp_list[j], arg[i], ft_strlen(arg[i])) == 0)
			|| ft_strncmp(arg[i], envp->envp_list[j], ft_strlen(arg[i]) + 1) == 0)
			flag = j;
	return (flag);
}

void unset_var(t_envp *envp, int flag)
{
	char	**temp;
	int		i;
	int	idx;
	
	i = -1;
	idx = 0;
	temp = (char **)malloc(sizeof(char *) * get_arg_size(envp->envp_list));
	if (temp == 0)
		exit(1);
	while (++i < get_arg_size(envp->envp_list))
	{
		if (i != flag)
			temp[idx++] = envp->envp_list[i];
		else
			free(envp->envp_list[i]);
	}
	temp[idx] = 0;
	if (envp->envp_list != 0)
		free(envp->envp_list);
	envp->envp_list = temp;
}

int	print_unset_err(char *arg)
{
	write(2, "minish: unset: `", ft_strlen("minish: unset: `"));
	write(2, arg, ft_strlen(arg));
	write(2, "\': not a valid identifier\n", \
		ft_strlen("\': not a valid identifier\n"));
	return (1);
}

int mini_unset(char **arg, t_envp *envp)
{
	int	size;
	int	i;
	int	flag;
	int ret;

	ret = 0;
	size = get_arg_size(arg);
	if (size == 1)
		return (0);
	i = 0;
	while (++i < size)
	{
		if (get_equal_idx(arg[i]) != -1 || env_validation(arg[i]) == -1)
		{
			if (ft_strlen(arg[i]) > 0)
				ret = print_unset_err(arg[i]);
			continue ;
		}
		flag = get_flag(envp, arg, i);
		if (flag != -1)
			unset_var(envp, flag);
	}
	sort_envp_idx(envp);
	return (ret);
}
