int main(){
}

int bestMove(int gameState[6][7], int col, int row, int player){
	gameState[row][col] = player;
	//Check for horizontal connect 4
	int horzCount = 0;
	int team;
	for (int i = 0; i < 5; i++){
		team = gameState[i][0];
		for (int j = 1; j < 6; j++){
			if (gameState[i][j] == team)
				horzCount++;
			else
				team = gameState[i][j];
			if (horzCount == 4){
				if (team == 1)
					columnScore += 10;
				else if (team == 2)
					columnScore -= 10;
		}
		if (horzCount >= 4)
			return columnScore;

	}
	
	//Check for vertical connect 4
	int verCount = 0;
	for (int i = 0; i < 6; i++){
		team = gameState[0][i];
		for (int j = 1; j < 5; j++){
			if (gameState[j][i] == team)
				verCount++;
			else
				team = gameState[j][i];
			if (verCount == 4){
				if (team == 1)
					columnScore += 10;
				else if (team == 2)
					columnScore -= 10;
				return columnScore;
			}
		}
	}4
	
	//Check for diagonal connect 4
	//Need to check for all left diagonals and all right diagonals XXX
	int rightDiagTeam, leftDiagTeam;
	int leftCount = 0;
	int rightCount = 0;
	for (int i = 0; i < 5; i++){
		rightDiagTeam = gameState[i][0];
		leftDiagTeam = gameState[i][6];
		int j = 0;
		do {
			for (int k = 1; (k < 6 - j) || (k < 6 - i); k++){
				if (gameState[i+k][j+k] == rightDiagTeam)
					rightCount++;
				else
					rightDiagTeam = gameState[i+k][j+k];
				if (gamestate[6-i-k][6-j-k] == leftDiagteam;
					leftCount++;
				else
					leftDiagTeam = gamestate[6-i-k][6-j-k];
				if (leftCount == 4 || rightCount == 4)
					if (team == 1)
						columnScore += 10;
					else if (team == 2)
						columnScore -= 10;
			}
			j++
		} while(j < 6 && j*i == 0);
		if (leftCount >= 4 || rightCount >= 4)
			return columnScore;
	}
	
	//Check which column is the best to play in
	int row;
	row = placeMove(gameState,1);
	col1Tot = bestMove(gameState,col,row) + columnScore;

}

int placeMove(int gameState[6][7],int col){
	int count = 0;
	for (int i = 0; gameState[i][col] != 0; i++)
		count++;
	return count;
}