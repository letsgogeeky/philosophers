/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   worker.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ramoussa <ramoussa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/23 21:15:36 by ramoussa          #+#    #+#             */
/*   Updated: 2023/10/24 21:26:47 by ramoussa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	philo_lifecycle(t_simulation *env, t_philo *philo, int order)
{
	while (philo->meals_eaten < env->meals_count)
	{
		if (philo->number == order && order++ && \
			ph_think(env, philo, env->time_to_eat))
			return (1);
		if (ph_eat(env, philo))
			return (1);
		if (ph_sleep(env, philo))
			return (1);
		if (env->time_to_eat > env->time_to_sleep && \
			ph_think(env, philo, env->time_to_eat - env->time_to_sleep))
			return (1);
		if (order && order % 2 == 1 && order >= env->num_of_philosophers)
			order = 1;
		else if (order && order % 2 == 0 && order >= env->num_of_philosophers)
			order = 2;
		else if (order)
			order += 2;
		pthread_mutex_lock(&philo->eating_mutex);
		philo->meals_eaten++;
		pthread_mutex_unlock(&philo->eating_mutex);
	}
	return (0);
}

void	*philo_worker(void *arg)
{
	t_philo_worker_arg	*ep;
	int					order;

	ep = (t_philo_worker_arg *)arg;
	order = 1;
	if (ep->env->num_of_philosophers % 2 == 0)
		order = 0;
	if (ep->philo->number == order)
		log_state(ep->env, ep->philo);
	else if (ep->philo->meals_eaten < ep->env->meals_count && \
		ep->philo->number % 2 == 0)
	{
		log_state(ep->env, ep->philo);
		if (ph_think(ep->env, ep->philo, ep->env->time_to_eat))
			return (free(ep), NULL);
		if (order)
			order++;
	}
	philo_lifecycle(ep->env, ep->philo, order);
	return (free(ep), NULL);
}
