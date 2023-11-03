/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_bonus.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ramoussa <ramoussa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/06 17:11:53 by ramoussa          #+#    #+#             */
/*   Updated: 2023/11/03 16:12:41 by ramoussa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_BONUS_H
# define PHILO_BONUS_H

# include <stdio.h>
# include <unistd.h>
# include <pthread.h>
# include <sys/time.h>
# include <stdlib.h>
# include <semaphore.h>
# include <fcntl.h>
# include <sys/wait.h>

enum	e_philo_state
{
	EAT,
	SLEEP,
	THINK,
	DEAD,
	ACQUIRE,
};

typedef struct s_philo
{
	int					number;
	int					meals_eaten;
	enum e_philo_state	status;
	struct timeval		last_meal_at;
	int					died;
}	t_philo;

typedef struct s_simulation
{
	int				num_of_philosophers;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				meals_count;
	time_t			bigbang_at;
	time_t			table_ready_at;
	struct timeval	begin;
	t_philo			*philos;
	sem_t			*forks;
	sem_t			*logger_sem;
	sem_t			*death;
}	t_simulation;

typedef struct s_philo_worker_arg
{
	t_philo			*philo;
	t_simulation	*env;
}	t_philo_worker_arg;

void				*ft_calloc(unsigned int count, unsigned int size);
t_philo_worker_arg	*copy_philo_worker_arg(t_philo_worker_arg arg);
void				log_state(t_simulation *env, t_philo *philo);
void				print_env(t_simulation *env);

time_t				time_now(void);
time_t				time_since(time_t since);
time_t				to_timestamp(struct timeval time);
void				time_sleep(int ms);

void				populate_begin_time(t_simulation *env);

t_simulation		*init_simulation(int argc, char **argv);
void				allocate_philos_and_forks(t_simulation *env);

void				sad_philo(t_simulation *env);
int					philo_worker(t_simulation *env, t_philo *philo);
int					setup_simulation(t_simulation *env);
int					begin_simulation(t_simulation *env);

int					ph_eat(t_simulation *env, t_philo *philo);
int					ph_think(t_simulation *env, t_philo *philo, \
						int thinking_ms);
int					ph_sleep(t_simulation *env, t_philo *philo);

void				set_state(t_simulation *env, t_philo *philo, \
						enum e_philo_state status);
int					has_death(void);
int					will_starve(t_simulation *env, t_philo *philo, \
						int action_ms);

void				adjust_for_midlaunch_abort(t_simulation *env, \
						pthread_mutex_t *to_destory1, \
						pthread_mutex_t *to_destroy2, int philos_initialized);
void				abort_exit(t_simulation *env, char *msg, int exit_code);

#endif
