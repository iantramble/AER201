#include <stdio.h>

struct arrayByVal
{
	int game[6][7];
}; //**structs can be passed by value (i.e. by making a copy), but arrays cannot**

//Used to display gameboard in main function
int display(int gameBoard[6][7]){
	for (int i = 5; i >= 0; i--){
		for (int j = 0; j < 7; j++){
			if (gameBoard[i][j] == 0)
				printf("| ");
			else
				printf("|%d",gameBoard[i][j]);
		}
		printf("|\n");
	}
	return 0;
}

int power(int base,int exp){
	int product = 1;
	for (int i = 0; i < exp;i++)
		product *= base;
	
	return product;
} 

//Used to determine which row a piece placed into a column will fall into
int placeMove(int gameState[6][7],int col){
	int i;
	for (i = 0; gameState[i][col] != 0 && i < 6; i++){}
	return i;
}

//Used to check for vertical,horizontal,and diagonal connect fours and return the resulting score
int bScore(int gameState[6][7]){
	long branchScore = 0;
	
	//Check for horizontal connect 4
	int horzCount;
	int team;
	for (int i = 0; i < 6; i++){
		team = gameState[i][0];
		horzCount = 0;
		for (int j = 0; j < 7; j++){
			if (gameState[i][j] != 0){
				if (gameState[i][j] == team){
					horzCount++;
				}
				else if (horzCount == 0){
					horzCount++;
					team = gameState[i][j];
				}
				else {
					team = gameState[i][j];
					horzCount = 1;
				}
			}
			else
				horzCount = 0;
			
			if (horzCount == 4){
				if (team == 1) {
					branchScore += 1000;
					//printf("Team 1 HC4\n");
				}
				else if (team == 2){
					branchScore -= 1000;
					//printf("Team 2 HC4\n");
				}
			}
		}
	}
	
	//Check for vertical connect 4
	int verCount;
	for (int i = 0; i < 7; i++){
		team = gameState[0][i];
		verCount = 0;
		for (int j = 0; j < 6; j++){
			if (gameState[j][i] != 0){
				if (gameState[j][i] == team){
					verCount++;
				}
				else if (verCount == 0){
					verCount++;
					team = gameState[j][i];
				}
				else {
					team = gameState[j][i];
					verCount = 1;
				}
			}
			else
				verCount = 0;
		
			//VERTICAL HEURISTICS
			if (verCount == 4){
				if (team == 1){
					branchScore += 1000;
					//printf("Team 1 VC4\n");
				}
				else if (team == 2){
					branchScore -= 1000;
					//printf("Team 2 VC4\n");
				}
			}
		}
	}
	
	//Check for diagonal connect 4
	//Need to check for all left diagonals and all right diagonals xxx
	int rightDiagTeam, leftDiagTeam;
	int leftCount;
	int rightCount;;
	for (int i = 0; i < 6; i++){
		int j = 0;
		do {
			rightDiagTeam = gameState[i][j];
			leftDiagTeam = gameState[i][6-j];
			leftCount = 0;
			rightCount = 0;
			for (int k = 0; (k < 6 - j) || (k < 6 - i); k++){
				if (gameState[i+k][j+k] != 0){
					if (rightCount == 0){
						rightCount++;
						rightDiagTeam = gameState[i+k][j+k];
					}
					else if (gameState[i+k][j+k] == rightDiagTeam){
						rightCount++;
					}
					else {
						rightDiagTeam = gameState[i+k][j+k];
						rightCount = 1;
					}
				}
				else
					rightCount = 0;

				if (gameState[i+k][6-j-k] != 0){
					if (leftCount == 0){
						leftCount++;
						leftDiagTeam = gameState[i+k][6-j-k];
					}
					else if (gameState[i+k][6-j-k] == leftDiagTeam){
						leftCount++;
					}

					else {
						leftDiagTeam = gameState[i+k][6-j-k];
						leftCount = 1;
					}
				}
				else
					leftCount = 0;
				
				//DIAGONAL HEURISTICS
				
				if (rightCount == 4){
					if (rightDiagTeam == 1){
						branchScore += 1000;
					}
					else if (rightDiagTeam == 2){
						branchScore -= 1000;
					}
				if (leftCount == 4){
					if (leftDiagTeam == 1){
						branchScore += 1000;
					}
					else if (leftDiagTeam == 2){
						branchScore -= 1000;
					}
				}
			}
			j++;
		} while(j < 7 && j*i == 0);
	}
	return branchScore;
}

//Returns score obtained by making the initial move
int score(struct arrayByVal gameState, int col, int row, int player,int depth, long prevTotal){

	(gameState.game)[row][col] = player;
	long branchScore = bScore(gameState.game);
	
	//BASE CASE
	if (depth == 6)
		return (branchScore - prevTotal);

	//SWITCH PLAYER - assuming turn based
	if (player == 2)
		player = 1;
	else
		player = 2;

	//SUM USING RECURSION
	long columnScore = (branchScore - prevTotal);
	int recurseRow;
	for (int i = 0; i < 7; i++) {
		recurseRow = placeMove(gameState.game,i);
		if (recurseRow < 6) {
			columnScore += score(gameState,i,recurseRow,player,depth+1,branchScore);
		}
	}
	
	return columnScore;
}

//Uses score to actually select which column is best
int pickCol(struct arrayByVal gameState){
	int colScore;
	
	int col = 0;
	int row = placeMove(gameState.game,col);
	int bestScore;
	
	//Find the first col that is not full
	while (row == 6)
		row = placeMove(gameState.game,++col);
	bestScore = score(gameState,col,row,1,0,bScore(gameState.game));
	
	for (int i = col + 1; i < 7; i++){
		row = placeMove(gameState.game,i);
		printf("Col: %d, Row: %d\n",i,row);
		if (row < 6) {
			colScore = score(gameState,i,row,1,0,bScore(gameState.game));
			if (colScore >= bestScore){
				bestScore = colScore;
				col = i;
			}
		}
	}
	printf("PickCol: %d\n",col);
	return col;
}


int main(){
	struct arrayByVal gameBoard = {
		0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,
		0,0,0,0,0,0,0
	}; //initialize array to empty
	int row,col,move;
	while (1){
		gameBoard.game[0][3] = 1;
		display(gameBoard.game);
		printf("Enter column: ");
		scanf("%d", &col);
		printf("\n");
		row = placeMove(gameBoard.game,col);
		(gameBoard.game)[row][col] = 2;
		move = pickCol(gameBoard);
		row = placeMove(gameBoard.game,move);
		(gameBoard.game)[row][move] = 1;
	}
	return 0;
}

