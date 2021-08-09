#include "minishell.h"

int with_path(char **arg_arr, t_envp *envp)
{
	int stat;

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

int exec_file(char *path, char **arg_arr, t_envp *envp)
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

int without_path(char **arg_arr, t_envp *envp)
{
	char	*path = ft_strdup(arg_arr[0]);
	char	*parsepath = parse_path(envp);
	char	**env_path = ft_split(parsepath, ':');
	int 	i = -1;
	struct 	stat sb;
	char *temp;
	
	while (env_path != 0 && arg_arr[0] != 0 && env_path[++i])
	{
		if (path != 0)
			free(path);
		temp = ft_strjoin("/", arg_arr[0]);
		path = ft_strjoin(env_path[i], temp);
		free(temp);
		temp = 0;
		if (stat(path, &sb) == 0 && (sb.st_mode&S_IXUSR))
			break ;
	}
	free_two_dimension(env_path);
	if (parsepath != 0)
	{
		free(parsepath);
		parsepath = 0;
	}
	if (stat(path, &sb) == 0 && (sb.st_mode&S_IXUSR))
		return (exec_file(path, arg_arr, envp));
	else
	{
		if (path != 0)
		{
			free(path);
			path = 0;
		}
		write(2, "minish: ", ft_strlen("minish: "));
		write(2, arg_arr[0], ft_strlen(arg_arr[0]));
		write(2, ": command not found\n", ft_strlen(": command not found\n"));
		return (127);
	}
	return(0);
}

int	interpret(char **arg_arr, t_envp *envp)
{
	int		last_slash;

	if (arg_arr[0] == 0)
		return (0);
	last_slash = get_last_slash_idx(arg_arr[0]);
	if (ft_strncmp(arg_arr[0], "cd", 3) == 0)
		return (mini_cd(arg_arr, envp));
	if (ft_strncmp(arg_arr[0], "export", 7) == 0)
		return (mini_export(arg_arr, envp));
	if (ft_strncmp(arg_arr[0], "unset", 6) == 0)
		return (mini_unset(arg_arr, envp));
	if (ft_strncmp(arg_arr[0], "exit", 5) == 0)
		return (mini_exit(arg_arr, false));
	if (ft_strncmp(arg_arr[0], "pwd", 4) == 0)
		return (mini_pwd(arg_arr, envp));
	if (ft_strncmp(arg_arr[0], "env", 4) == 0)
		return (mini_env(arg_arr, envp));
	if (last_slash != -1)
		return (with_path(arg_arr, envp));
	else if (last_slash == -1)
		return (without_path(arg_arr, envp));
	return (127);
}

int	interpret2(char **arg_arr, t_envp *envp)
{
	int		last_slash;
	pid_t	pid;
	int sig_get;

	if (arg_arr[0] == 0)
		return (0);
	last_slash = get_last_slash_idx(arg_arr[0]);
	if (ft_strncmp(arg_arr[0], "cd", 3) == 0)
		return (mini_cd(arg_arr, envp));
	if (ft_strncmp(arg_arr[0], "export", 7) == 0)
		return (mini_export(arg_arr, envp));
	if (ft_strncmp(arg_arr[0], "unset", 6) == 0)
		return (mini_unset(arg_arr, envp));
	if (ft_strncmp(arg_arr[0], "exit", 5) == 0)
		return (mini_exit(arg_arr, true));
	if (ft_strncmp(arg_arr[0], "pwd", 4) == 0)
		return (mini_pwd(arg_arr, envp));
	if (ft_strncmp(arg_arr[0], "env", 4) == 0)
		return (mini_env(arg_arr, envp));
	pid = fork();
	if (pid < 0)
		exit(1);
	else if (pid == 0)
	{
		signal(SIGQUIT, SIG_DFL);
		if (last_slash != -1)
			exit (with_path(arg_arr, envp));
		else if (last_slash == -1)
			exit (without_path(arg_arr, envp));
	}
	else
	{
		signal(SIGINT, child_sigquit_handler);
		signal(SIGQUIT, child_sigquit_handler);
		wait(&sig_get);
		if (!WIFSIGNALED(sig_get))
			g_status = WEXITSTATUS(sig_get);
		signal(SIGINT, sigint_handler);
		signal(SIGQUIT, SIG_IGN);
		return (g_status);
	}
	return (127);
}