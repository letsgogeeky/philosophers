/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   offload.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ramoussa <ramoussa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/22 20:09:54 by ramoussa          #+#    #+#             */
/*   Updated: 2023/11/02 21:56:40 by ramoussa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

void	abort_exit(t_simulation *env, char *msg, int exit_code)
{
	if (msg)
		printf("%s\n", msg);
	if (env->philos)
		free(env->philos);
	// if (env->forks)
	// 	free(env->forks);
	free(env);
	exit(exit_code);
}
