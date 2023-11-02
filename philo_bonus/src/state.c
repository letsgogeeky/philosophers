/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   state.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ramoussa <ramoussa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/22 20:32:34 by ramoussa          #+#    #+#             */
/*   Updated: 2023/11/02 19:35:01 by ramoussa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

void	set_state(t_simulation *env, t_philo *philo, \
			enum e_philo_state status)
{
	philo->status = status;
	log_state(env, philo);
}
int	has_death()
{
	if (sem_open("/finish", 0) != SEM_FAILED)
		return (1);
	return (0);	
}

int	will_starve(t_simulation *env, t_philo *philo, int action_ms)
{
	time_t	since;

	since = time_since(to_timestamp(philo->last_meal_at));
	if (since + action_ms > env->time_to_die)
	{
		time_sleep(env->time_to_die - since);
		if (sem_open("/finish", 0) != SEM_FAILED)
			return (1);
		set_state(env, philo, DEAD);
		sem_open("/finish", O_CREAT, 0644, 1);
		return (1);
	}
	return (0);
}
