/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   action.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ramoussa <ramoussa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/22 20:36:54 by ramoussa          #+#    #+#             */
/*   Updated: 2023/11/01 22:27:59 by ramoussa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

int	ph_sleep(t_simulation *env, t_philo *philo)
{
	set_state(env, philo, SLEEP);
	if (will_starve(env, philo, env->time_to_sleep))
		return (1);
	time_sleep(env->time_to_sleep);
	if (has_death())
		return (1);
	set_state(env, philo, THINK);
	return (0);
}

int	ph_think(t_simulation *env, t_philo *philo, int thinking_ms)
{
	if (will_starve(env, philo, thinking_ms))
		return (1);
	time_sleep(thinking_ms);
	if (has_death())
		return (1);
	return (0);
}

int	ph_eat(t_simulation *env, t_philo *philo)
{
	// philo->status = EAT;
	philo->status = ACQUIRE;
	sem_wait(env->forks);
	log_state(env, philo);
	sem_wait(env->forks);
	log_state(env, philo);
	set_state(env, philo, EAT);
	gettimeofday(&philo->last_meal_at, NULL);
	if (will_starve(env, philo, env->time_to_eat))
	{
		sem_post(env->forks);
		sem_post(env->forks);
		return (1);
	}
	time_sleep(env->time_to_eat);
	sem_post(env->forks);
	sem_post(env->forks);
	if (has_death())
		return (1);
	return (0);
}
