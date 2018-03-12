#ifndef MYREADKEY_H
#define MYREADKEY_H
#include "bigchars.h"
#include <termios.h>
#include <string.h>

// F5    F6     UP   DOWN LEFT RIGHT
// \E[[E \E[17~ \E[A \E[B \E[D \E[C

enum keys {
	F5,
	F6,
	UP,
	DOWN,
	LEFT,
	RIGHT
};

extern int x;
extern int y;

struct termios tty;
struct termios savetty;

int rk_readKey(enum keys *key);
int rk_mytermsave();
int rk_mytermrestore();
int rk_mytermregime(int regime, int vtime, int vmin, int echo, int sigint);

#endif