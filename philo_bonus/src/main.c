/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ramoussa <ramoussa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/06 17:11:56 by ramoussa          #+#    #+#             */
/*   Updated: 2023/11/03 14:52:38 by ramoussa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

int	main(int argc, char **argv)
{
	t_simulation	*env;

	if (argc < 5 || argc > 6)
		return (1);
	env = init_simulation(argc, argv);
	gettimeofday(&env->begin, NULL);
	if (setup_simulation(env))
		abort_exit(env, NULL, 0);
	if (begin_simulation(env))
		abort_exit(env, NULL, 1);
	abort_exit(env, NULL, 0);
	return (0);
}
