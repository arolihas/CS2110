/*
 * Exported with nin10kit v1.7
 * Invocation command was nin10kit --mode=3 --resize=240x160,240x160,240x160 screens splash.png win.png lose.png 
 * Time-stamp: Monday 11/13/2017, 00:30:24
 * 
 * Image Information
 * -----------------
 * splash.png 240@160
 * win.png 240@160
 * lose.png 240@160
 * 
 * All bug reports / feature requests are to be filed here https://github.com/TricksterGuy/nin10kit/issues
 */

#ifndef SCREENS_H
#define SCREENS_H

extern const unsigned short splash[38400];
#define SPLASH_SIZE 76800
#define SPLASH_LENGTH 38400
#define SPLASH_WIDTH 240
#define SPLASH_HEIGHT 160

extern const unsigned short win[38400];
#define WIN_SIZE 76800
#define WIN_LENGTH 38400
#define WIN_WIDTH 240
#define WIN_HEIGHT 160

extern const unsigned short lose[38400];
#define LOSE_SIZE 76800
#define LOSE_LENGTH 38400
#define LOSE_WIDTH 240
#define LOSE_HEIGHT 160

#endif

