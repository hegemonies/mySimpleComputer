#include "myReadKey.h"


int rk_readKey(enum keys *key)
{
	rk_mytermsave();
	tcgetattr (STDIN_FILENO, &tty);

	char buf[10] = { 0 };
	rk_mytermregime(1, 0, 1, 0, 0);
	int num = read(STDIN_FILENO, buf, 10);
	buf[num] = 0;
	// printf("buf = %s\n", buf);

	if (strcmp(buf, "\E[C") == 0) {
		printf("PRESS RIGHT\n");
	}
	if (strcmp(buf, "\E[D") == 0) {
		printf("PRESS LEFT\n");
	}
	if (strcmp(buf, "\E[A") == 0) {
		printf("PRESS UP\n");
	}
	if (strcmp(buf, "\E[B") == 0) {
		printf("PRESS DOWN\n");
	}
	if (strcmp(buf, "\E[[E") == 0) {
		printf("PRESS F5\n");
	}
	if (strcmp(buf, "\E[17~") == 0) {
		
	}

	//tcsetattr (0, TCSAFLUSH, &savetty);
	tcsetattr (0, TCSAFLUSH, &savetty);
	rk_mytermrestore();

	return 0;
}

int rk_mytermsave()
{
	savetty = tty;
	//tcsetattr (0, TCSAFLUSH, &tty);
	return 0;
}

int rk_mytermrestore()
{
	if (tcsetattr(STDIN_FILENO, TCSANOW, &savetty)) {
		return 1;
	}

	return 0;
}

int rk_mytermregime(int regime, int vtime, int vmin, int echo, int sigint)
{
	// struct termios tty;

	if (regime == 1) { // некононичный
		tty.c_lflag &= ~ICANON;

		if (echo == 1) {
			tty.c_lflag &= ~ECHO;
		} else if (echo == 0) {
			tty.c_lflag |= ECHO;
		} else {
			write(STDERR_FILENO, "Uncorrect argument ECHO in rk_mytermregime\n", 43);
			return -1;
		}

		if (sigint == 1) {
			tty.c_lflag &= ~ISIG;
		} else if (sigint == 0) {
			tty.c_lflag |= ISIG;
		} else {
			write(STDERR_FILENO, "Uncorrect argument SIGINT in rk_mytermregime\n", 43);
			return -1;
		}

		tty.c_cc[VMIN] = vmin;
		tty.c_cc[VTIME] = vtime;
	} else if (regime == 0) {// кононичный 
		tty.c_lflag |= ICANON;	
	} else {
		write(STDERR_FILENO, "Uncorrect argument REGIME in rk_mytermregime\n", 43);
		return -1;
	}

	tcsetattr (0, TCSANOW, &tty);

	return 0;
}