/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgrochow <staafnet@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/02 21:11:51 by rgrochow          #+#    #+#             */
/*   Updated: 2024/08/11 18:51:03 by rgrochow         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <signal.h>
#include <unistd.h>

int	g_bit_received;

void	bit_confirmation(int sig)
{
	(void)sig;
	g_bit_received = 1;
}

void	send_char(char c, int pid)
{
	int	bit;

	bit = 0;
	while (bit < 8)
	{
		if ((c >> bit) & 1)
			kill(pid, SIGUSR2);
		else
			kill(pid, SIGUSR1);
		while (!g_bit_received)
			pause();
		g_bit_received = 0;
		bit++;
	}
}

void	confirm_signal(int sig)
{
	(void)sig;
	write(1, "Message has been received by server!\n", 38);
}

int	ft_atoi(const char *str)
{
	int	result;
	int	sign;

	result = 0;
	sign = 1;
	while (*str == ' ' || (*str >= 9 && *str <= 13))
		str++;
	if (*str == '-')
	{
		sign = -1;
		str++;
	}
	else if (*str == '+')
		str++;
	while (*str >= '0' && *str <= '9')
	{
		result = result * 10 + (*str - '0');
		str++;
	}
	return (result * sign);
}

int	main(int argc, char *argv[])
{
	int	pid;
	int	i;

	if (argc != 3)
		return (1);
	pid = ft_atoi(argv[1]);
	signal(SIGUSR2, bit_confirmation);
	i = 0;
	while (argv[2][i] != '\0')
	{
		send_char(argv[2][i], pid);
		i++;
	}
	send_char('\n', pid);
	signal(SIGUSR1, confirm_signal);
	send_char('\0', pid);
	return (0);
}
