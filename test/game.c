#include <stdio.h>
#include <stdlib.h>
#include "myLib.h"
#include "screens.h"
#include "projectile.h"

int main() {
	
	REG_DISPCTL = MODE3 | BG2_ENABLE;
	enum GBAState state = START;

	int bWidth = 18; //initializing brick sizes
	int bHeight = 4;
	char buffer[40]; //for strings with levels and scores
	char bufferLev[40];

	while(1) {
		waitForVblank();
		switch(state) {

			case START: 
				drawImage(0,0,SPLASH_WIDTH,SPLASH_HEIGHT,splash);
				Brick bricksL1[20]; //initializing bricks for Level 1
				for(int i = 0; i<20; i++) {
					bricksL1[i].width = bWidth;
					bricksL1[i].height = bHeight;
					bricksL1[i].bottom = i/10 * bricksL1[i].height + (i/10 + 20); //placing bricks in rows of 10 in center
					bricksL1[i].left = bricksL1[i].width * (i % 10) + (i % 10 + 20); //centering bricks
					bricksL1[i].top = bricksL1[i].bottom + bHeight;
					bricksL1[i].right = bricksL1[i].left + bWidth;
					bricksL1[i].color = BLUE;
				}
				//initializes paddle and ball position
				//paddle
				int paddleH = 5; //height of paddle
				int paddleW = 20; //width of paddle
				int pRow = 140; //y position of paddle
				int pCol = 120 - (paddleW/2); //x position of paddle
				int oldpRow = pRow;
				int oldpCol = pCol;
				int pMove = 2;
				//ball
				int rMove = 1; //how fast ball moves in y
				int cMove = 1; //how fast ball moves in x
				int ballSize = 5; 
				int bRow = pRow - paddleH; //ball is on paddle in beginning
				int bCol = 120 - ballSize/2; //ball is in middle of paddle
				int oldBrow = bRow;
				int oldBcol = bCol;
				int score = 0; //how many bricks you've hit
				int level = 1; 
				if(KEY_DOWN_NOW(BUTTON_START)) {
					fillScreen(BLACK);
					state = LEVEL1;
				}
				break;

			case LEVEL1:
				for(int i = 0; i < 20; i++) { //loop through draw bricks as rectangles
					drawRect(bricksL1[i].bottom,bricksL1[i].left,bricksL1[i].height,bricksL1[i].width,bricksL1[i].color);
				}
				drawRect(150, 5, 10, 60, BLACK); // fill in old score with black background
				sprintf(buffer, "Score: %d", score); 
				drawString(150, 5, buffer, WHITE); //display score
				sprintf(bufferLev, "Level: %d", level); 
				drawString(150, 100, bufferLev, WHITE); //display level
				drawRect(oldBrow, oldBcol, ballSize, ballSize, BLACK); // fill in the old position of ball with black background
				drawImage(bRow, bCol, ballSize, ballSize, projectile); // draw image of ball
				drawRect(oldpRow, oldpCol, 5, 20, BLACK); // fill in the old position of the paddle with black background
				drawRect(pRow, pCol, paddleH,paddleW, CYAN); // draw rectangle as paddle
				
				oldBcol = bCol;
				oldBrow = bRow;
				oldpRow = pRow;
				oldpCol = pCol;
				
				if(KEY_DOWN_NOW(BUTTON_RIGHT)) {
					pCol += pMove;
				}

				if(KEY_DOWN_NOW(BUTTON_LEFT)) {
					pCol -= pMove;
				}

				if(pCol < 0) {
					pCol = 0;
				}

				if(pCol > 240 - paddleW) {
					pCol = 240 - paddleW;
				}

				bRow += rMove;
				bCol += cMove;

				if(bRow < 0) {
					bRow = 0;
					rMove = -rMove;
				}

				if(bCol < 0) {
					bCol = 0;
					cMove = -cMove;
				}

				if(bCol > 240 - ballSize) {
					bCol = 240 - ballSize;
					cMove = -cMove;
				}
				if(KEY_DOWN_NOW(BUTTON_SELECT)) {
					state = START;
					break;
				}

				if (bRow + ballSize/2 >= pRow + paddleH/2) {
					state = LOSE;
					break;
				}

				if ((bRow + ballSize/2) == (pRow - paddleH / 2) && bCol + ballSize >= pCol && bCol <= pCol + paddleW) {
					rMove = -rMove;
				}

				for (int i = 0; i < 20; i++) { // iterate through bricks to see if ball is in contact with any of them
					int hitTop = (bRow == bricksL1[i].top) && (bCol + ballSize >= bricksL1[i].left) && (bCol < bricksL1[i].right);
					int hitBottom = (bRow + ballSize == bricksL1[i].bottom) && (bCol + ballSize >= bricksL1[i].left) && (bCol < bricksL1[i].right);
					// make sure that a brick is still in the position being checked by checking it's not black
					if ((bricksL1[i].color != BLACK) && (hitBottom || hitTop)) {
						if (hitBottom || hitTop) { // if the ball hit the top or bottom of a brick, bounce off of it like a screen edge
							rMove = -rMove;
						}
						bricksL1[i].color = BLACK; // delete brick by making it black background
						score++; // increment score
						if (score == 20) { // if all bricks have been hit, go to next level 
							fillScreen(BLACK);
							state = LEVEL2CUT;
						}
						break;
					}
				}
				break;

			case LEVEL2CUT:
				//initializes paddle and ball position
				//paddle
				paddleH = 5;
				paddleW = 20;
				pRow = 140;
				pCol = 120 - (paddleW/2);
				oldpRow = pRow;
				oldpCol = pCol;
				pMove = 2;
				//ball
				rMove = 1;
				cMove = 1;
				ballSize = 5; 
				bRow = pRow - paddleH;
				bCol = 120 - ballSize/2;
				oldBrow = bRow;
				oldBcol = bCol;
				score = 0;
				level = 2;
				Brick bricksL2[10];

				for(int i = 0; i<10; i++) {
					bricksL2[i].width = bWidth;
					bricksL2[i].height = bHeight;
					bricksL2[i].bottom = i/10 * bricksL2[i].height + (i/10 + 30);
					bricksL2[i].left = bricksL2[i].width * (i % 10) + (i % 10 + 20);
					bricksL2[i].top = bricksL2[i].bottom + bHeight;
					bricksL2[i].right = bricksL2[i].left + bWidth;
					bricksL2[i].color = ORANGE;
				}
				if(KEY_DOWN_NOW(BUTTON_START)) {
					fillScreen(BLACK);
					state = LEVEL2;
				}

			case LEVEL2:

				for(int i = 0; i < 10; i++) {
					drawRect(bricksL2[i].bottom,bricksL2[i].left,bricksL2[i].height,bricksL2[i].width,bricksL2[i].color);
				}
				drawRect(150, 5, 10, 60, BLACK); // fill in old position with background color
				sprintf(buffer, "Score: %d", score); // display score
				drawString(150, 5, buffer, WHITE); // draw the score string at bottom left of screen 
				sprintf(bufferLev, "Level: %d", level); // display level
				drawString(150, 100, bufferLev, WHITE);
				drawRect(oldBrow, oldBcol, ballSize, ballSize, BLACK); // fill in the old position of ball with bgcolor
				drawImage(bRow, bCol, ballSize, ballSize, projectile);
				drawRect(oldpRow, oldpCol, 5, 20, BLACK); // fill in the old position of the paddle with bgcolor
				drawRect(pRow, pCol, paddleH,paddleW, CYAN); // draw in the paddle
				
				oldBcol = bCol;
				oldBrow = bRow;
				oldpRow = pRow;
				oldpCol = pCol;
				
				if(KEY_DOWN_NOW(BUTTON_RIGHT)) {
					pCol += pMove;
				}

				if(KEY_DOWN_NOW(BUTTON_LEFT)) {
					pCol -= pMove;
				}

				if(pCol < 0) {
					pCol = 0;
				}

				if(pCol > 240 - paddleW) {
					pCol = 240 - paddleW;
				}

				bRow += rMove;
				bCol += cMove;

				if(bRow < 0) {
					bRow = 0;
					rMove = -rMove;
				}

				if(bCol < 0) {
					bCol = 0;
					cMove = -cMove;
				}

				if(bCol > 240 - ballSize) {
					bCol = 240 - ballSize;
					cMove = -cMove;
				}
				if(KEY_DOWN_NOW(BUTTON_SELECT)) {
					state = START;
					break;
				}

				if (bRow + ballSize/2 >= pRow + paddleH/2) {
					state = LOSE;
					break;
				}

				if ((bRow + ballSize/2) == (pRow - paddleH / 2) && bCol + ballSize >= pCol && bCol <= pCol + paddleW) {
					rMove = -rMove;
				}

				for (int i = 0; i < 10; i++) { // iterate through bricks to see if ball is in contact with any of them
					int hitTop = (bRow == bricksL2[i].top) && (bCol + ballSize >= bricksL2[i].left) && (bCol < bricksL2[i].right);
					int hitBottom = (bRow + ballSize == bricksL2[i].bottom) && (bCol + ballSize >= bricksL2[i].left) && (bCol < bricksL2[i].right);
					// make sure that a brick is still in the position being checked by checking the brick's color
					if ((bricksL2[i].color != BLACK) && (hitBottom || hitTop)) {
						if (hitBottom || hitTop) { // if the ball hit the top or bottom of a brick, bounce off it
							rMove = -rMove;
						}
						bricksL2[i].color = BLACK; // deletes brick
						score++; // increment score
						if (score == 10) { // if all bricks have been hit, go to next level 
							fillScreen(BLACK);
							state = LEVEL3CUT;
						}
						break;
					}
				}
				break;

			case LEVEL3CUT:
				//initializes paddle and ball position
				//paddle
				paddleH = 5;
				paddleW = 20;
				pRow = 140;
				pCol = 120 - (paddleW/2);
				oldpRow = pRow;
				oldpCol = pCol;
				pMove = 2;
				//ball
				rMove = 1;
				cMove = 1;
				ballSize = 5; 
				bRow = pRow - paddleH;
				bCol = 120 - ballSize/2;
				oldBrow = bRow;
				oldBcol = bCol;
				score = 0;
				level = 3;
				Brick bricksL3[10];
				for(int i = 0; i<10; i++) {
					bricksL3[i].width = bWidth;
					bricksL3[i].height = bHeight;
					bricksL3[i].bottom = i/10 * bricksL2[i].height + (i/10 + 10);
					bricksL3[i].left = bricksL2[i].width * (i % 10) + (i % 10);
					bricksL3[i].right = bricksL3[i].left + bWidth;
					bricksL3[i].top = bricksL3[i].bottom + bHeight;
					bricksL3[i].color = YELLOW;
				}
				if(KEY_DOWN_NOW(BUTTON_START)) {
					fillScreen(BLACK);
					state = LEVEL3;
				}

			case LEVEL3:

				for(int i = 0; i < 10; i++) {
					drawRect(bricksL3[i].bottom,bricksL3[i].left,bricksL3[i].height,bricksL3[i].width,bricksL3[i].color);
				}
				drawRect(150, 5, 10, 60, BLACK); // fill in old position with background color
				sprintf(buffer, "Score: %d", score); // display score
				drawString(150, 5, buffer, WHITE); // draw the score string at bottom left of screen 
				sprintf(bufferLev, "Level: %d", level); // display score
				drawString(150, 100, bufferLev, WHITE);
				drawRect(oldBrow, oldBcol, ballSize, ballSize, BLACK); // fill in the old position of ball with bgcolor
				drawImage(bRow, bCol, ballSize, ballSize, projectile);
				drawRect(oldpRow, oldpCol, 5, 20, BLACK); // fill in the old position of the paddle with bgcolor
				drawRect(pRow, pCol, paddleH,paddleW, CYAN); // draw in the paddle
				
				oldBcol = bCol;
				oldBrow = bRow;
				oldpRow = pRow;
				oldpCol = pCol;
				
				if(KEY_DOWN_NOW(BUTTON_RIGHT)) {
					pCol += pMove;
				}

				if(KEY_DOWN_NOW(BUTTON_LEFT)) {
					pCol -= pMove;
				}

				if(pCol < 0) {
					pCol = 0;
				}

				if(pCol > 240 - paddleW) {
					pCol = 240 - paddleW;
				}

				bRow += rMove;
				bCol += cMove;

				if(bRow < 0) {
					bRow = 0;
					rMove = -rMove;
				}

				if(bCol < 0) {
					bCol = 0;
					cMove = -cMove;
				}

				if(bCol > 240 - ballSize) {
					bCol = 240 - ballSize;
					cMove = -cMove;
				}
				if(KEY_DOWN_NOW(BUTTON_SELECT)) {
					state = START;
					break;
				}

				if (bRow + ballSize/2 >= pRow + paddleH/2) {
					state = LOSE;
					break;
				}

				if ((bRow + ballSize/2) == (pRow - paddleH / 2) && bCol + ballSize >= pCol && bCol <= pCol + paddleW) {
					rMove = -rMove;
				}

				for (int i = 0; i < 10; i++) { // iterate through bricks to see if ball is in contact with any of them
					int hitTop = (bRow == bricksL3[i].top) && (bCol + ballSize >= bricksL3[i].left) && (bCol < bricksL3[i].right);
					int hitBottom = (bRow + ballSize == bricksL3[i].bottom) && (bCol + ballSize >= bricksL3[i].left) && (bCol < bricksL3[i].right);
					// make sure that a brick is still in the position being checked by checking the brick's color
					if ((bricksL3[i].color != BLACK) && (hitBottom || hitTop)) {
						if (hitBottom || hitTop) { // if the ball hit the top or bottom of a brick, change its rdel
							rMove = -rMove;
						}
						bricksL3[i].color = BLACK; // "delete brick"
						score++; // increment score
						if (score == 10) { // if all bricks have been hit, go to next level 
							state = WON;
						}
						break;
					}
				}
				break;

			case LOSE:

				drawImage(0,0,LOSE_WIDTH,LOSE_HEIGHT,lose);
				if(KEY_DOWN_NOW(BUTTON_SELECT)) {
					state = START;
				}
				break;

			case WON:

				drawImage(0,0,WIN_WIDTH,WIN_HEIGHT,win);
				if(KEY_DOWN_NOW(BUTTON_SELECT)) {
					state = START;
				}
				break;
				
		}
	}
}