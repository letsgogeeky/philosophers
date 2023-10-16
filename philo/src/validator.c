/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validator.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ramoussa <ramoussa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/14 13:34:13 by ramoussa          #+#    #+#             */
/*   Updated: 2023/10/14 17:15:53 by ramoussa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo.h"

int	ra_is_int(char *str)
{
	int	idx;

	idx = 0;
	if (!str)
		return (0);
	if (str[idx] == '-' && str[idx + 1])
		idx++;
	while (str[idx])
	{
		if (str[idx] < '0' || str[idx] > '9')
			return (0);
		idx++;
	}
	return (1);
}

int	ft_iswhitespace(char c)
{
	if (c == ' ' || c == '\t'
		|| c == '\n' || c == '\v'
		|| c == '\f' || c == '\r')
		return (1);
	return (0);
}

int	ft_atoi(const char *str)
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
