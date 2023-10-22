/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ramoussa <ramoussa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/06 17:11:56 by ramoussa          #+#    #+#             */
/*   Updated: 2023/10/22 21:45:35 by ramoussa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo.h"

int	philo_lifecycle(t_simulation *env, t_philo *philo, int order)
{
	while (philo->meals_eaten < env->meals_count)
	{
		if (philo->number == order && order++ && ph_think(env, philo, env->time_to_eat))
			return (1);
		if (ph_eat(env, philo))
			return (1);
		if (ph_sleep(env, philo))
			return (1);
		if (env->time_to_eat > env->time_to_sleep && \
			ph_think(env, philo, env->time_to_eat - env->time_to_sleep))
			return (1);
		if (order && order % 2 == 1 && order >= env->num_of_philosophers)
			order = 1;
		else if (order && order % 2 == 0 && order >= env->num_of_philosophers)
			order = 2;
		else if (order)
			order += 2;
		pthread_mutex_lock(&philo->eating_mutex);
		philo->meals_eaten++;
		pthread_mutex_unlock(&philo->eating_mutex);
	}
	return (0);
}

void	*philo_worker(void *arg)
{
	t_philo_worker_arg *env_philo;
	int		order;
	
	env_philo = (t_philo_worker_arg *)arg;
	order = 1;
	if (env_philo->env->num_of_philosophers % 2 == 0)
		order = 0;
	if (env_philo->philo->number == order)
		log_state(env_philo->env, env_philo->philo);
	else if (env_philo->philo->meals_eaten < env_philo->env->meals_count && \
		env_philo->philo->number % 2 == 0)
	{
		log_state(env_philo->env, env_philo->philo);
		if (ph_think(env_philo->env, env_philo->philo, env_philo->env->time_to_eat))
			return (free(env_philo), NULL);
		if (order)
			order++;
	}
	philo_lifecycle(env_philo->env, env_philo->philo, order);
	return (free(env_philo), NULL);
}
t_philo_worker_arg	*copy_philo_worker_arg(t_philo_worker_arg arg)
{
	t_philo_worker_arg *worker_arg;
	
	worker_arg = (t_philo_worker_arg *)malloc(sizeof(t_philo_worker_arg));
	worker_arg->env = arg.env;
	worker_arg->philo = arg.philo;
	return (worker_arg);
}

int	dispatch_philosophers(t_simulation *env)
{
	int	idx;
	t_philo_worker_arg	worker_arg;
	
	idx = 0;
	while (idx < env->num_of_philosophers)
	{
		worker_arg.env = env;
		worker_arg.philo = &env->philos[idx];
		if (pthread_create(&env->philos[idx].worker, NULL, philo_worker, \
				copy_philo_worker_arg(worker_arg)))
			return (1); // FREE EVERYTHING
		idx++;
	}
	return (0);
}

int	seat_philos(t_simulation *env)
{
	int	idx;

	idx = 0;
	while (idx < env->num_of_philosophers)
	{
		env->philos[idx].number = idx + 1;
		env->philos[idx].status = THINK;
		env->philos[idx].meals_eaten = 0;
		// env->philos[idx].last_meal_at = env->begin;
		if (pthread_mutex_init(&env->forks[idx], NULL))
			return (adjust_for_midlaunch_abort(env, NULL, NULL, idx - 1), 1); // FREE ALL
		if (pthread_mutex_init(&env->philos[idx].eating_mutex, NULL))
			return (adjust_for_midlaunch_abort(env, &env->forks[idx], NULL, idx - 1), 1); // FREE ALL
		if (pthread_mutex_init(&env->philos[idx].starvation_mutex, NULL))
			return (adjust_for_midlaunch_abort(env, &env->forks[idx], &env->philos[idx].eating_mutex, idx - 1), 1); // FREE ALL
		env->philos[idx].right_fork = &env->forks[idx];
		if (idx >= 1)
			env->philos[idx - 1].left_fork = &env->forks[idx];
		idx++;
	}
	env->philos[idx - 1].left_fork = &env->forks[0];
	return (0);
}

int	build_table(t_simulation *env)
{
	env->philos = ft_calloc(env->num_of_philosophers, sizeof(t_philo));
	if (!env->philos)
		return (1);
	env->forks = ft_calloc(env->num_of_philosophers, sizeof(pthread_mutex_t));
	if (!env->forks)
		return (1); // remember to FREE philos if this returns
	if (pthread_mutex_init(&env->logger_mutex, NULL))
		return (1); // remember to FREE philos & forks if this fails
	if (seat_philos(env))
		return (1); // remember to FREE ALL and Destroy mutexes
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

void	sad_philo(t_simulation *env)
{
	env->philos[0].status = ACQUIRE;
	log_state(env, &env->philos[0]);
	time_sleep(env->time_to_die);
	env->philos[0].status = DEAD;
	log_state(env, &env->philos[0]);
	// TODO: REMEMBER TO FREE EVERYTHING ON THE MAIN CALLER!
}

int	begin_simulation(t_simulation *env)
{
	pthread_t	watcher;

	if (pthread_create(&watcher, NULL, watcher_worker, env))
		return (1);
	if (dispatch_philosophers(env))
		return (1);
	pthread_join(watcher, NULL);
	join_all_threads(env);
	return (0);
}

int	setup_simulation(t_simulation *env)
{
	env->bigbang_at = time_now();
	if(build_table(env))
		return (1);	
	if (env->num_of_philosophers == 1)
	{
		sad_philo(env);
		return (1);
	}
	return (0);
}


int	main(int argc, char **argv)
{
	t_simulation *env;
	// pthread_t th;
	if (argc < 5 || argc > 6)
		return (1);
	env = init_simulation(argc, argv);
	print_env(env);
	time_t t = time_now();
	printf("%ld\n", t);
	if (setup_simulation(env))
		return (1); // TODO: FREE EVERYTHING AND EXIT
	gettimeofday(&env->begin, NULL);
	populate_begin_time(env);
	if (begin_simulation(env))
		return (1); // TODO: FREE EVERYTHING AND EXIT
	printf("%ld\n", time_since(t));
	// TODO: FREE EVERYTHINGGGGG!!
	return (0);
}
