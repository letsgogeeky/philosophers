/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ramoussa <ramoussa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/06 17:11:53 by ramoussa          #+#    #+#             */
/*   Updated: 2023/10/16 14:30:33 by ramoussa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <stdio.h>
# include <unistd.h>
# include <pthread.h>
# include <sys/time.h>
# include <stdlib.h>

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
	pthread_mutex_t		*left_fork;
	pthread_mutex_t		*right_fork;
	struct	timeval		last_meal_at;
	pthread_mutex_t		eating_mutex;
	pthread_mutex_t		starvation_mutex;
	pthread_t			worker;
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
	pthread_mutex_t	*forks;
	pthread_mutex_t	logger_mutex;
}	t_simulation;

typedef struct s_philo_worker_arg
{
	t_philo			*philo;
	t_simulation	*env;
}	t_philo_worker_arg;


int		ft_atoi(const char *str);
int		ra_is_int(char *str);
void	*ft_calloc(unsigned int count, unsigned int size);

#endif
