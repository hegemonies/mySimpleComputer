#include "myReadKey.h"
#include <termios.h>
#include <string.h>

int rk_readKey(enum keys *key)
{
	struct termios tty;
	tcgetattr (0, &tty);
	//struct termios savetty;
	//savetty = tty;
	tty.c_lflag &= ~(ICANON | ECHO | ISIG);	
	tty.c_cc[VMIN] = 1;
	//tty.c_cc[VTIME] = 0;
	//tcsetattr (0, TCSAFLUSH, &tty);

	char buf[10] = { 0 };
	int num = read(STDIN_FILENO, buf, 10);
	printf("amount reading words is %d\n", num);
	buf[num] = 0;
	// printf("buf = %s\n", buf);


	printf("buf = \n");
	for (int i = 0; buf[i] != 0; i++)
		printf("%c", buf[i]);

	if (strcmp(buf, "\E[C") == 0) {
		printf("PRESS RIGHT\n");
	}
	if (strcmp(buf, buttoms[LEFT]) == 0) {
		printf("PRESS LEFT\n");
	}
	if (strcmp(buf, buttoms[UP]) == 0) {
		printf("PRESS UP\n");
	}
	if (strcmp(buf, buttoms[DOWN]) == 0) {
		printf("PRESS DOWN\n");
	}

	//tcsetattr (0, TCSAFLUSH, &savetty);

	return 0;
}

int rk_mytermsave()
{


	//tcsetattr (0, TCSAFLUSH, &tty);
	return 0;
}
// int rk_mytermrestore();

int rk_mytermregime(int regime, int vtime, int vmin, int echo, int sigint)
{
	struct termios tty;
/*
	if (regime == 0) { //некононичный
		tty.c_lflag &= ~(ICANON | ECHO | ISIG);	
	} else if (regime == 1) {// кононичный 
		tty.c_lflag &= ~(ICANON | ECHO | ISIG);	
	}
*/
	tty.c_cc[VMIN] = vmin;
	tty.c_cc[VTIME] = vtime;
	tcsetattr (0, TCSAFLUSH, &tty);


	tcsetattr (0, TCSAFLUSH, &savetty);
	return 0;
}