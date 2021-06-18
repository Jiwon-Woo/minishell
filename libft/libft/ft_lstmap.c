/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstmap.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jwoo <jwoo@student.42seoul.kr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/05 16:52:28 by jwoo              #+#    #+#             */
/*   Updated: 2021/01/05 23:54:57 by jwoo             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_list	*ft_lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *))
{
	t_list	*make_list;
	t_list	*new;

	if (lst == NULL || f == NULL)
		return (NULL);
	make_list = ft_lstnew(f(lst->content));
	if (make_list == NULL)
		return (NULL);
	new = make_list;
	while (lst->next != NULL)
	{
		if (ft_lstnew(f(lst->content)) == NULL)
		{
			ft_lstclear(&new, del);
			return (NULL);
		}
		make_list->next = ft_lstnew(f(lst->next->content));
		lst = lst->next;
		make_list = make_list->next;
	}
	make_list->next = NULL;
	return (new);
}
