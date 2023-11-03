/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   offload.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ramoussa <ramoussa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/22 20:09:54 by ramoussa          #+#    #+#             */
/*   Updated: 2023/11/03 16:13:33 by ramoussa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

void	abort_exit(t_simulation *env, char *msg, int exit_code)
{
	if (msg)
		printf("%s\n", msg);
	if (env->philos)
		free(env->philos);
	sem_close(env->logger_sem);
	sem_close(env->forks);
	if (env->death)
		sem_close(env->death);
	free(env);
	exit(exit_code);
}

/*
Deprecated on MacOS. Works on Linux
if (env->forks)
	sem_destroy(env->forks);
sem_destroy(env->logger_sem);
*/