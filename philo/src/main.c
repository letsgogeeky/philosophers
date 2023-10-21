/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ramoussa <ramoussa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/06 17:11:56 by ramoussa          #+#    #+#             */
/*   Updated: 2023/10/21 21:28:18 by ramoussa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo.h"

void abort_exit(t_simulation *env, char *msg, int exit_code)
{
	(void)env;
	if (msg)
		printf("%s\n", msg);
	// FREE philos & forks
	exit(exit_code);
}

t_simulation	*init_simulation(int argc, char **argv)
{
	t_simulation *env;

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
	return (env);
}

void	print_env(t_simulation *env)
{
	printf("Number of philosophers %d\n", env->num_of_philosophers);
	printf("Time to eat %d\n", env->time_to_eat);
	printf("Time to sleep %d\n", env->time_to_sleep);
	printf("Time to die %d\n", env->time_to_die);
	printf("Number of times each philosopher must eat %d\n", env->meals_count);
}

time_t	time_now()
{
	struct timeval currentTime;

	gettimeofday(&currentTime, NULL);
	return ((currentTime.tv_sec * 1000) + (currentTime.tv_usec * 0.001));
}

time_t	time_since(time_t since)
{
	return (time_now() - since);
}

time_t	to_timestamp(struct timeval time)
{
	return ((time.tv_sec * 1000) + (time.tv_usec * 0.001));
}

void	time_sleep(int ms)
{
	struct timeval time;
	time_t	until;
	
	gettimeofday(&time, NULL);
	until = to_timestamp(time) + ms;
	if (ms > 5)
		usleep(((ms - 5) * 1000));
	gettimeofday(&time, NULL);
	while (to_timestamp(time) < until)
	{
		usleep(1000);
		gettimeofday(&time, NULL);
	}
}

int	has_death(t_simulation *env)
{
	int	idx;

	idx = 0;
	while (idx < env->num_of_philosophers)
	{
		pthread_mutex_lock(&env->philos[idx].starvation_mutex);
		if (env->philos[idx].died == 1)
		{
			pthread_mutex_unlock(&env->philos[idx].starvation_mutex);
			return (1);
		}
		pthread_mutex_unlock(&env->philos[idx].starvation_mutex);
		idx++;
	}
	return (0);
}
void	signal_terminate_all(t_simulation *env)
{
	int	idx;

	idx = 0;
	while (idx < env->num_of_philosophers)
	{
		pthread_mutex_lock(&env->philos[idx].starvation_mutex);
		env->philos[idx].died = 1;
		pthread_mutex_unlock(&env->philos[idx].starvation_mutex);
		idx++;
	}	
}

int	should_serve_more_meals(t_simulation *env)
{
	int	idx;

	idx = 0;
	while (idx < env->num_of_philosophers)
	{
		pthread_mutex_lock(&env->philos[idx].eating_mutex);
		if (env->philos[idx].meals_eaten < env->meals_count)
		{
			pthread_mutex_unlock(&env->philos[idx].eating_mutex);
			return (1);
		}
		pthread_mutex_unlock(&env->philos[idx].eating_mutex);
		idx++;
	}
	return (0);
}

void	*watcher_worker(void *simulation)
{
	t_simulation *env;
	
	env = (t_simulation *)simulation;
	while (1)
	{
		time_sleep(1);
		if (has_death(env))
		{
			signal_terminate_all(env);
			break;
		}
		if (should_serve_more_meals(env) == 0)
			break;
	}
	return (NULL);
}

void	log_state(t_simulation *env, t_philo *philo)
{
	int	time_arrow;

	time_arrow = time_since(to_timestamp(env->begin));
	pthread_mutex_lock(&env->logger_mutex);
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
	pthread_mutex_unlock(&env->logger_mutex);
}

int	is_dead(t_philo *philo)
{
	pthread_mutex_lock(&philo->starvation_mutex);
	if (philo->died == 1)
	{
		pthread_mutex_unlock(&philo->starvation_mutex);
		return (1);
	}
	pthread_mutex_unlock(&philo->starvation_mutex);
	return (0);
}
void	set_state(t_simulation *env, t_philo *philo, \
			enum e_philo_state status)
{
	philo->status = status;
	log_state(env, philo);
}

int	will_starve(t_simulation *env, t_philo *philo, int action_ms)
{
	if (time_since(to_timestamp(philo->last_meal_at)) + action_ms > env->time_to_die)
	{
		time_sleep(env->time_to_die - time_since(to_timestamp(philo->last_meal_at)));
		if (is_dead(philo))
			return (1);
		set_state(env, philo, DEAD);
		pthread_mutex_lock(&philo->starvation_mutex);
		philo->died = 1;
		pthread_mutex_unlock(&philo->starvation_mutex);
		return (1);
	}
	return (0);
}

int	ph_eat(t_simulation *env, t_philo *philo)
{
	philo->status = EAT;
	if (philo->number % 2 == 0)
	{
		philo->status = ACQUIRE;
		pthread_mutex_lock(philo->right_fork);
		log_state(env, philo);
		pthread_mutex_lock(philo->left_fork);
		log_state(env, philo);
		set_state(env, philo, EAT);
		gettimeofday(&philo->last_meal_at, NULL);
		if (will_starve(env, philo, env->time_to_eat))
		{
			pthread_mutex_unlock(philo->left_fork);
			pthread_mutex_unlock(philo->right_fork);
			return (1);
		}
		time_sleep(env->time_to_eat);
		pthread_mutex_unlock(philo->left_fork);
		pthread_mutex_unlock(philo->right_fork);
	}
	else
	{
		philo->status = ACQUIRE;
		pthread_mutex_lock(philo->left_fork);
		log_state(env, philo);
		pthread_mutex_lock(philo->right_fork);
		log_state(env, philo);
		set_state(env, philo, EAT);
		gettimeofday(&philo->last_meal_at, NULL);
		if (will_starve(env, philo, env->time_to_eat))
		{
			pthread_mutex_unlock(philo->right_fork);
			pthread_mutex_unlock(philo->left_fork);
			return (1);
		}
		time_sleep(env->time_to_eat);
		pthread_mutex_unlock(philo->right_fork);
		pthread_mutex_unlock(philo->left_fork);
	}
	if (is_dead(philo))
		return (1);
	return (0);
}

int	ph_sleep(t_simulation *env, t_philo *philo)
{
	set_state(env, philo, SLEEP);
	if (will_starve(env, philo, env->time_to_sleep))
		return (1);
	time_sleep(env->time_to_sleep);
	if (is_dead(philo))
		return (1);
	set_state(env, philo, THINK);
	return (0);
}

int	ph_think(t_simulation *env, t_philo *philo, int thinking_ms)
{
	if (will_starve(env, philo, thinking_ms))
		return (1);
	time_sleep(thinking_ms);
	if (is_dead(philo))
		return (1);
	return (0);
}

int	philo_lifecycle(t_simulation *env, t_philo *philo, int order)
{
	while (philo->meals_eaten < env->meals_count)
	{
		if (philo->number == order && ph_think(env, philo, env->time_to_eat))
			return (1);
		order++;
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
void	adjust_for_midlaunch_abort(t_simulation *env, pthread_mutex_t *to_destory1, \
			pthread_mutex_t *to_destroy2, int philos_initialized)
{
	if (to_destory1 != NULL)
		pthread_mutex_destroy(to_destory1);
	if (to_destroy2 != NULL)
		pthread_mutex_destroy(to_destory1);
	env->num_of_philosophers = philos_initialized;
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
void	populate_begin_time(t_simulation *env)
{
	int	idx;

	idx = 0;
	while (idx < env->num_of_philosophers)
	{
		env->philos[idx].last_meal_at = env->begin;
		idx++;
	}
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
