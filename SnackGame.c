# include <stdio.h>
# include <string.h>
# include <stdlib.h>
# include <conio.h>
# include <unistd.h>
# include <windows.h>
# include <time.h>

int width = 30;
int height = 15;
char window[15][30];
int snickLength = 2;
int snick [100][2] = {{5,5},{5,4},{5,3}};
int gameOver = 1;
int targetX = 0;
int targetY = 0;
char diraction = 'E';

void setWindow(){
	printf("\t\t Score : %d\n\t\t ",snickLength+1);
	for (int x = 0;x<width;x++){
		printf("-");
	}
	printf("\n");
	for(int i=0; i<height; i++){
		printf("\t\t|");
		for(int j=0; j<width;j++){
			printf("%c",window[i][j]);
		}
		printf("|\n");
	}
	printf("\t\t ");
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
	// if keyPressed => key
	if (kbhit()){
		switch(getch()){
		case 'w':
			diraction = 'N';
			break;
		case 's':
			diraction = 'S';
			break;
		case 'd':
			diraction = 'E';
			break;
		case 'a':
			diraction = 'W';
		}
	}
}
int main(){
	srand(time(0));
	cleanWindow();
	setTarget();
	while(gameOver){
		
		inputListener();
		moveAndCkeak();
		window[targetY][targetX]='$';
		setSnick();
		setWindow();
		Sleep(50);
		if(gameOver){
			system("cls");
		}
	}
	printf("\t\t gameOver !   your Score is %d",snickLength+1);
	getch();
	return 0;
}


