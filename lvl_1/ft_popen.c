/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_popen.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miguel-f <miguel-f@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/06 12:35:53 by miguel-f          #+#    #+#             */
/*   Updated: 2026/02/06 12:36:55 by miguel-f         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

int ft_popen(const char *file, char *const argv[], char type)
{
	int		pipefd[2];
	pid_t	my_pid;

	if (!file || !argv || (type != 'r' && type != 'w'))
		return (-1);
	if (pipe(pipefd) == -1)
		return (-1);
	my_pid = fork();
	if (my_pid == -1)
	{
		close(pipefd[0]);
		close(pipefd[1]);
		return (-1);
	}
	if (my_pid == 0)
	{
		if (type == 'r')
			dup2(pipefd[1], 1);
		else
			dup2(pipefd[0], 0);
		close(pipefd[0]);
		close(pipefd[1]);
		execvp(file, argv);
		exit(1);
	}
		if (type == 'r')
		{
			close(pipefd[1]);
			return (pipefd[0]);
		}
		else
		{
			close(pipefd[0]);
			return (pipefd[1]);
	}
}