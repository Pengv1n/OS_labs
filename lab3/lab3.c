#include "stdio.h"
#include "string.h"
#include "stdlib.h"
#include "sys/wait.h"
#include "signal.h"
#include "unistd.h"

int	flag;

void	ft_error(char *str)
{
	printf("Error: %s\n", str);
	exit(EXIT_FAILURE);
}

void	ft_change_led(int state)
{
	if (state == 0)
		write(1, "0\n", 2);
	else
		write(1, "1\n", 2);
}

void	ft_init(int	sig)
{
	ft_change_led(0);
	exit(0);
}

void	ft_swap_value(int sig)
{
	flag = 1;
}

int	main(int argc, char **argv)
{
	pid_t	pid;
	int		fd[2];
	int		status;
	int		T;

	if (argc != 2)
		ft_error("Invalid number arguments");
	if (pipe(fd) == -1)
		return (0);
	if (!(T = atoi(argv[1])))
		ft_error("argv[1] not number");
	pid = fork();
	if (pid == -1)
		ft_error("fork()");
	if (pid == 0)
	{
		int	state;

		flag = 0;
		state = 0;
		close(fd[1]);
		signal(SIGINT, ft_init);
		signal(SIGUSR1, ft_swap_value);
		while (1)
		{
			if (flag)
			{
				if (read(fd[0], &T, sizeof(int)) <= 0)
					return (0);
				flag = 0;
			}
			ft_change_led(state);
			state = !state;
			sleep(T);
		}
	}
	close(fd[0]);
	while (1)
	{
		scanf("%d", &T);
		if (T <= 0)
		{
			ft_error("T <= 0");
			break;
		}
		write(fd[1], &T, sizeof(int));
		kill(pid, SIGUSR1);
	}
	waitpid(pid, &status, 0);
	close(fd[0]);
	close(fd[1]);
	if (WIFEXITED(status) != 0)
	{
		ft_error("Error: status");
	}
	return (0);
}