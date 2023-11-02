/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   logger.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ramoussa <ramoussa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/22 20:27:17 by ramoussa          #+#    #+#             */
/*   Updated: 2023/11/02 21:47:36 by ramoussa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

void	log_state(t_simulation *env, t_philo *philo)
{
	int	time_arrow;

	time_arrow = time_since(to_timestamp(env->begin));
	sem_wait(env->logger_sem);
	if (philo->status == EAT)
		printf("%i %i is eating\n", time_arrow, philo->number);
	if (philo->status == SLEEP)
		printf("%i %i is sleeping\n", time_arrow, philo->number);
	if (philo->status == THINK)
		printf("%i %i is thinking\n", time_arrow, philo->number);
	if (philo->status == DEAD)
		printf("%i %i died\n", time_arrow, philo->number);
	if (philo->status == ACQUIRE)
		printf("%i %i has taken a fork\n", time_arrow, philo->number);
	sem_post(env->logger_sem);
}

void	print_env(t_simulation *env)
{
	printf("Number of philosophers %d\n", env->num_of_philosophers);
	printf("Time to eat %d\n", env->time_to_eat);
	printf("Time to sleep %d\n", env->time_to_sleep);
	printf("Time to die %d\n", env->time_to_die);
	printf("Number of times each philosopher must eat %d\n", env->meals_count);
}
