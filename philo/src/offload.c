/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   offload.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ramoussa <ramoussa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/22 20:09:54 by ramoussa          #+#    #+#             */
/*   Updated: 2023/10/22 20:25:22 by ramoussa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	adjust_for_midlaunch_abort(t_simulation *env, pthread_mutex_t *to_destory1, \
			pthread_mutex_t *to_destroy2, int philos_initialized)
{
	if (to_destory1 != NULL)
		pthread_mutex_destroy(to_destory1);
	if (to_destroy2 != NULL)
		pthread_mutex_destroy(to_destory1);
	env->num_of_philosophers = philos_initialized;
}

void	destroy_mutexes(t_simulation *env)
{
	int	idx;

	idx = 0;
	while (idx < env->num_of_philosophers)
	{
		pthread_mutex_destroy(&env->forks[idx]);
		pthread_mutex_destroy(&env->philos[idx].eating_mutex);
		pthread_mutex_destroy(&env->philos[idx].starvation_mutex);
	}
	pthread_mutex_destroy(&env->logger_mutex);
}

void abort_exit(t_simulation *env, char *msg, int exit_code)
{
	(void)env;
	if (msg)
		printf("%s\n", msg);
	destroy_mutexes(env);
	free(env->philos);
	free(env->forks);
	free(env);
	exit(exit_code);
}
