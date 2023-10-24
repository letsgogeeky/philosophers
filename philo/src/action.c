/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   action.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ramoussa <ramoussa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/22 20:36:54 by ramoussa          #+#    #+#             */
/*   Updated: 2023/10/24 21:18:25 by ramoussa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	ph_sleep(t_simulation *env, t_philo *philo)
{
	set_state(env, philo, SLEEP);
	if (will_starve(env, philo, env->time_to_sleep))
		return (1);
	time_sleep(env->time_to_sleep);
	if (is_dead(philo))
		return (1);
	set_state(env, philo, THINK);
	return (0);
}

int	ph_think(t_simulation *env, t_philo *philo, int thinking_ms)
{
	if (will_starve(env, philo, thinking_ms))
		return (1);
	time_sleep(thinking_ms);
	if (is_dead(philo))
		return (1);
	return (0);
}

static int	ph_odd_eat(t_simulation *env, t_philo *philo)
{
	philo->status = ACQUIRE;
	pthread_mutex_lock(philo->left_fork);
	log_state(env, philo);
	pthread_mutex_lock(philo->right_fork);
	log_state(env, philo);
	set_state(env, philo, EAT);
	gettimeofday(&philo->last_meal_at, NULL);
	if (will_starve(env, philo, env->time_to_eat))
	{
		pthread_mutex_unlock(philo->right_fork);
		pthread_mutex_unlock(philo->left_fork);
		return (1);
	}
	time_sleep(env->time_to_eat);
	pthread_mutex_unlock(philo->right_fork);
	pthread_mutex_unlock(philo->left_fork);
	return (0);
}

static int	ph_even_eat(t_simulation *env, t_philo *philo)
{
	philo->status = ACQUIRE;
	pthread_mutex_lock(philo->right_fork);
	log_state(env, philo);
	pthread_mutex_lock(philo->left_fork);
	log_state(env, philo);
	set_state(env, philo, EAT);
	gettimeofday(&philo->last_meal_at, NULL);
	if (will_starve(env, philo, env->time_to_eat))
	{
		pthread_mutex_unlock(philo->left_fork);
		pthread_mutex_unlock(philo->right_fork);
		return (1);
	}
	time_sleep(env->time_to_eat);
	pthread_mutex_unlock(philo->left_fork);
	pthread_mutex_unlock(philo->right_fork);
	return (0);
}

int	ph_eat(t_simulation *env, t_philo *philo)
{
	philo->status = EAT;
	if (philo->number % 2 == 0)
	{
		if (ph_even_eat(env, philo))
			return (1);
	}
	else
	{
		if (ph_odd_eat(env, philo))
			return (1);
	}
	if (is_dead(philo))
		return (1);
	return (0);
}
