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

void replace_env_value(char *key, char **ret, t_envp *envp)
{
	char	*value;

	if (key == 0)
		*ret = str_append_char(*ret, '$');
	else
	{
		value = get_value(key, envp->envp_list);
		*ret = ft_strjoin_with_free(*ret, value);
	}
}

void	check_single_in_double(char *line, int i, t_quote *quote)
{
	init_quote(quote);
	while (line[i] != 0 && line[i] != '\"')
	{
		if (line[i] == '\'')
		{
			quote->q_single *= -1;
			quote->q_single_index = i;
		}
		i++;
	}
	if (quote->q_single != -1)
		quote->q_single_index = -1;
}

int find_env_in_double_quote(char **ret, char *cmd_line, int i, t_envp *envp)
{
	char	*key;
	t_quote quote;

	check_single_in_double(cmd_line, i, &quote);
	key = 0;
	while (cmd_line[i] != 0 && ft_isspace(cmd_line[i]) == 0
		&& cmd_line[i] != '$' && cmd_line[i] != '?'
		&& cmd_line[i] != '\"' && !(cmd_line[i] == '\'' && i != quote.q_single_index))
	{
		key = str_append_char(key, cmd_line[i++]);
	}
	if (key == 0 && (cmd_line[i] == '$' || cmd_line[i] == '?'))
	{
		if (cmd_line[i] == '$')
			key = ft_strdup("$");
		else
			*ret = ft_strjoin_with_free(*ret, ft_itoa(envp->last_status));
		i++;
	}
	else
		replace_env_value(key, ret, envp);
	if (key != 0)
	{
		free(key);
		key = 0;
	}
	return (i);
}

int find_env_without_quote(char **ret, char *cmd_line, int i, t_envp *envp)
{
	char	*key;
	t_quote	quote;

	init_quote(&quote);
	check_quote(cmd_line, &quote);
	key = 0;
	while (cmd_line[i] != 0 && (!(cmd_line[i] == '\"' && i != quote.q_double_index)
		&& !(cmd_line[i] == '\'' && i != quote.q_single_index))
		&& cmd_line[i] != '$' && cmd_line[i] != '?' && ft_isspace(cmd_line[i]) == 0)
	{
		key = str_append_char(key, cmd_line[i++]);
	}
	if (key == 0 && (cmd_line[i] == '$' || cmd_line[i] == '?'))
	{
		if (cmd_line[i] == '$')
			key = ft_strdup("$");
		else
			*ret = ft_strjoin_with_free(*ret, ft_itoa(envp->last_status));
		i++;
	}
	else
		replace_env_value(key, ret, envp);
	if (key != 0)
	{
		free(key);
		key = 0;
	}
	return (i);
}

int	in_single_quote(char **ret, char *cmd_line, int i)
{
	while (cmd_line[i] != '\'' && cmd_line[i] != 0)
		*ret = str_append_char(*ret, cmd_line[i++]);
	if (cmd_line[i] == '\'')
		i++;
	return (i);
}

int in_double_quote(char **ret, char *cmd_line, int i, t_envp *envp)
{
	while (cmd_line[i] != '\"' && cmd_line[i] != 0)
	{
		if (cmd_line[i] == '$')
			i = find_env_in_double_quote(ret, cmd_line, ++i, envp);
		else
			*ret = str_append_char(*ret, cmd_line[i++]);
	}
	if (cmd_line[i] == '\"')
		i++;
	return (i);
}

int without_quote(char **ret, char *cmd_line, int i)
{
	if (cmd_line[i] == '\'' || cmd_line[i] == '\"')
		*ret = str_append_char(*ret, cmd_line[i++]);
	while (cmd_line[i] != '\'' && cmd_line[i] != '\"' && cmd_line[i] != 0)
		*ret = str_append_char(*ret, cmd_line[i++]);
	return (i);
}

char	*remove_quote(char *cmd_line, t_envp *envp)
{
	int	i;
	t_quote	quote;
	char	*ret;

	init_quote(&quote);
	check_quote(cmd_line, &quote);
	ret = ft_strdup("");
	i = 0;
	while (cmd_line[i])
	{
		if (cmd_line[i] == '\'' && i != quote.q_single_index)
			i = in_single_quote(&ret, cmd_line, ++i);
		else if (cmd_line[i] == '\"' && i != quote.q_double_index)
			i = in_double_quote(&ret, cmd_line, ++i, envp);
		else if (cmd_line[i] == '$')
			i = find_env_without_quote(&ret, cmd_line, ++i, envp);
		else
			i = without_quote(&ret, cmd_line, i);
	}
	return (ret);
}

// char	*flagnum_to_sperate(int flag)
// {
// 	if (flag == 1)
// 		return (ft_strdup("|"));
// 	if (flag == 2)
// 		return (ft_strdup(">"));
// 	if (flag == 3)
// 		return (ft_strdup("<"));
// 	if (flag == 4)
// 		return (ft_strdup(">>"));
// 	if (flag == 5)
// 		return (ft_strdup("<<"));
// 	if (flag == 6)
// 		return (ft_strdup("&&"));
// 	if (flag == 6)
// 		return (ft_strdup("||"));
// 	return (0);
// }

void	add_one_line_of_cmd(t_list **arg_list, t_envp *envp, t_list **cmd_list)
{
	int	sperate_num;
	char	**arg_arr;
	int	idx;
	t_list *temp;

	sperate_num = get_list_size(*arg_list);
	idx = -1;
	arg_arr = (char **)malloc(sizeof(char *) * (sperate_num + 1));
	while (++idx < sperate_num)
	{
		arg_arr[idx] = remove_quote((char *)(*arg_list)->content, envp);
		(*arg_list) = (*arg_list)->next;
	}
	arg_arr[sperate_num] = (char *)0;
	temp = ft_lstnew(arg_arr);
	if ((*arg_list) != NULL)
	{
		temp->next_flag = is_separate((char *)(*arg_list)->content);
		(*arg_list) = (*arg_list)->next;
	}
	else
	{
		temp->next_flag = 0;
	}
	if (temp != 0 && ft_lstlast(*cmd_list) != 0)
		temp->pre_flag = ft_lstlast(*cmd_list)->next_flag;
	else if (temp != 0)
		temp->pre_flag = 0;
	ft_lstadd_back(cmd_list, temp);
}

t_list	*list_to_char_arr(t_list *arg_list, t_envp *envp)
{
	t_list	*first;
	t_list	*cmd_list = 0;
	int	lst_size;
	int	idx;

	lst_size = ft_lstsize(arg_list);
	first = arg_list;
	while (arg_list)
	{
		add_one_line_of_cmd(&arg_list, envp, &cmd_list);
	}
	arg_list = first;
	return (cmd_list);
}
