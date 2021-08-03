#include "minishell.h"

// 시그널, status 처리, 에러처리, -> 보너스?? (&& 및 ||, *)
// fprintf 변경 / fork 및 malloc 가드 / 메모리 누수 / norm3
// 부모 자식간 시그널, << 이것도 자식 프로세스에서 돌리기?,
// 부모 : 컨트롤 c 하면 errno 1 /자식 :  cat << 하고 컨트롤 c하면 errno 130, cat 하고 컨트롤 \ 하면 errno 131
// 전역변수 -> last_status


void	sort_envp_idx(t_envp *envp);
int	file_or_directory(char *arg);

int		get_arg_size(char **arg)
{
	int size;

	size = 0;
	if (arg == 0)
		return (size);
	while (arg[size])
		size++;
	return (size);
}

void	child_sigquit_handler(int signo)
{
	if (signo == SIGINT)
		fprintf(stderr, "\n");
	if (signo == SIGQUIT)
		fprintf(stderr, "Quit: 3\n");
	// signal(SIGQUIT, SIG_DFL);
}

void	sigint_handler(int signo)
{
	int	end = ft_strlen(rl_line_buffer);

	rl_on_new_line();
	rl_redisplay();
	printf("  \b\b\n");	//^C 지우기, 개행해주고 새로운 프롬프트 디스플레이 필요
	rl_replace_line("", 0);	// rl_buffer 빈문자열로 초기화 (문자 읽던 도중 ^C 했을 때 필요)
	// printf("\n");
	rl_on_new_line();
	rl_redisplay();
	// printf("\n");

	// cat : redisplay -> on_newline -> \b\b\n
	//		redisplay -> \b\b\n -> on_newline
	//		\b\b\n -> redisplay -> on_newline
}

// void	sigquit_handler(int signo)
// {
// 	exit(0);
// }

void	envp_add(t_envp *envp_, char *content)
{
	int size;
	char **old_envp;
	int i;

	size = get_arg_size(envp_->envp_list);
	old_envp = envp_->envp_list;
	envp_->envp_list = (char **)malloc(sizeof(char *) * (size + 2));
	i = -1;
	while (old_envp[++i])
		envp_->envp_list[i] = old_envp[i];
	free(old_envp);
	envp_->envp_list[i++] = ft_strdup(content);
	envp_->envp_list[i] = 0;
}

void	envp_substitute(t_envp *envp_, char *content, int idx)
{
	free(envp_->envp_list[idx]);
	envp_->envp_list[idx] = ft_strdup(content);
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
			arg_arr[i], key_last) == 0) // 같은거 있기만 하면 교체
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

int		mini_export(char **arg_arr, t_envp *envp)
{
	int	i;
	int ret;
	// int	j;
	// int	key_last;
	// int	input_key_last;

	if (export_without_arg(arg_arr)) // print
	{
		print_export(envp);
		return (0);
	}
	ret = 0;
	i = 1;
	while (arg_arr[i])
	{//에러처리?
		if (env_validation(arg_arr[i]) == -1)
		{
			if (ft_strlen(arg_arr[i]) > 0)
			{
				fprintf(stderr, "bash: export: `%s': not a valid identifier\n", arg_arr[i]);
				ret = 1;
			}
		}
		else if (get_equal_idx(arg_arr[i]) > 0) // = 이 있는 인자가 들어옴
		{
			with_eq(arg_arr, envp, i);
			// input_key_last = get_equal_idx(arg_arr[i]);
			// j = -1;
			// while (envp->envp_list[++j])
			// {
			// 	key_last = get_equal_idx(envp->envp_list[j]);
			// 	if (key_last < 0)
			// 		key_last = ft_strlen(envp->envp_list[j]);
			// 	if (key_last == input_key_last && ft_strncmp(envp->envp_list[j], \
			// 		arg_arr[i], key_last) == 0) // 같은거 있기만 하면 교체
			// 	{
			// 		envp_substitute(envp, arg_arr[i], j);
			// 		break ;
			// 	}
			// }
			// if (envp->envp_list[j] == 0)
			// 	envp_add(envp, arg_arr[i]);
		}
		else // = 없는 인자가 들어옴
		{
			without_eq(arg_arr, envp, i);
			// input_key_last = ft_strlen(arg_arr[i]);
			// j = -1;
			// while (envp->envp_list[++j])
			// {
			// 	key_last = get_equal_idx(envp->envp_list[j]);
			// 	if (key_last < 0)
			// 		key_last = ft_strlen(envp->envp_list[j]);
			// 	if (key_last == input_key_last && \
			// 		ft_strncmp(envp->envp_list[j], arg_arr[i], key_last) == 0)
			// 		break ;
			// }
			// if (envp->envp_list[j] == 0)
			// 	envp_add(envp, arg_arr[i]);
		}
		i++;
	}
	sort_envp_idx(envp);
	return (ret);
}

int mini_pwd(char **arg, t_envp *envp)
{
	char	*pwd;
	int		status;

	pwd = getcwd(NULL, 0);
	printf("%s\n", pwd);
	free(pwd);
	return (0);
}

int mini_env(char **arg, t_envp *envp)
{
	pid_t	pid;
	int		status;

	pid = fork();
	wait(&status);
	if (pid == 0)
		execve("/usr/bin/env", arg, envp->envp_list);
	else
		return (status);
	return (1);
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

int mini_cd(char **arg, t_envp *envp) //envp 추가! 
{
	pid_t	pid;
	int		status;
	int		ret;
	char	**pwd;

	ret = 0;
	if (arg[1] == 0)
		ret = chdir(get_value("HOME", envp->envp_list));
	else
		ret = chdir(arg[1]);
	if (ret != -1)
	{
		pwd = get_env_ptr("PWD", envp->envp_list);
		char *buf = getcwd(NULL, 0);
		free(*pwd);
		*pwd = ft_strjoin("PWD=", buf);
		free(buf);
		return (0);
	}
	else if (arg[1] == 0)
		file_or_directory(get_value("HOME", envp->envp_list));
	else
		file_or_directory(arg[1]);
	return (1);
}

int mini_exit(char **arg_arr, t_envp *envp, bool is_parent)
{
	int ret;
	int error;

	error = 0;
	if (arg_arr == 0 || arg_arr[1] == 0)
	{
		if (is_parent == true)
			ret = envp->last_status;
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
		fprintf(stderr, "bash: exit: %s: numeric argument required\n", arg_arr[1]);
		ret = 255;
	}
	else if (get_arg_size(arg_arr) > 2)
	{
		fprintf(stderr, "bash: exit: too many arguments\n");
		ret = 1;
	}
	exit (((int)(char)(ret)));
}

char *parse_path(t_envp *envp)//path 환경변수 받아오기
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

int mini_ls(char **arg, t_envp *envp)
{
	pid_t	pid;
	int		status;

	pid = fork();
	wait(&status);
	if (pid == 0)
		execve("ls", arg, envp->envp_list);
	else
		return (status);
	return (1);
}

int mini_unset(char **arg, t_envp *envp)
{
	int	size;
	int	i;
	int	j;
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
			{
				fprintf(stderr, "bash: unset: `%s': not a valid identifier\n", arg[i]);
				ret = 1;
			}
			continue ;
		}
		j = -1;
		flag = -1;
		while (++j < get_arg_size(envp->envp_list))
		{
			if ((get_equal_idx(envp->envp_list[j]) == ft_strlen(arg[i]) && ft_strncmp(envp->envp_list[j], arg[i], ft_strlen(arg[i])) == 0)
				|| ft_strncmp(arg[i], envp->envp_list[j], ft_strlen(arg[i]) + 1) == 0)
				flag = j;
		}
		if (flag != -1)
		{
			char **temp;

			temp = (char **)malloc(sizeof(char *) * get_arg_size(envp->envp_list));
			j = -1;
			int	idx = 0;
			while (++j < get_arg_size(envp->envp_list))
			{
				if (j != flag)
					temp[idx++] = envp->envp_list[j];
				else
					free(envp->envp_list[j]);
			}
			temp[idx] = 0;
			if (envp->envp_list != 0)
				free(envp->envp_list);
			envp->envp_list = temp;
		}
	}
	sort_envp_idx(envp);
	return (ret);
}

int mini_status(char **arg_arr, t_envp *envp)
{
	char *status;

	//bash: 127: command not found
	write(2, "bash: ", ft_strlen("bash: "));
	status = ft_itoa(envp->last_status);
	write(2, status, ft_strlen(status));
	free(status);
	write(2, ": command not found\n", ft_strlen(": command not found\n"));
	return (127);
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
		return (1); // dir
	else
		return (0); // file
}

int	file_or_directory(char *arg)
{
	int	first_slash = 0; // Absolute Path
	int	last_slash = 0; // Dir
	char	**slash;
	int	i = 0;
	char	*path = 0;
	int	type;

	if (arg[0] == '/')
		first_slash = 1;
	if (arg[ft_strlen(arg) - 1] == '/')
		last_slash = 1;
	slash = ft_split(arg, '/');
	if (first_slash == 1)
		path = ft_strjoin(path, "/");
	path = ft_strjoin(path, slash[0]);
	while ((type = get_file_type(path)) > 0 && slash[++i]) // 디렉토리 다 스킵
	{
		// printf("%s : %d\n", path, type);
		path = ft_strjoin(path, "/");
		path = ft_strjoin(path, slash[i]);
		// type = get_file_type(path);
	}
	if (type == 0 && slash[i + 1] == 0)
		i++;
	// write(2, strerror(errno), ft_strlen(strerror(errno)));
	// write(2, "\n", 1);
	if ((slash[i] != 0 && type == 0) || (slash[i] == 0 && type == 0 && last_slash == 1))
	{
		fprintf(stderr, "bash: %s: Not a directory\n", arg);
		return (126);
		// exit(20);
	}
	else if (type == -1)
	{
		fprintf(stderr, "bash: %s: No such file or directory\n", arg);
		return (127);
		// exit(2);
	}
	else if (slash[i] == 0 && type == 1)
	{
		fprintf(stderr, "bash: %s: is a directory\n", arg);
		return(126);
		// exit(21);
	}
	return (0);
}

// int with_path(char **arg_arr, t_envp *envp)
// {
// 	int status;
// 	int stat;
// 	pid_t pid;

// 	stat = file_or_directory(arg_arr[0]);
// 	if (stat != 0) // 존재하지 않는 명령?파일?디렉토리면 바로 종료
// 		exit (stat); // 실행 파일이 존재하지 않으면 명령어를 찾을 수 없는 이유 출력 (경로가 있는 명령어)
// 	else
// 	{
// 		pid = fork();
// 		wait(&status);
// 		if (pid == 0) 
// 			if (execve(arg_arr[0], arg_arr, envp->envp_list) == -1)
// 				return (1);
// 	}
// 	return (status);
// }

// int with_path(char **arg_arr, t_envp *envp)
// {
// 	int status;
// 	int stat;
// 	pid_t pid;

// 	stat = file_or_directory(arg_arr[0]);
// 	if (stat != 0) // 존재하지 않는 명령?파일?디렉토리면 바로 종료
// 		exit (stat); // 실행 파일이 존재하지 않으면 명령어를 찾을 수 없는 이유 출력 (경로가 있는 명령어)
// 	else
// 	{
// 		pid = fork();
// 		wait(&status);
// 		if (pid == 0) 
// 			if (execve(arg_arr[0], arg_arr, envp->envp_list) == -1)
// 				exit (1);
// 	}
// 	exit (status);
// }

// int exec_file(char *path, char **arg_arr, t_envp *envp)
// {
// 	pid_t pid;
// 	int status;

// 	pid = fork();
// 	wait(&status);
// 	if (pid == 0)
// 	{
// 		if (execve(path, arg_arr, envp->envp_list) == -1)
// 		{
// 			if (path != 0)
// 			{
// 				free(path);
// 				path = 0;
// 			}
// 			return (1);
// 		}
// 		// char *temp[] = {"pwd", (char*)0};
// 		// if (execve("pwd", temp, envp->envp_list) == -1)
// 		// {
// 		// 	return (1);
// 		// }
// 	}
// 	if (path != 0)
// 	{
// 		free(path);
// 		path = 0;
// 	}
// 	return (status);
// }

int with_path(char **arg_arr, t_envp *envp)
{
	int stat;
	pid_t pid;

	stat = file_or_directory(arg_arr[0]);
	if (stat != 0) // 존재하지 않는 명령?파일?디렉토리면 바로 종료
	{
		return (stat); // 실행 파일이 존재하지 않으면 명령어를 찾을 수 없는 이유 출력 (경로가 있는 명령어)
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
		strerror(errno);////////
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
	if (stat(path, &sb) == 0 && (sb.st_mode&S_IXUSR)) // 실행 파일 존재
		exec_file(path, arg_arr, envp);
	else // 실행 파일이 존재하지 않으면 명령어를 찾을 수 없다고 하고 종료 (경로가 없는 명령어)
	{
		if (path != 0)
		{
			free(path);
			path = 0;
		}
		fprintf(stderr, "bash: %s: command not found\n", arg_arr[0]);
		strerror(errno);
		return (127);
	}
	return(0);
}

int	is_not_builtin(char **arg_arr)
{
	if (ft_strncmp(arg_arr[0], "cd", 3) == 0)
		return (1);
	if (ft_strncmp(arg_arr[0], "export", 7) == 0)
		return (1);
	if (ft_strncmp(arg_arr[0], "unset", 6) == 0)
		return (1);
	if (ft_strncmp(arg_arr[0], "exit", 5) == 0)
		return (1);
	if (ft_strncmp(arg_arr[0], "$?", 3) == 0)
		return (1);
	return (0);
}

// void	none_builtin(char **arg_arr)
// {
// 	if (ft_strncmp(arg_arr[0], "cd", 3) == 0)
// 		return (mini_cd(arg_arr, envp));
// 	if (ft_strncmp(arg_arr[0], "export", 7) == 0)
// 		return (mini_export(arg_arr, envp));
// 	if (ft_strncmp(arg_arr[0], "unset", 6) == 0)
// 		return (mini_unset(arg_arr, envp));
// 	if (ft_strncmp(arg_arr[0], "exit", 5) == 0)
// 		return (mini_exit(arg_arr, envp));
// 	if (ft_strncmp(arg_arr[0], "$?", 3) == 0)
// 		return (mini_status(arg_arr, envp));
// }




// void	check_quote(char *line, t_quote *quote)
// {
// 	int	idx;

// 	idx = -1;
// 	while (++idx < ft_strlen(line))
// 	{
// 		if (line[idx] == '\'' && quote->q_double == 1)
// 		{
// 			quote->q_single *= -1;
// 			quote->q_single_index = idx;
// 		}
// 		else if (line[idx] == '\"' && quote->q_single == 1)
// 		{
// 			quote->q_double *= -1;
// 			quote->q_double_index = idx;
// 		}
// 	}
// 	if (quote->q_single != -1)
// 		quote->q_single_index = -1;
// 	if (quote->q_double != -1)
// 		quote->q_double_index = -1;
// }

int		is_remain_quote(char *line, int idx, char quote)
{
	while (line[++idx])
	{
		if (line[idx] == quote)
			return (idx);
	}
	return (-1);
}

void	check_quote(char *line, t_quote *quote)
{
	int	idx;
	int remain_single_quote;
	int remain_double_quote;

	idx = -1;
	remain_single_quote = -42; // 초기화
	remain_double_quote = -42; // 초기화
	while (++idx < ft_strlen(line))
	{
		if (line[idx] == '\'' && (quote->q_double == 1
			|| (quote->q_double != 1 && remain_double_quote == -1))) // 만약 "가 닫혀있거나, 열려있는데 닫힐일이 없다면 ' 처리
		{
			quote->q_single *= -1;
			quote->q_single_index = idx;
			if (quote->q_single == -1) // 따옴표가 열리면 닫히는 곳이 있는지 탐색, 닫히는 곳 없으면 -1 저장
				remain_single_quote = is_remain_quote(line, idx, '\'');
		}
		else if (line[idx] == '\"' && (quote->q_single == 1
			|| (quote->q_single != 1 && remain_single_quote == -1)))
		{
			quote->q_double *= -1;
			quote->q_double_index = idx;
			if (quote->q_double == -1) // 따옴표가 열리면 닫히는 곳이 있는지 탐색, 닫히는 곳 없으면 -1 저장
				remain_double_quote = is_remain_quote(line, idx, '\"');
		}
	}
	if (quote->q_single == 1) // 질 닫혀있으면 -1로 변경
		quote->q_single_index = -1;
	if (quote->q_double == 1)
		quote->q_double_index = -1;
}

int valid_quote(char **arg, char *line, t_quote *quote, int *i)
{
	if ((line[*i] == '\"' && *i != quote->q_double_index) ||
		(line[*i] == '\'' && *i != quote->q_single_index))
	{
		if (line[*i] == '\'')
		{
			*arg = str_append_char(*arg, line[(*i)++]);
			while (line[*i] != '\'' && line[*i] != 0)
				*arg = str_append_char(*arg, line[(*i)++]);
			if (line[*i] != 0)
				*arg = str_append_char(*arg, line[(*i)++]);
		}
		else
		{
			*arg = str_append_char(*arg, line[(*i)++]);
			while (line[*i] != '\"' && line[*i] != 0)
				*arg = str_append_char(*arg, line[(*i)++]);
			if (line[*i] != 0)
				*arg = str_append_char(*arg, line[(*i)++]);
		}
		return (1);
	}
	return (0);
}

void invalid_quote(char **arg, char *line, t_quote *quote, int *i)
{
	if (line[*i] == '\'' || line[*i] == '\"')
		*arg = str_append_char(*arg, line[(*i)++]);
	while (ft_isspace(line[*i]) == 0 && line[*i] != 0 \
		&& line[*i] != '\'' && line[*i] != '\"')
		*arg = str_append_char(*arg, line[(*i)++]);
}

t_list	*get_arg_list(char *line, t_quote quote)
{
	t_list 	*arg_list;
	char	*arg;
	int		i;
	
	i = 0;
	arg_list = 0;
	while (line[i])
	{
		arg = 0;
		while (ft_isspace(line[i]) == 1 && line[i] != 0)
			i++;
		while (line[i])
		{
			if (valid_quote(&arg, line, &quote, &i) == 0)
				invalid_quote(&arg, line, &quote, &i);
			if (ft_isspace(line[i]) == 1 || line[i] == 0)
				break ;
		}
		if (arg != NULL)
			ft_lstadd_back(&arg_list, ft_lstnew(arg));
	}
	return (arg_list);
}

void init_quote(t_quote *quote)
{
	quote->q_single = 1;
	quote->q_double = 1;
	quote->q_single_index = -1;
	quote->q_double_index = -1;
}

char *make_prompt()
{
	char		*prompt;
	char		buffer[1024];

	getcwd(buffer, 1024);
	prompt = ft_strjoin(buffer, "$ ");
	return (prompt);
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

void	init_envp(t_envp *envp, char **first_envp)
{
	int	i;
	int	j;
	int	size;
	int	temp;

	size = get_arg_size(first_envp);
	envp->envp_list = (char **)malloc(sizeof(char *) * (size + 1));
	envp->sort_idx = 0;
	i = -1;
	while (++i < size)
		envp->envp_list[i] = ft_strdup(first_envp[i]);
	envp->envp_list[size] = 0;
	sort_envp_idx(envp);
}

void set_signal()
{
	signal(SIGINT, (void *)sigint_handler);
	signal(SIGQUIT, SIG_IGN);
}

int	is_redirection(int flag)
{
	if (flag <= REDIRECT4 && flag >= REDIRECT1)
		return (1);
	return (0);
}

int is_output_redirect(int flag)
{
	if (flag == REDIRECT2 || flag == REDIRECT4)
		return (1);
	return (0);
}

int is_input_redirect(int flag)
{
	if (flag == REDIRECT1 || flag == REDIRECT3)
		return (1);
	return (0);
}

char **append_strarr(char **str1, char **str2)
{
	char 	**ret_strarr;
	int 	idx;
	int		i;

	
	ret_strarr = (char **)malloc(sizeof(char *) * (get_arg_size(str1) + get_arg_size(str2) + 1));
	i = 0;
	idx = 0;
	while(str1 && str1[i])
		ret_strarr[idx++] = ft_strdup(str1[i++]);
	i = 0;
	while(str2 && str2[i])
		ret_strarr[idx++] = ft_strdup(str2[i++]);
	ret_strarr[idx] = 0;
	return(ret_strarr);
}

int	interpret(char **arg_arr, t_envp *envp, int *fd) // envp인자 구조체로 바꾸기 & 절대경로로 실행할 수 있는 명령어 : echo ls env pwd
{
	int		status;
	t_envp  envp_;
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
		return (mini_exit(arg_arr, envp, false));
	if (ft_strncmp(arg_arr[0], "pwd", 4) == 0)
		return (mini_pwd(arg_arr, envp));
	if (ft_strncmp(arg_arr[0], "$?", 3) == 0)
		return (mini_status(arg_arr, envp));
	if (last_slash != -1) // 명령어에 절대경로가 주어졌을 때
		return (with_path(arg_arr, envp));
	else if (last_slash == -1) // 인자가 경로로 주어지지 않을 때
		return (without_path(arg_arr, envp));
	return (127);
}

void	sigquit_handler(int signo)
{
	if (signo == SIGINT)
	{
		signal(SIGINT, sigquit_handler);
		exit (130);
	}
	if (signo == SIGQUIT)
	{
		signal(SIGQUIT, sigquit_handler);
		exit (131);
	}
}

int	interpret2(char **arg_arr, t_envp *envp) // envp인자 구조체로 바꾸기 & 절대경로로 실행할 수 있는 명령어 : echo ls env pwd
{
	int		status;
	t_envp  envp_;
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
		return (mini_exit(arg_arr, envp, true));
	if (ft_strncmp(arg_arr[0], "pwd", 4) == 0)
		return (mini_pwd(arg_arr, envp));
	// if (ft_strncmp(arg_arr[0], "$?", 3) == 0)
	// return (mini_status(arg_arr, envp));
	// signal(SIGINT, child_sigquit_handler);
	// signal(SIGQUIT, child_sigquit_handler);
	int p = 0;
	if (fork() == 0)
	{
		signal(SIGINT, SIG_IGN);
		signal(SIGQUIT, SIG_IGN);
		if (last_slash != -1) // 명령어에 절대경로가 주어졌을 때
			exit (with_path(arg_arr, envp));
		else if (last_slash == -1) // 인자가 경로로 주어지지 않을 때
			exit (without_path(arg_arr, envp));
	}
	else
	{
		// signal(SIGINT, //만약 SIG_INT 받으면 -> 자식 프로세스 KILL하고 , QUIT 출력 , last_status 130으로 업데이트 
		//만약 SIG_QUIT 받으면 -> 자식 프로세스 KILL하고 , QUIT 출력 , last_status 131로 업데이트
		signal(SIGINT, child_sigquit_handler);
		signal(SIGQUIT, child_sigquit_handler);
		wait(&(envp->last_status));
		signal(SIGINT, sigint_handler);
		signal(SIGQUIT, SIG_IGN);
		envp->last_status = WEXITSTATUS(envp->last_status);
		return (envp->last_status);
	}
	return (127);
}

void handle_line(char **line_prompt, t_list **arg_cmd_tmp, t_quote *quote, t_envp *envp)
{
	char **arg_arr;

	set_signal();
	add_history(line_prompt[0]);
	check_quote(line_prompt[0], quote);
	arg_cmd_tmp[0] = get_arg_list(line_prompt[0], *quote);//t_list **arg_cmd_tmp
	arg_cmd_tmp[1] = list_to_char_arr(arg_cmd_tmp[0], envp);
	arg_cmd_tmp[2] = arg_cmd_tmp[1];

	int	size = ft_lstsize(arg_cmd_tmp[1]);
	int fds[size][2];
	int idx = -1;
	// int status;

	// if (size == 1 && ft_strncmp(((char **)(arg_cmd_tmp[1]->content))[0], "exit", 5) == 0)
	// {
	// 	envp->last_status =  mini_exit((char **)(arg_cmd_tmp[1]->content), envp);
	// 	return;
	// }
	while (arg_cmd_tmp[1])
	{
		arg_arr = (char **)arg_cmd_tmp[1]->content;
		// fprintf(stderr, "arg_arr : %s, pre_flag = %d next_flag = %d\n", arg_arr[0], arg_cmd_tmp[1]->pre_flag, arg_cmd_tmp[1]->next_flag);
		pipe(fds[++idx]);
		/*
		# define REDIRECT1 (2) // <
		# define REDIRECT2 (3) // >
		# define REDIRECT3 (4) // <<
		# define REDIRECT4 (5) // >>
		*/
		int fd[2];

		fd[0] = -1;
		fd[1] = -1;

		t_list	*current_cmd = arg_cmd_tmp[1];
		char	**copy_cmd = append_strarr((char **)(current_cmd->content), 0);
		int		current_fds = idx;
		char	*error_file = 0;

		if (is_redirection(current_cmd->pre_flag) == 0 &&
			is_redirection(arg_cmd_tmp[1]->next_flag) == 1)
		{
			while (is_redirection(arg_cmd_tmp[1]->next_flag) == 1 && arg_cmd_tmp[1]->next != 0)
			{
				arg_cmd_tmp[1] = arg_cmd_tmp[1]->next;
				pipe(fds[++idx]);
				char **redirect_cmd = arg_cmd_tmp[1]->content;
				if (is_output_redirect(arg_cmd_tmp[1]->pre_flag) == 1)
				{
					if (arg_cmd_tmp[1]->pre_flag == REDIRECT2)
						fd[1] = open(redirect_cmd[0], O_WRONLY | O_TRUNC | O_CREAT, 0644);
					if (arg_cmd_tmp[1]->pre_flag == REDIRECT4)
						fd[1] = open(redirect_cmd[0], O_WRONLY | O_APPEND | O_CREAT, 0644);
					if (get_arg_size(redirect_cmd) > 1)
					{
						copy_cmd = append_strarr(copy_cmd, redirect_cmd + 1);
						// fprintf(stderr, "hihihih: %s %s\n", *(copy_cmd), *(copy_cmd + 1));
					}
				}
				if (arg_cmd_tmp[1]->pre_flag == REDIRECT1)
				{
					fd[0] = open(redirect_cmd[0], O_RDONLY);
					// fprintf(stderr, "fd[0] : %d\n", fd[0]);
					if (get_arg_size(redirect_cmd) > 1)
					{
						copy_cmd = append_strarr(copy_cmd, redirect_cmd + 1);
					}
					if (fd[0] == -1)
					{
						// fprintf(stderr, "redirect_cmd[0] : %s\n", redirect_cmd[0]);
						// fprintf(stderr, "%s: %s: No such file or directory\n", arg_arr[0], redirect_cmd[0]);
						if (error_file == 0)
							error_file = ft_strdup(redirect_cmd[0]);
						continue ;
					}
				}
				if (arg_cmd_tmp[1]->pre_flag == REDIRECT3)
				{
					if (get_arg_size(redirect_cmd) > 1)
					{
						copy_cmd = append_strarr(copy_cmd, redirect_cmd + 1);
					}
					// char *store = ft_strdup(rl_line_buffer);
					char *line = readline("> ");
					while (line != NULL && ft_strncmp(line, redirect_cmd[0], ft_strlen(redirect_cmd[0]) + 1) != 0)
					{
						write(fds[idx - 1][1], line, ft_strlen(line));
						write(fds[idx - 1][1], "\n", 1);
						line = readline("> ");
					}
					// rl_line_buffer = store;
					close(fds[idx - 1][1]);
					fd[0] = fds[idx - 1][0];
					// close(fds[idx - 1][0]);
				}
			}
			if (is_redirection(arg_cmd_tmp[1]->next_flag) == 1 && arg_cmd_tmp[1]->next == 0)
			{
				fprintf(stderr, "bash: syntax error near unexpected token `newline'\n");
				envp->last_status = 258;
				return;
			}
			if (error_file != 0)
			{
				fprintf(stderr, "bash: %s: No such file or directory\n", error_file);
				envp->last_status = 1;
				return;
			}
		}
		if (current_cmd->pre_flag == PIPE || (arg_cmd_tmp[1]->next_flag == PIPE))
		{
			if (current_cmd->pre_flag == PIPE && fd[0] == -1)
				fd[0] = fds[current_fds - 1][0];
			if (arg_cmd_tmp[1]->next_flag == PIPE && ((char **)(current_cmd->content))[0] == 0)
			{
				fprintf(stderr, "bash: syntax error near unexpected token `|'\n");
				envp->last_status = 258;
				return;
			}
			else if (arg_cmd_tmp[1]->next_flag == PIPE && fd[1] == -1)
				fd[1] = fds[idx][1];
			if (fork() == 0)
			{
				if (fd[0] != -1)
					dup2(fd[0], 0);
				if (fd[1] != -1)
					dup2(fd[1], 1);
				exit (interpret(copy_cmd, envp, fd));
			}
			else
			{
				wait(&(envp->last_status));
				envp->last_status = WEXITSTATUS(envp->last_status);
				close(fd[0]);
				close(fd[1]);
			}
		}
		else
		{
			int backup[2];
			pipe(backup);
			dup2(0, backup[0]);
			dup2(1, backup[1]);
			if (fd[0] != -1)
				dup2(fd[0], 0);
			if (fd[1] != -1)
				dup2(fd[1], 1);
			envp->last_status = interpret2(copy_cmd, envp);
			dup2(backup[0], 0);
			dup2(backup[1], 1);
			close(backup[0]);
			close(backup[1]);
			close(fd[0]);
			close(fd[1]);
		}
		// else if (ft_strncmp(((char **)(current_cmd->content))[0], "exit", 5) == 0)
		// {
		// 	envp->last_status =  mini_exit((char **)(arg_cmd_tmp[1]->content), envp);
		// 	return;
		// }s
		arg_cmd_tmp[1] = arg_cmd_tmp[1]->next;
	}
	arg_cmd_tmp[1] = arg_cmd_tmp[2];
	// free(line_prompt[0]);
	// free(line_prompt[1]);
	// line_prompt[1] = make_prompt();
	// line_prompt[0] = readline(line_prompt[1]);
	ft_lstclear(&arg_cmd_tmp[0], free);
	ft_lstclear_two(&arg_cmd_tmp[1], free_two_dimension);
}

int	main(int argc, char **argv, char **first_envp)
{
	t_list		**arg_cmd_tmp;
	char		**line_prompt;
	t_quote		quote;
	t_envp		envp;
	// int fds[2];
	// pipe(fds);

	init_quote(&quote);
	init_envp(&envp, first_envp);
	set_signal();
	line_prompt = (char **)malloc(sizeof(char *) * 2);
	arg_cmd_tmp = (t_list **)malloc(sizeof(t_list *) * 3);
	line_prompt[1] = make_prompt();
	line_prompt[0] = readline(line_prompt[1]);
	while (line_prompt[0] != NULL)
	{
		handle_line(line_prompt, arg_cmd_tmp, &quote, &envp);
		free(line_prompt[0]);
		free(line_prompt[1]);
		line_prompt[1] = make_prompt();
		line_prompt[0] = readline(line_prompt[1]);
		// ft_lstclear(&arg_cmd_tmp[0], free);
		// ft_lstclear_two(&arg_cmd_tmp[1], free_two_dimension);
	}
	free(line_prompt[1]);
	mini_exit(0, &envp, true);
	return (0);
}
