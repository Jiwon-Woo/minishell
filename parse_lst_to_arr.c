/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_lst_to_arr.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jwoo <jwoo@student.42seoul.kr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/10 10:15:26 by jwoo              #+#    #+#             */
/*   Updated: 2021/08/10 19:28:13 by jwoo             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_separate(char *command)
{
	if (ft_strncmp(command, "|", 2) == 0)
		return (PIPE);
	if (ft_strncmp(command, "<", 2) == 0)
		return (REDIRECT1);
	if (ft_strncmp(command, ">", 2) == 0)
		return (REDIRECT2);
	if (ft_strncmp(command, "<<", 3) == 0)
		return (REDIRECT3);
	if (ft_strncmp(command, ">>", 3) == 0)
		return (REDIRECT4);
	return (NONE);
}

int	get_unit_size(t_list *arg_list)
{
	int		size;
	t_list	*first;

	size = 0;
	first = arg_list;
	while (arg_list)
	{
		if (is_separate((char *)arg_list->content) != NONE)
			break ;
		size++;
		arg_list = arg_list->next;
	}
	arg_list = first;
	return (size);
}

void	add_one_unit_cmd(t_list **arg_list, t_list **cmd_list, char	**arg_arr)
{
	t_list	*unit;

	unit = ft_lstnew(arg_arr);
	if ((*arg_list) != NULL)
	{
		unit->next_flag = is_separate((char *)(*arg_list)->content);
		(*arg_list) = (*arg_list)->next;
	}
	else
	{
		unit->next_flag = NONE;
	}
	if (unit != 0 && ft_lstlast(*cmd_list) != 0)
		unit->pre_flag = ft_lstlast(*cmd_list)->next_flag;
	else if (unit != 0)
		unit->pre_flag = NONE;
	ft_lstadd_back(cmd_list, unit);
}

void	add_one_line_of_cmd(t_list **arg_list, t_envp *envp, t_list **cmd_list)
{
	int		sperate_num;
	char	**arg_arr;
	int		idx;

	sperate_num = get_unit_size(*arg_list);
	idx = -1;
	arg_arr = (char **)malloc(sizeof(char *) * (sperate_num + 1));
	if (!arg_arr)
		exit(1);
	while (++idx < sperate_num)
	{
		arg_arr[idx] = remove_quote((char *)(*arg_list)->content, envp);
		(*arg_list) = (*arg_list)->next;
	}
	arg_arr[sperate_num] = (char *)0;
	add_one_unit_cmd(arg_list, cmd_list, arg_arr);
}

t_list	*list_to_char_arr(t_list *arg_list, t_envp *envp)
{
	t_list	*first;
	t_list	*cmd_list;
	int		lst_size;

	cmd_list = 0;
	lst_size = ft_lstsize(arg_list);
	first = arg_list;
	while (arg_list)
	{
		add_one_line_of_cmd(&arg_list, envp, &cmd_list);
	}
	arg_list = first;
	return (cmd_list);
}
