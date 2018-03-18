#include "myReadKey.h"


int rk_readKey(enum keys *key)
{
	rk_mytermsave();
	//tcgetattr(STDIN_FILENO, &tty);

	char buf[6] = { 0 };
	rk_mytermregime(1, 0, 1, 1, 1);

	read(STDIN_FILENO, buf, 6);

	if (strcmp(buf, "\E[C") == 0) {
		*key = RIGHT;
	} else if (strcmp(buf, "\E[D") == 0) {
		*key = LEFT;
	} else if (strcmp(buf, "\E[A") == 0) {
		*key = UP;
	} else if (strcmp(buf, "\E[B") == 0) {
		*key = DOWN;
	} else if (strcmp(buf, "\E[15~") == 0) {
		*key = F5;
	} else if (strcmp(buf, "\E[17~") == 0) {
		*key = F6;
	} else if (strcmp(buf, "-") == 0) {
		*key = MINUS;
	} else if (strcmp(buf, "+") == 0) {
		*key = PLUS;
	} else if (buf[0] >= 48 && buf[0] < 58) {
		*key = buf[0] - 48;
	} else if (buf[0] > 64 && buf[0] < 91) {
		*key = buf[0];
	} else if (buf[0] > 96 && buf[0] < 123) {
		*key = buf[0];
	} else {
		*key = OTHER;
	}
	
	rk_mytermrestore();

	return 0;
}

int rk_mytermsave()
{
	tcgetattr(STDIN_FILENO, &tty);
	savetty = tty;

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