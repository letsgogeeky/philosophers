/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   memory.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ramoussa <ramoussa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/14 17:08:06 by ramoussa          #+#    #+#             */
/*   Updated: 2023/10/22 21:51:13 by ramoussa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo.h"

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

t_philo_worker_arg	*copy_philo_worker_arg(t_philo_worker_arg arg)
{
	t_philo_worker_arg *worker_arg;
	
	worker_arg = (t_philo_worker_arg *)malloc(sizeof(t_philo_worker_arg));
	worker_arg->env = arg.env;
	worker_arg->philo = arg.philo;
	return (worker_arg);
}
