/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   watcher.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ramoussa <ramoussa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/22 20:50:35 by ramoussa          #+#    #+#             */
/*   Updated: 2023/10/24 21:28:24 by ramoussa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	signal_terminate_all(t_simulation *env);
static int	should_serve_more_meals(t_simulation *env);

void	*watcher_worker(void *simulation)
{
	t_simulation	*env;

	env = (t_simulation *)simulation;
	while (1)
	{
		time_sleep(1);
		if (has_death(env))
		{
			signal_terminate_all(env);
			break ;
		}
		if (should_serve_more_meals(env) == 0)
			break ;
	}
	return (NULL);
}

static void	signal_terminate_all(t_simulation *env)
{
	int	idx;

	idx = 0;
	while (idx < env->num_of_philosophers)
	{
		pthread_mutex_lock(&env->philos[idx].starvation_mutex);
		env->philos[idx].died = 1;
		pthread_mutex_unlock(&env->philos[idx].starvation_mutex);
		idx++;
	}
}

static int	should_serve_more_meals(t_simulation *env)
{
	int	idx;

	idx = 0;
	while (idx < env->num_of_philosophers)
	{
		pthread_mutex_lock(&env->philos[idx].eating_mutex);
		if (env->philos[idx].meals_eaten < env->meals_count)
		{
			pthread_mutex_unlock(&env->philos[idx].eating_mutex);
			return (1);
		}
		pthread_mutex_unlock(&env->philos[idx].eating_mutex);
		idx++;
	}
	return (0);
}
