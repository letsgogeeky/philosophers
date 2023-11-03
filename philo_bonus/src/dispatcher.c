/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dispatcher.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ramoussa <ramoussa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/23 21:09:40 by ramoussa          #+#    #+#             */
/*   Updated: 2023/11/03 16:07:43 by ramoussa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

int	init_philo(t_simulation *env, t_philo *philo, int number)
{
	philo->number = number;
	philo->status = THINK;
	philo->meals_eaten = 0;
	philo->last_meal_at = env->begin;
	return (0);
}

int	setup_simulation(t_simulation *env)
{
	env->bigbang_at = time_now();
	env->logger_sem = sem_open("/logger", O_CREAT, 0644, 1);
	sem_unlink("/logger");
	if (env->num_of_philosophers == 1)
		sad_philo(env);
	env->forks = sem_open("/forks", O_CREAT, 0644, env->num_of_philosophers);
	sem_unlink("/forks");
	return (0);
}

void	wait_children(t_simulation *env)
{
	int	idx;

	idx = 0;
	while (idx < env->num_of_philosophers)
	{
		waitpid(-1, NULL, 0);
		idx++;
	}
}

int	dispatch_philosophers(t_simulation *env)
{
	int					idx;
	pid_t				pid;

	idx = 0;
	while (idx < env->num_of_philosophers)
	{
		init_philo(env, &env->philos[idx], idx + 1);
		pid = fork();
		if (pid == 0)
		{
			if (philo_worker(env, &env->philos[idx]))
				return (1);
			return (0);
		}
		if (pid != -1)
			idx++;
	}
	if (pid != 0)
		wait_children(env);
	return (sem_unlink("/finish"), 0);
}

int	begin_simulation(t_simulation *env)
{
	if (dispatch_philosophers(env))
		abort_exit(env, NULL, 1);
	return (0);
}
