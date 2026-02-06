/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_popen.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miguel-f <miguel-f@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/06 12:35:53 by miguel-f          #+#    #+#             */
/*   Updated: 2026/02/06 13:30:12 by miguel-f         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

/*
** FT_POPEN - Abre un pipe hacia/desde un proceso hijo
**
** Parámetros:
**   - file: nombre del ejecutable a lanzar
**   - argv: argumentos para el ejecutable (argv[0] debe ser el nombre del programa)
**   - type: 'r' para leer del comando, 'w' para escribir al comando
**
** Funcionamiento:
**   1. Valida parámetros (file, argv, type válido)
**   2. Crea un pipe (pipefd[0] = lectura, pipefd[1] = escritura)
**   3. Hace fork() para crear un proceso hijo
**   4. En el proceso HIJO:
**      - Si type='r': redirige STDOUT (fd 1) al pipe de escritura (pipefd[1])
**                     para que el padre pueda LEER la salida del comando
**      - Si type='w': redirige STDIN (fd 0) al pipe de lectura (pipefd[0])
**                     para que el padre pueda ESCRIBIR al comando
**      - Cierra ambos extremos del pipe (ya duplicados)
**      - Ejecuta el comando con execvp()
**   5. En el proceso PADRE:
**      - Si type='r': cierra pipefd[1] y retorna pipefd[0] (para leer)
**      - Si type='w': cierra pipefd[0] y retorna pipefd[1] (para escribir)
**
** Retorna:
**   - File descriptor del pipe (para leer o escribir según type)
**   - -1 en caso de error
*/
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