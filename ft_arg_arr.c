#include "minishell.h"

char	*str_append_char(char *str, char c)
{
	char	*ret;
	int	len;
	int	i;
	
	if (str == 0)
		str = ft_strdup("");
	ret = (char *)malloc(ft_strlen(str) + 2);
	i = -1;
	while (++i < ft_strlen(str))
		ret[i] = str[i];
	ret[i] = c;
	ret[i + 1] = 0;
	free(str);
	str = 0;
	return (ret);
}

int	is_separate(char *command)
{
	if (ft_strncmp(command, "|", 2) == 0)
		return (1);
	if (ft_strncmp(command, "<", 2) == 0)
		return (2);
	if (ft_strncmp(command, ">", 2) == 0)
		return (3);
	if (ft_strncmp(command, "<<", 3) == 0)
		return (4);
	if (ft_strncmp(command, ">>", 3) == 0)
		return (5);
	if (ft_strncmp(command, "&&", 3) == 0)
		return (6);
	if (ft_strncmp(command, "||", 3) == 0)
		return (7);
	return (0);
}

int	get_list_size(t_list *arg_list)
{
	int	size;
	t_list	*first;

	size = 0;
	first = arg_list;
	while (arg_list)
	{
		if (is_separate((char *)arg_list->content) > 0)
			break;
		size++;
		arg_list = arg_list->next;
	}
	arg_list = first;
	return (size);
}

char	*remove_qnote(char *cmd_line, t_envp *envp)
{
	int	i = 0;
	t_quote quote;
	char	*ret = 0;

	init_quote(&quote);
	check_quote(cmd_line, &quote);
	ret = ft_strdup("");
	while (cmd_line[i])
	{
		if ((cmd_line[i] == '\"' && i != quote.q_double_index) ||
			(cmd_line[i] == '\'' && i != quote.q_single_index))
		{	
			if (cmd_line[i++] == '\'')
			{
				while (cmd_line[i] != '\'' && cmd_line[i] != 0)
					ret = str_append_char(ret, cmd_line[i++]);
				if (cmd_line[i] == '\'')
					i++;
			}
			else
			{
				while (cmd_line[i] != '\"' && cmd_line[i] != 0)
				{
					if (cmd_line[i] == '$')
					{
						char *key = 0;
						i++;
						while (cmd_line[i] != 0 && cmd_line[i] != '\"' && cmd_line[i] != '$' && cmd_line[i] != '?' && ft_isspace(cmd_line[i]) == 0)
						{
							key = str_append_char(key, cmd_line[i++]);
						}
						if (key == 0 && cmd_line[i] == '$')
						{
							key = ft_strdup("$");
							i++;
						}
						if (key == 0 && cmd_line[i] == '?')
						{
							ret = ft_strjoin(ret, ft_itoa(envp->last_status));
							i++;
						}
						else if (key == 0)
							ret = str_append_char(ret, '$');
						else
						{
							char *value = get_value(key, envp->envp_list);
							// char *value = getenv(key);
							ret = ft_strjoin(ret, value);
						}
					}
					else
						ret = str_append_char(ret, cmd_line[i++]);
				}
				if (cmd_line[i] == '\"')
					i++;
			}
		}
		else if (cmd_line[i] == '$')
		{
			char *key = 0;
			i++;
			while (cmd_line[i] != 0 && (!(cmd_line[i] == '\"' && i != quote.q_double_index)
				&& !(cmd_line[i] == '\'' && i != quote.q_single_index))
				&& cmd_line[i] != '$' && cmd_line[i] != '?' && ft_isspace(cmd_line[i]) == 0)
			{
				key = str_append_char(key, cmd_line[i++]);
			}
			if (key == 0 && cmd_line[i] == '$')
			{
				key = ft_strdup("$");
				i++;
			}
			if (key == 0 && cmd_line[i] == '?')
			{
				ret = ft_strjoin(ret, ft_itoa(envp->last_status));
				i++;
			}
			else if (key == 0)
				ret = str_append_char(ret, '$');
			else
			{
				char *value = get_value(key, envp->envp_list);
				// char *value = getenv(key);
				ret = ft_strjoin(ret, value);
			}
		}
		else
		{
			if (cmd_line[i] == '\'' || cmd_line[i] == '\"')
				ret = str_append_char(ret, cmd_line[i++]);
			while (cmd_line[i] != '\'' && cmd_line[i] != '\"' && cmd_line[i] != 0)
				ret = str_append_char(ret, cmd_line[i++]);
		}
	}
	// printf("ret : %s\n", ret);
	return (ret);
}

t_list	*list_to_char_arr(t_list *arg_list, t_envp *envp)
{
	t_list	*first;
	t_list	*cmd_list = 0;
	int	lst_size;
	int	idx;
	char	**arg_arr;

	lst_size = ft_lstsize(arg_list);
	first = arg_list;
	while (arg_list)
	{
		// lst_size = get_list_size(arg_list);
		int	sperate_num = get_list_size(arg_list);
		arg_arr = (char **)malloc(sizeof(char *) * (sperate_num + 1));
		idx = -1;
		while (++idx < sperate_num)
		{
			arg_arr[idx] = remove_qnote((char *)arg_list->content, envp);
			arg_list = arg_list->next;
		}
		arg_arr[sperate_num] = (char *)0;
		t_list *temp = ft_lstnew(arg_arr);
		ft_lstadd_back(&cmd_list, temp);
		if (arg_list != NULL)
		{
			temp->pipe_redirect = is_separate((char *)arg_list->content);
			arg_list = arg_list->next;
		}
	}
	arg_list = first;
	return (cmd_list);
}