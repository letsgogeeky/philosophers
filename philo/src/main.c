/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ramoussa <ramoussa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/06 17:11:56 by ramoussa          #+#    #+#             */
/*   Updated: 2023/10/13 19:34:29 by ramoussa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo.h"

void abort_exit(t_philo *env, char *msg, int exit_code)
{
	(void)env;
	if (msg)
		ft_printf("%s\n", msg);
	exit(exit_code);
}

t_philo	*init(int argc, char **argv)
{
	t_philo *env;

	env = (t_philo *)malloc(sizeof(t_philo));
	env->num_of_philosophers = ft_atoi(argv[1]);
	if (env->num_of_philosophers < 1)
		abort_exit(env, "Number of philos less than 1", 0);
	env->time_to_die = ft_atoi(argv[2]);
	if (env->time_to_die < 0)
		abort_exit(env, "Time of die cannot be less than 0", 2);
	env->time_to_eat = ft_atoi(argv[3]);
	if (env->time_to_eat < 0)
		abort_exit(env, "Time to eat cannot be less than 0", 2);
	env->time_to_sleep = ft_atoi(argv[4]);
	if (env->time_to_sleep < 0)
		abort_exit(env, "Time to sleep cannot be less than 0", 2);
	if (argc == 6)
	{
		env->num_of_times_each_philosopher_must_eat = ft_atoi(argv[5]);
		if (env->num_of_times_each_philosopher_must_eat < 0)
			abort_exit(env, "Num of times to eat cannot be less than 0", 2);
	}
	return (env);
}

void	print_env(t_philo *env)
{
	ft_printf("Number of philosophers %d\n", env->num_of_philosophers);
	ft_printf("Time to eat %d\n", env->time_to_eat);
	ft_printf("Time to sleep %d\n", env->time_to_sleep);
	ft_printf("Time to die %d\n", env->time_to_die);
	ft_printf("Number of times each philosopher must eat %d\n", env->num_of_times_each_philosopher_must_eat);
}

time_t	time_now()
{
	struct timeval currentTime;

	gettimeofday(&currentTime, NULL);
	return ((currentTime.tv_sec * 1000) + (currentTime.tv_usec / 1000));
}

time_t	time_since(time_t since)
{
	return (time_now() - since);
}

void	time_sleep(unsigned int ms)
{
	usleep(ms * 1000);
}

int	main(int argc, char **argv)
{
	t_philo *env;
	// pthread_t th;
	if (argc < 5)
		return (0);
	env = init(argc, argv);
	print_env(env);
	time_t t = time_now();
	printf("%ld\n", t);
	time_sleep(1);
	printf("%ld\n", time_since(t));
	return (0);
}
