/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   state.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ramoussa <ramoussa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/22 20:32:34 by ramoussa          #+#    #+#             */
/*   Updated: 2023/10/25 13:48:36 by ramoussa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

void	set_state(t_simulation *env, t_philo *philo, \
			enum e_philo_state status)
{
	philo->status = status;
	log_state(env, philo);
}

int	is_dead(t_philo *philo)
{
	pthread_mutex_lock(&philo->starvation_mutex);
	if (philo->died == 1)
	{
		pthread_mutex_unlock(&philo->starvation_mutex);
		return (1);
	}
	pthread_mutex_unlock(&philo->starvation_mutex);
	return (0);
}

int	has_any_death(t_simulation *env)
{
	pthread_mutex_lock(&env->any_death_mutex);
	if (env->has_death)
	{
		pthread_mutex_unlock(&env->any_death_mutex);
		return (1);
	}
	pthread_mutex_unlock(&env->any_death_mutex);
	return (0);
}

int	will_starve(t_simulation *env, t_philo *philo, int action_ms)
{
	time_t	since;

	if (has_any_death(env))
		return (1);
	since = time_since(to_timestamp(philo->last_meal_at));
	if (since + action_ms > env->time_to_die)
	{
		pthread_mutex_lock(&env->any_death_mutex);
		env->has_death = 1;
		pthread_mutex_unlock(&env->any_death_mutex);
		time_sleep(env->time_to_die - since);
		if (is_dead(philo))
			return (1);
		set_state(env, philo, DEAD);
		pthread_mutex_lock(&philo->starvation_mutex);
		philo->died = 1;
		pthread_mutex_unlock(&philo->starvation_mutex);
		return (1);
	}
	return (0);
}

int	has_death(t_simulation *env)
{
	int	idx;

	idx = 0;
	while (idx < env->num_of_philosophers)
	{
		if (is_dead(&env->philos[idx]))
			return (1);
		idx++;
	}
	return (0);
}
