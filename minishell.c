#include "minishell.h"

// 파이프 및 리다이렉션, $? 처리, 보너스?? (&& 및 ||, *), string not in pwd ?, arg 갯수 - 에러?,

void	sort_envp_idx(t_envp *envp);
int	file_or_directory(char *arg);

int		get_arg_size(char **arg)
{
	int size;

	size = 0;
	while (arg[size])
		size++;
	return (size);
}

void	sigint_handler(int signo)
{
	int	end = ft_strlen(rl_line_buffer);
	char temp[end + 2];

	for (int i = 0; i < end; i++)
		temp[i] = rl_line_buffer[i];
	temp[end] = ' ';
	temp[end + 1] = ' ';
	temp[end + 2] = 0;
	// rl_replace_line(temp, 0);
	rl_on_new_line();
	rl_redisplay();
	printf("\n");
	// rl_replace_line("", 0);
	rl_on_new_line();
	rl_redisplay();
}

void	sigquit_handler(int signo)
{
	exit(0);
}

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
		write(1, envp->envp_list[envp->sort_idx[i]] + equal_idx + 1, ft_strlen(envp->envp_list[envp->sort_idx[i]] + equal_idx + 1));
		if (equal_idx != ft_strlen(envp->envp_list[envp->sort_idx[i]]))
			write(1, "\"", 1);
		write(1, "\n", 1);
	}
}

int		mini_export(char **arg_arr, t_envp *envp)
{
	int	i;
	int	j;
	int	key_last;
	int	input_key_last;

	i = 1;
	if (get_arg_size(arg_arr) == 1) // print
	{
		print_export(envp);
		return (0);
	}
	while (arg_arr[i])
	{
		if (get_equal_idx(arg_arr[i]) > 0) // = 이 있는 인자가 들어옴
		{
			input_key_last = get_equal_idx(arg_arr[i]);
			j = -1;
			while (envp->envp_list[++j])
			{
				key_last = get_equal_idx(envp->envp_list[j]);
				if (key_last < 0)
					key_last = ft_strlen(envp->envp_list[j]);
				if (key_last == input_key_last && ft_strncmp(envp->envp_list[j], arg_arr[i], key_last) == 0) // 같은거 있기만 하면 교체
				{
					envp_substitute(envp, arg_arr[i], j);
					break ;
				}
			}
			if (envp->envp_list[j] == 0)
			{
				envp_add(envp, arg_arr[i]);
			}
		}
		else // = 없는 인자가 들어옴
		{
			input_key_last = ft_strlen(arg_arr[i]);
			j = -1;
			while (envp->envp_list[++j])
			{
				key_last = get_equal_idx(envp->envp_list[j]);
				if (key_last < 0)
					key_last = ft_strlen(envp->envp_list[j]);
				if (key_last == input_key_last && ft_strncmp(envp->envp_list[j], arg_arr[i], key_last) == 0)
				{
					// 밑 112번째 줄 조건에 안들어가도록..
					break ;
				}
			}
			if (envp->envp_list[j] == 0)
			{
				envp_add(envp, arg_arr[i]);
			}
		}
		i++;
	}
	sort_envp_idx(envp);
	return (0);
}

int mini_pwd(char **arg, t_envp *envp)
{
	pid_t	pid;
	char	*arg_temp[] = {"pwd", (char *)0};
	int		status;

	pid = fork();
	wait(&status);
	if (pid == 0)
	{
		execve("/bin/pwd", arg_temp, envp->envp_list);
	}
	else
	{
		return (status);
	}
	return (1);
}

int mini_env(char **arg, t_envp *envp)
{
	pid_t	pid;
	int		status;

	pid = fork();
	wait(&status);
	if (pid == 0)
	{
		execve("/usr/bin/env", arg, envp->envp_list);
	}
	else
	{
		return (status);
	}
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

	if (chdir(arg[1]) != -1)
	{
		char **pwd = get_env_ptr("PWD", envp->envp_list);
		char *buf = getcwd(NULL, 0);
		*pwd = ft_strjoin("PWD=", buf);
		free(buf);
		return (1);
	}
	return (-1);
}

int mini_exit()
{
	printf("exit\n");
	exit (0);
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

int mini_echo(char **arg, t_envp *envp, int last_slash)
{
	pid_t	pid;
	int		status;
	char	*path = ft_strdup(arg[0]);
	struct 	stat sb;
	int		i;
	char 	**path_ = 0;

	if (last_slash == -1) // 인자가 경로로 주어지지 않을 때
	{
		path_ = ft_split(parse_path(envp), ':');
		i = -1;
		while(path_ != 0 && path != 0 && path_[++i])
		{
			if (path != 0)
				free(path);
			path = ft_strjoin(path_[i], "/echo");
			if (stat(path, &sb) == 0)
				break ;
		}
		if (path_ == 0 || path_[i] == 0)
		{
			if (path != 0)
				free(path);
			path = 0;
		}
	}
	if (path == 0 || stat(path, &sb) == -1)
		return (file_or_directory(path));
	pid = fork();
	wait(&status);
	if (pid == 0) 
	{
		if (execve(path, arg, envp->envp_list) == -1)
			return (1);
	}
	else
	{
		if(WIFEXITED(status))
			printf("Normal exit: exit(%d)\n", WEXITSTATUS(status));
		else if(WIFSIGNALED(status))
			printf("Incorrect exit: signo(%d) %s\n", WTERMSIG(status), WCOREDUMP(status)?"(core dumped)":"");
		free(path);
		printf("%s\n", strerror(errno));
		printf("status____ : %d\n", status >> 8);
		return (status);
	}
	return (1);
}

int mini_ls(char **arg, t_envp *envp)
{
	pid_t	pid;
	int		status;

	pid = fork();
	wait(&status);
	if (pid == 0)
	{
		execve("/bin/ls", arg, envp->envp_list);
	}
	else
	{
		return (status);
	}
	return (1);
}

int mini_unset(char **arg, t_envp *envp)
{
	int	size;
	int	i;
	int	j;
	int	flag;

	size = get_arg_size(arg);
	if (size == 1)
		return (1);
	i = 0;
	while (++i < size)
	{
		if (get_equal_idx(arg[i]) != -1)
		{
			printf("bash: unset: `%s': not a valid identifier\n", arg[i]);
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
				if (j != flag)
					temp[idx++] = envp->envp_list[j];
			temp[idx] = 0;
			if (envp->envp_list != 0)
				free(envp->envp_list);
			envp->envp_list = temp;
		}
	}
	sort_envp_idx(envp);
	return (0);
}

int mini_status(char **arg_arr, t_envp *envp)
{
	char *status;

	//bash: 127: command not found
	write(1, "bash: ", ft_strlen("bash: "));
	status = ft_itoa(envp->last_status);
	write(1, status, ft_strlen(status));
	free(status);
	write(1, ": command not found\n", ft_strlen(": command not found\n"));
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
	// path = ft_strjoin(path, slash[i]);
	// printf("%d\n", stat(path, &sb));
	if (stat(path, &sb) == -1)
		return (-1);
	else if ((S_IFMT & sb.st_mode) == S_IFDIR)
		return (1); // dir
	else
		return (0); // file
	// path = ft_strjoin(path, "/");
	// printf("%d\n", stat(path, &sb));
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
	if ((slash[i] != 0 && type == 0) || (slash[i] == 0 && type == 0 && last_slash == 1))
	{
		printf("bash: %s: Not a directory\n", arg);
		return (126);
	}
	else if (type == -1)
	{
		printf("bash: %s: No such file or directory\n", arg);
		return (127);
	}
	else if (slash[i] == 0 && type == 1)
	{
		printf("bash: %s: is a directory\n", arg);
		return(126);
	}
	return (0);
}

int interpret(char **arg_arr, t_envp *envp) // envp인자 구조체로 바꾸기 & 절대경로로 실행할 수 있는 명령어 : echo ls env pwd
{
	int		status;
	int		ret_value;
	t_envp  envp_;
	int		last_slash;

	last_slash = 0;
	if (arg_arr[0] == 0)
		return (0);
	last_slash = get_last_slash_idx(arg_arr[0]);
	if (last_slash != -1) // 명령어에 절대경로가 주어졌을 때
	{
		int stat = file_or_directory(arg_arr[0]);
		if (stat != 0) // 존재하지 않는 명령?파일?디렉토리면 바로 종료
			return (stat); // 실행 파일이 존재하지 않으면 명령어를 찾을 수 없는 이유 출력 (경로가 있는 명령어)
		else
		{
			// 실행시키고 리턴
			pid_t pid = fork();
			wait(&status);
			if (pid == 0) 
			{
				if (execve(arg_arr[0], arg_arr, envp->envp_list) == -1)
					return (1);
			}
			else
				return (status);
		}
	}
	/*
	arg_arr[0] 을 path 환경변수 파싱한 후에 뒤에 붙여서 stat 함수로 존재여부 & 실행 가능 여부 확인해보기(루프 돌면서,,)
	-> 존재하고 실행 가능하면 mini_echo등 작성한 함수 실행하기
	-> 다 돌아도 없으면 strncmp함수로 나머지 명령어들(export, unset, exit, $?, cd)과 비교 -> 실행
	-> cmp도 안되면 없는 커맨드
	*/
	if (ft_strncmp(arg_arr[0] + last_slash + 1, "cd", 3) == 0)
		return (mini_cd(arg_arr, envp));
	if (ft_strncmp(arg_arr[0], "export", 7) == 0)
		return (mini_export(arg_arr, envp));
	if (ft_strncmp(arg_arr[0], "unset", 6) == 0)
		return (mini_unset(arg_arr, envp));
	if (ft_strncmp(arg_arr[0], "exit", 5) == 0)
		return (mini_exit());
	if (ft_strncmp(arg_arr[0], "$?", 3) == 0)
		return (mini_status(arg_arr, envp));
	if (last_slash == -1) // 인자가 경로로 주어지지 않을 때
	{
		char	*path = ft_strdup(arg_arr[0]);
		char	**path_ = ft_split(parse_path(envp), ':');
		int i = -1;
		struct 	stat sb;
		int	flag = 0;

		while (path_ != 0 && path != 0 && path_[++i])
		{
			if (path != 0)
				free(path);
			path = ft_strjoin(path_[i], ft_strjoin("/", arg_arr[0]));
			if (stat(path, &sb) == 0 && (sb.st_mode&S_IXUSR))
			{
				flag = 1;
				break ;
			}
		}
		if (flag == 1) // 실행 파일 존재
		{
			pid_t pid = fork();
			wait(&status);
			if (pid == 0)
				if (execve(path, arg_arr, envp->envp_list) == -1)
					return (1);
			else 
				return (status);
		}
		else // 실행 파일이 존재하지 않으면 명령어를 찾을 수 없다고 하고 종료 (경로가 없는 명령어)
		{
			printf("bash: %s: command not found\n", arg_arr[0]);
			return (127);
		}
	}
	return (127);
}

void	check_quote(char *line, t_quote *quote)
{
	int	idx;

	idx = -1;
	while (++idx < ft_strlen(line))
	{
		if (line[idx] == '\'' && quote->q_double == 1)
		{
			quote->q_single *= -1;
			quote->q_single_index = idx;
		}
		else if (line[idx] == '\"' && quote->q_single == 1)
		{
			quote->q_double *= -1;
			quote->q_double_index = idx;
		}
	}
	if (quote->q_single != -1)
		quote->q_single_index = -1;
	if (quote->q_double != -1)
		quote->q_double_index = -1;
}

int valid_quote(char *arg, char *line, t_quote *quote, int *i)
{
	if ((line[*i] == '\"' && *i != quote->q_double_index) ||
		(line[*i] == '\'' && *i != quote->q_single_index))
	{
		if (line[*i] == '\'')
		{
			arg = str_append_char(arg, line[(*i)++]);
			while (line[*i] != '\'' && line[*i] != 0)
				arg = str_append_char(arg, line[(*i)++]);
			if (line[*i] != 0)
				arg = str_append_char(arg, line[(*i)++]);
		}
		else
		{
			arg = str_append_char(arg, line[(*i)++]);
			while (line[*i] != '\"' && line[*i] != 0)
				arg = str_append_char(arg, line[(*i)++]);
			if (line[*i] != 0)
				arg = str_append_char(arg, line[(*i)++]);
		}
		return (1);
	}
	return (0);
}
void invalid_quote(char *arg, char *line, t_quote *quote, int *i)
{
	if (line[*i] == '\'' || line[*i] == '\"')
		arg = str_append_char(arg, line[(*i)++]);
	while (ft_isspace(line[*i]) == 0 && line[*i] != 0 \
		&& line[*i] != '\'' && line[*i] != '\"')
		arg = str_append_char(arg, line[(*i)++]);
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
			//////여기에if(조건)
			if ((line[i] == '\"' && i != quote.q_double_index) ||
				(line[i] == '\'' && i != quote.q_single_index))
			{
				if (line[i] == '\'')
				{
					arg = str_append_char(arg, line[i++]);
					while (line[i] != '\'' && line[i] != 0)
						arg = str_append_char(arg, line[i++]);
					if (line[i] != 0)
						arg = str_append_char(arg, line[i++]);
				}
				else
				{
					arg = str_append_char(arg, line[i++]);
					while (line[i] != '\"' && line[i] != 0)
						arg = str_append_char(arg, line[i++]);
					if (line[i] != 0)
						arg = str_append_char(arg, line[i++]);
				}
			}
			// if (valid_quote(arg, line, &quote, &i) == 0)
			else
			{
				// invalid_quote(arg, line, &quote, &i);
				if (line[i] == '\'' || line[i] == '\"')
					arg = str_append_char(arg, line[i++]);
				while (ft_isspace(line[i]) == 0 && line[i] != 0 \
					&& line[i] != '\'' && line[i] != '\"')
					arg = str_append_char(arg, line[i++]);
			}
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

void handle_line(char **line_prompt, t_list **arg_cmd_tmp, t_quote *quote, t_envp *envp)
{
	char **arg_arr;

	add_history(line_prompt[0]);
	check_quote(line_prompt[0], quote);
	arg_cmd_tmp[0] = get_arg_list(line_prompt[0], *quote);
	arg_cmd_tmp[1] = list_to_char_arr(arg_cmd_tmp[0], envp);
	arg_cmd_tmp[2] = arg_cmd_tmp[1];
	while (arg_cmd_tmp[1])
	{
		arg_arr = (char **)arg_cmd_tmp[1]->content;
		envp->last_status = interpret(arg_arr, envp);
		arg_cmd_tmp[1] = arg_cmd_tmp[1]->next;
	}
	arg_cmd_tmp[1] = arg_cmd_tmp[2];
	free(line_prompt[0]);
	free(line_prompt[1]);
	line_prompt[1] = make_prompt();
	line_prompt[0] = readline(line_prompt[1]);
}

int	main(int argc, char **argv, char **first_envp)
{
	t_list		**arg_cmd_tmp;
	char		**arg_arr;
	char		**line_prompt;
	t_quote		quote;
	t_envp		envp;

	init_quote(&quote);
	init_envp(&envp, first_envp);
	set_signal();
	line_prompt = (char **)malloc(sizeof(char *) * 2);
	arg_cmd_tmp = (t_list **)malloc(sizeof(t_list *) * 3);
	line_prompt[1] = make_prompt();
	line_prompt[0] = readline(line_prompt[1]);
	while (line_prompt[0] != NULL)
		handle_line(line_prompt, arg_cmd_tmp, &quote, &envp);
	free(line_prompt[1]);
	mini_exit();
	return (0);
}
