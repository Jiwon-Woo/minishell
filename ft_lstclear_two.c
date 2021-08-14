/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstclear_two.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jwoo <jwoo@student.42seoul.kr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/05 12:08:01 by jwoo              #+#    #+#             */
/*   Updated: 2021/08/14 11:24:24 by jwoo             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_lstclear_two(t_list **lst, void (*del)(char **))
{
	t_list	*temp;

	if (lst == NULL || del == NULL)
		return ;
	while (*lst != NULL)
	{
		temp = *lst;
		del((*lst)->content);
		(*lst)->content = 0;
		*lst = (*lst)->next;
		free(temp);
		temp = 0;
	}
	return ;
}
