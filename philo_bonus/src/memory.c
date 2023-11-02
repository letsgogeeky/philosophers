/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   memory.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ramoussa <ramoussa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/14 17:08:06 by ramoussa          #+#    #+#             */
/*   Updated: 2023/11/02 21:54:03 by ramoussa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

void	*ft_calloc(unsigned int count, unsigned int size)
{
	char				*ptr;
	unsigned int		idx;

	ptr = (char *)malloc(count * size);
	if (ptr == 0)
		return (0);
	idx = 0;
	while (idx < count * size)
	{
		ptr[idx] = 0;
		idx++;
	}
	return (ptr);
}
