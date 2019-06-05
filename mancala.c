#include <ncurses.h>
#include <stdlib.h>

int max_x = 0, max_y = 0;
int board_x = 43, board_y = 5;
int p1[7];
int p2[7];
int ii = 0;
bool p1_turn = 1;
bool running = 1;
int lastchar = 0;

void initBoard(){
	for(int i = 0; i < 6; i++){
		p1[i] = 4;
		p2[i] = 4;
	}
	p1[6] = 0;
	p2[6] = 0;
}


void printBoard(int x, int y){

        mvprintw(y, x,     "+-----------------------------------------+\n");
        mvprintw(y + 1, x, "|(  ) (%02d) (%02d) (%02d) | (%02d) (%02d) (%02d) (  )|\n", p2[5], p2[4], p2[3], p2[2], p2[1], p2[0]);
        mvprintw(y + 2, x, "||%02d|                |                |%02d||\n", p2[6], p1[6]);
        mvprintw(y + 3, x, "|(  ) (%02d) (%02d) (%02d) | (%02d) (%02d) (%02d) (  )|\n", p1[0], p1[1], p1[2], p1[3], p1[4], p1[5]);
        mvprintw(y + 4, x, "+-----------------------------------------+\n");
}

void printEndScreen(int y){
	mvprintw(y, (max_x / 2) - 5, "Game Over!");
	int p1_mc = p1[6];
	int p2_mc = p2[6];
	if(p1_mc > p2_mc){
		mvprintw(y + 1, (max_x / 2) - 6, "Player 1 Won!");
	}else if(p1_mc == p2_mc){
		mvprintw(y + 1, (max_x / 2) - 4, "Draw Game!");
	}else{
		mvprintw(y + 1, (max_x / 2) - 4, "Player 2 Won!");
	}
	mvprintw(y + 2, (max_x / 2) - 11, "Press key to contine...");
}

void movePebs(int i){
	if(p1_turn){
		int amount = p1[i];
		p1[i] = 0;
		for(int j = 1; j <= amount; j++){
			int yy = (i + j) / 7;
			int xx = (i + j) % 7;
			if(yy % 2 == 0){
				p1[xx] += 1;
			}else{
				if(xx == 6){
					amount++;
					continue;
				}
				p2[xx] += 1;
				
			}
			if(j == amount){
				if(xx == 6 && yy % 2 == 0){return;}
				if(yy % 2 == 0){
					if(p1[xx] == 1 && p2[abs(xx - 5)] > 0){
						p1[xx] = 0;
						int am = p2[abs(xx - 5)];
						p2[abs(xx - 5)] = 0;
						p1[6] += am + 1;
					}
				}
				p1_turn = !p1_turn;
			}
		}
	}else{
		int amount = p2[abs(i - 5)];
		p2[abs(i - 5)] = 0;
		for(int j = 1; j <= amount; j++){
			int yy = (abs(i - 5) + j) / 7;
			int xx = (abs(i - 5) + j) % 7;
			if(yy % 2 == 0){
				p2[xx] += 1;
			}else{
				if(xx == 6){
					amount++;
					continue;
				}
				p1[xx] += 1;
				
			}
			if(j == amount){
				if(xx == 6 && yy % 2 == 0){return;}
				if(yy % 2 == 0){
					if(p2[xx] == 1 && p1[abs(xx - 5)] > 0){
						p2[xx] = 0;
						int am = p1[abs(xx - 5)];
						p1[abs(xx - 5)] = 0;
						p2[6] += am + 1;
					}
				}
				p1_turn = !p1_turn;
			}
		}
	}
}


bool hasValidMove(){
	int j = 0;
	for(int i = 0; i < 6; i++){
		if(p1[i] > 0){
			j = j + 1;
			break;
		}
	}
	for(int i = 0; i < 6; i++){
		if(p2[i] > 0){
			j = j + 2;
			break;
		}
	}
	if( j == 3){
		return true;
	}else{
		return false;
	}
}

int main(int argc, char *args[]){
        initscr();
        noecho();
        curs_set(FALSE);
		keypad(stdscr, TRUE);
		initBoard();
        while(running){
                getmaxyx(stdscr, max_y, max_x);
                clear();
                int xt = (max_x / 2) - (board_x / 2);
				int yt = (max_y / 2) - (board_y / 2);
				
				mvprintw(0, 0, "Press q to quit");
				printBoard(xt, yt);
				if(hasValidMove()){
					int xx = (xt + 8) + (ii * 5);
					if(ii > 2){xx += 2;}
					if(p1_turn){
						mvprintw(yt + 5, xx, "^");
					}else{
						mvprintw(yt - 1, xx, "V");
					}
				}else{
					printEndScreen(yt + 5);
				}
				mvprintw(1, 0, "char: %d", lastchar);

                refresh();
				int ch = getch();
				lastchar = ch;
				if(hasValidMove()){
					switch(ch){
						case KEY_LEFT:
							if(ii <= 0){
								ii = 0;
							}else{
								ii -= 1;
							}
						break;
						case KEY_RIGHT:
							if(ii >= 5){
								ii = 5;
							}else{
								ii += 1;
							}
						break;
						case 10: //Enter
							movePebs(ii);
						break;
						case 113: //q
							running = 0;
						break;
					}
				}else{
					initBoard();
					if(ch == 113){
						running = 0;
					}
				}
        }
        endwin();
        return 0;
}
