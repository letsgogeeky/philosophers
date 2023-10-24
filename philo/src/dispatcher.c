/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dispatcher.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ramoussa <ramoussa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/23 21:09:40 by ramoussa          #+#    #+#             */
/*   Updated: 2023/10/24 21:39:06 by ramoussa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	seat_philos(t_simulation *env)
{
	int	idx;

	idx = 0;
	while (idx < env->num_of_philosophers)
	{
		env->philos[idx].number = idx + 1;
		env->philos[idx].status = THINK;
		env->philos[idx].meals_eaten = 0;
		if (pthread_mutex_init(&env->forks[idx], NULL))
			return (adjust_for_midlaunch_abort(env, NULL, NULL, idx - 1), 1);
		if (pthread_mutex_init(&env->philos[idx].eating_mutex, NULL))
			return (adjust_for_midlaunch_abort(env, \
				&env->forks[idx], NULL, idx - 1), 1);
		if (pthread_mutex_init(&env->philos[idx].starvation_mutex, NULL))
			return (adjust_for_midlaunch_abort(env, \
				&env->forks[idx], &env->philos[idx].eating_mutex, idx - 1), 1);
		env->philos[idx].right_fork = &env->forks[idx];
		if (idx >= 1)
			env->philos[idx - 1].left_fork = &env->forks[idx];
		idx++;
	}
	env->philos[idx - 1].left_fork = &env->forks[0];
	return (0);
}

int	setup_simulation(t_simulation *env)
{
	env->bigbang_at = time_now();
	if (pthread_mutex_init(&env->logger_mutex, NULL))
		return (1);
	if (pthread_mutex_init(&env->any_death_mutex, NULL))
		return (1);
	if (seat_philos(env))
		abort_exit(env, NULL, 1);
	if (env->num_of_philosophers == 1)
	{
		sad_philo(env);
		return (1);
	}
	return (0);
}

void	join_all_threads(t_simulation *env)
{
	int	idx;

	idx = 0;
	while (idx < env->num_of_philosophers)
	{
		pthread_join(env->philos[idx].worker, NULL);
		idx++;
	}
}

int	dispatch_philosophers(t_simulation *env)
{
	int					idx;
	t_philo_worker_arg	worker_arg;

	idx = 0;
	while (idx < env->num_of_philosophers)
	{
		worker_arg.env = env;
		worker_arg.philo = &env->philos[idx];
		if (pthread_create(&env->philos[idx].worker, NULL, philo_worker, \
				copy_philo_worker_arg(worker_arg)))
			return (1);
		idx++;
	}
	return (0);
}

int	begin_simulation(t_simulation *env)
{
	pthread_t	watcher;

	if (pthread_create(&watcher, NULL, watcher_worker, env))
		return (1);
	if (dispatch_philosophers(env))
		abort_exit(env, NULL, 1);
	pthread_join(watcher, NULL);
	join_all_threads(env);
	return (0);
}
