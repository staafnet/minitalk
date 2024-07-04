/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgrochow <staafnet@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/02 21:10:51 by rgrochow          #+#    #+#             */
/*   Updated: 2024/07/04 18:48:08 by rgrochow         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <signal.h>
#include <unistd.h>

void	get_char(int sig, siginfo_t *info, void *context)
{
	static int	bit;
	static int	c;

	(void)context;
	if (sig == SIGUSR2)
		c |= (1 << bit);
	bit++;
	if (bit == 8)
	{
		if (c == 0)
			kill(info->si_pid, SIGUSR1);
		write(1, &c, 1);
		bit = 0;
		c = 0;
	}
}

void	write_pid(int pid)
{
	char	buffer[12];
	int		i;

	i = 10;
	if (pid == 0)
		buffer[i--] = '0';
	while (pid > 0)
	{
		buffer[i--] = (pid % 10) + '0';
		pid /= 10;
	}
	write(1, "Server PID: ", 12);
	write(1, &buffer[i + 1], 10 - i);
	write(1, "\n", 1);
}

int	main(void)
{
	int					pid;
	struct sigaction	sa;

	pid = getpid();
	write_pid(pid);
	sa.sa_sigaction = get_char;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_SIGINFO;
	while (1)
	{
		sigaction(SIGUSR1, &sa, NULL);
		sigaction(SIGUSR2, &sa, NULL);
		pause();
	}
	return (0);
}
