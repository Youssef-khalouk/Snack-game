# include <stdio.h>
# include <string.h>
# include <stdlib.h>
# include <unistd.h>
# include <time.h>

#if defined (__WIN32__)
	# include <conio.h>
	# include <windows.h>
	
	void enableVTMode(){
		HANDLE hOut = GetStdHandle(STD-OUTPUT-HANDLE);
		if(hOut == INVALID_HANDLE_VALUE) return;
		DWORD dwMode = 0;
		if (!GetConsoleMode(hOut, &dwMode)) return;
		dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
		SetConsoleMode(hOut, dwMode);
	}
	
#else
	#include <termios.h>
	#include <fcntl.h>

	void enableRawMode() {
		struct termios term;
		tcgetattr(0, &term);
		term.c_lflag &= ~(ICANON | ECHO); // Disable canonical mode and echo
		tcsetattr(0, TCSANOW, &term);
	}

	void disableRawMode() {
		struct termios term;
		tcgetattr(0, &term);
		term.c_lflag |= ICANON | ECHO; // Re-enable canonical mode and echo
		tcsetattr(0, TCSANOW, &term);
	}

	int kbhit() {
		struct termios oldt, newt;
		int ch;
		int oldf;

		tcgetattr(0, &oldt);
		newt = oldt;
		newt.c_lflag &= ~(ICANON | ECHO);
		tcsetattr(0, TCSANOW, &newt);
		oldf = fcntl(0, F_GETFL, 0);
		fcntl(0, F_SETFL, oldf | O_NONBLOCK);

		ch = getchar();

		tcsetattr(0, TCSANOW, &oldt);
		fcntl(0, F_SETFL, oldf);

		if(ch != EOF) {
			ungetc(ch, stdin);
			return 1;
		}

		return 0;
	}

#endif

int width = 40;
int height = 20;
char window[20][40];
int snickLength = 2;
int snick [100][2] = {{5,5},{5,4},{5,3}};
int gameOver = 1;
int targetX = 0;
int targetY = 0;
char diraction = 'E';

void setWindow(){
	printf("    Score : %d",snickLength+1);
	printf("\n");
	printf("    ");
	for (int x = 0;x<width;x++){
		printf("-");
	}
	printf("\n");
	for(int i=0; i<height; i++){
		printf("    |");
		for(int j=0; j<width;j++){
			printf("%c",window[i][j]);
		}
		printf("|\n");
	}
	printf("    ");
	for (int x = 0;x<width;x++){
		printf("-");
	}
	printf("\n");
}
void cleanWindow(){
	for(int i=0; i<20; i++){
		for(int j=0; j<50;j++){
			window[i][j] = ' ';
		}
	}
}
void setSnick(){
	for(int i=1; i<snickLength; i++){
		window[snick[i][0]][snick[i][1]]= '*';
	}
	if(diraction == 'N'){
		window[snick[0][0]][snick[0][1]] = '^';
	}else if(diraction == 'E'){
		window[snick[0][0]][snick[0][1]] = '>';
	}else if(diraction == 'S'){
		window[snick[0][0]][snick[0][1]] = 'v';
	}else if(diraction == 'W'){
		window[snick[0][0]][snick[0][1]] = '<';
	}
}
void setTarget(){
	int p =1;
	while(p){
		targetX = rand()% width;
		targetY = rand()% height;
		for(int i=0; i<snickLength; i++){
			if (!(snick[i][0]==targetY && snick[i][1]==targetX)){
				p=0;
			}
		}
	}
	window[targetY][targetX]= '$';
}
void moveAndCkeak(){
	int xLE = snick[snickLength][1];
	int yLE = snick[snickLength][0];
	window[yLE][xLE]=' ';
	for (int i=snickLength; i>0;i--){
		snick[i][0] = snick[i-1][0];
		snick[i][1] = snick[i-1][1];
	}
	if(diraction == 'E'){
		snick[0][1] += 1;
		if (snick[0][1] >= width){
			snick[0][1] = 0;
		}
	}else if(diraction == 'W'){
		snick[0][1] -= 1;
		if(snick[0][1] < 0){
			snick[0][1] = width-1;
		}
		
	}else if(diraction == 'N'){
		snick[0][0] -= 1;
		if(snick[0][0] < 0){
			snick[0][0] = height-1;
		}
	}else if(diraction == 'S'){
		snick[0][0] += 1;
		if(snick[0][0] >= height){
			snick[0][0] = 0;
		}
	}
	if(snick[0][0]==targetY && snick[0][1]==targetX){
		setTarget();
		snickLength++;
		snick[snickLength][1]= xLE;
		snick[snickLength][0]= yLE;
	}
	for(int j=1; j<snickLength;j++){
		if (snick[0][0] == snick[j][0]&&snick[0][1] == snick[j][1]){
			gameOver = 0;
		}
	}
}
void inputListener(){
	if (kbhit()){
		#ifdef _WIN32
			int ch = getch();
		#else
			int ch = getchar();
		#endif
		switch(ch){
			case 'w': diraction = 'N'; break;
			case 's': diraction = 'S'; break;
			case 'd': diraction = 'E'; break;
			case 'a': diraction = 'W'; break;
			case 'x': gameOver = 0; break;
		}	
	}
}
int main(){
	#ifdef _WIN32
		enableVTMode();
	#else
		enableRawMode();
	#endif

	srand(time(0));
	cleanWindow();
	setTarget();
	while(gameOver){
		
		inputListener();
		moveAndCkeak();
		window[targetY][targetX]='$';
		setSnick();
		setWindow();
		#ifdef _WIN32
			Sleep(200);
		#else 
			usleep(200000);
		#endif
		if(gameOver){
			for(int i = 0; i< height +3; i++) { printf("\033[1A"); }
		}
	}
	printf("    gameOver !   your Score is %d\n",snickLength+1);
	
	#ifndef _WIN32
		disableRawMode();
	#endif
	return 0;
}


