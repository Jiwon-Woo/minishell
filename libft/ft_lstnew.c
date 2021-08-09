#include "libft.h"

t_list	*ft_lstnew(void *content)
{
	t_list	*lst;

	lst = (t_list *)malloc(sizeof(t_list));
	if (lst == NULL)
		exit (1);
	lst->content = content;
	lst->next = NULL;
	return (lst);
}
