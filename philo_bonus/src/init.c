/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ramoussa <ramoussa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/14 13:34:13 by ramoussa          #+#    #+#             */
/*   Updated: 2023/11/03 16:13:17 by ramoussa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

static int	ft_iswhitespace(char c)
{
	if (c == ' ' || c == '\t'
		|| c == '\n' || c == '\v'
		|| c == '\f' || c == '\r')
		return (1);
	return (0);
}

static int	ft_atoi(const char *str)
{
	int	result;
	int	sign;

	result = 0;
	sign = 1;
	while (ft_iswhitespace(*str))
		str++;
	if (*str != '\0' && (*str == '-' || *str == '+'))
	{
		if (*str == '-')
			sign *= -1;
		str++;
	}
	while (*str != '\0' && *str >= '0' && *str <= '9')
	{
		result = result * 10;
		result += *str - 48;
		str++;
	}
	result *= sign;
	return (result);
}

static void	allocate_philos(t_simulation *env)
{
	env->philos = ft_calloc(env->num_of_philosophers, sizeof(t_philo));
	if (!env->philos)
		abort_exit(env, NULL, 1);
	env->forks = NULL;
	env->death = NULL;
}

t_simulation	*init_simulation(int argc, char **argv)
{
	t_simulation	*env;

	env = (t_simulation *)malloc(sizeof(t_simulation));
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
	env->meals_count = 2147483640;
	if (argc == 6)
	{
		env->meals_count = ft_atoi(argv[5]);
		if (env->meals_count < 0)
			abort_exit(env, "Num of times to eat cannot be less than 0", 2);
	}
	allocate_philos(env);
	return (env);
}
