/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   worker.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ramoussa <ramoussa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/23 21:15:36 by ramoussa          #+#    #+#             */
/*   Updated: 2023/11/01 21:48:54 by ramoussa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

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
		philo->meals_eaten++;
	}
	return (0);
}

int	philo_worker(t_simulation *env, t_philo *philo)
{
	int					order;

	order = 1;
	if (env->num_of_philosophers % 2 == 0)
		order = 0;
	if (philo->number == order)
		log_state(env, philo);
	else if (philo->meals_eaten < env->meals_count && \
		philo->number % 2 == 0)
	{
		log_state(env, philo);
		if (ph_think(env, philo, env->time_to_eat))
			return (1);
		if (order)
			order++;
	}
	philo_lifecycle(env, philo, order);
	return (0);
}
