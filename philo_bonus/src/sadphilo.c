/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sadphilo.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ramoussa <ramoussa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/23 21:13:14 by ramoussa          #+#    #+#             */
/*   Updated: 2023/10/25 13:48:33 by ramoussa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

void	sad_philo(t_simulation *env)
{
	env->philos[0].status = ACQUIRE;
	log_state(env, &env->philos[0]);
	time_sleep(env->time_to_die);
	env->philos[0].status = DEAD;
	log_state(env, &env->philos[0]);
}
