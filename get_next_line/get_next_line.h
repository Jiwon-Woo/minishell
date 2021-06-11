/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clim <clim@student.42seoul.kr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/13 13:12:23 by clim              #+#    #+#             */
/*   Updated: 2021/01/15 13:41:19 by clim             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H

# include <stdlib.h>
# include <unistd.h>
# include <fcntl.h>

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 500
# endif

# ifndef OPEN_MAX
#  define OPEN_MAX 32
# endif

int				get_next_line(int fd, char **line);

size_t			ft_strlen(const char *s);

char			*ft_strdup(const char *s1);

char			*ft_strjoin(char *s1, char *s2);

size_t			ft_strlcpy(char *dest, const char *src, size_t dstsize);

size_t			ft_strlcat(char *dest, const char *src, size_t dstsize);

#endif
