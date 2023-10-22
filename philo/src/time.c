/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ramoussa <ramoussa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/22 20:12:40 by ramoussa          #+#    #+#             */
/*   Updated: 2023/10/22 20:48:25 by ramoussa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

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