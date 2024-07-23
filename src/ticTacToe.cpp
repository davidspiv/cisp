#include <iostream>
#include <string>

using String = std::string;

struct Square {
  int row;
  int col;
};

void display(String output, bool returnFlag = false) {
  if (returnFlag) output += "\n";
  std::cout << output;
}

int getInput(int minVal, int maxVal) {
  int input;

  std::cin >> input;

  while (std::cin.fail() || input < minVal || input > maxVal) {
    display("Must be an integer between and including " +
                std::to_string(minVal) + " to " + std::to_string(maxVal),
            true);

    std::cin.clear();
    std::cin.ignore(1000, '\n');
    std::cin >> input;
  }
  return input;
}

void displayBoard(char boardArr[3][3]) {
  for (int i = 0; i < 3; i++) {
    display("     /     /     ", true);
    for (int j = 0; j < 3; j++) {
      String lineSegment = "  ";
      lineSegment += boardArr[i][j];
      if (j < 2) {
        lineSegment += "  /";
      } else {
        lineSegment += "  \n";
      }
      display(lineSegment);
      if (boardArr[i][j] == ' ') {
      }
    }
    if (i < 2) {
      display("_____/_____/_____", true);
    } else {
      display("     /     /     ", true);
    }
  }
}

Square getSquare() {
  display("row: ");
  int playerRow = getInput(1, 3) - 1;
  display("col: ");
  int playerCol = getInput(1, 3) - 1;
  Square newSquare = {playerRow, playerCol};
  return newSquare;
}

bool playerMove(char boardArr[3][3], char player) {
  Square playerSquare;
  display("Player " + String(1, player) + "'s turn to move.", true);
  playerSquare = getSquare();
  while (boardArr[playerSquare.row][playerSquare.col] != ' ') {
    display("Square already occupied, try again.", true);
    playerSquare = getSquare();
  }
  boardArr[playerSquare.row][playerSquare.col] = player;
  displayBoard(boardArr);

  String rowCheck = "";
  String colCheck = "";
  String diagCheck1 = "";
  String diagCheck2 = "";
  String winCode = String(1, player) + String(1, player) + String(1, player);

  for (int i = 0; i < 3; i++) {
    rowCheck += boardArr[playerSquare.row][i];
    colCheck += boardArr[i][playerSquare.col];
    diagCheck1 += boardArr[i][i];
    diagCheck2 += boardArr[i][2 - i];
  }

  if (rowCheck == winCode || colCheck == winCode || diagCheck1 == winCode ||
      diagCheck2 == winCode) {
    display("Player " + String(1, player) + " wins!!", true);
    return true;
  }
  return false;
}

int main() {
  char board[3][3];
  bool isGameEnd = false;
  while (!isGameEnd) {
    for (int i = 0; i < 3; i++) {
      for (int j = 0; j < 3; j++) {
        board[i][j] = ' ';
      }
    }
    displayBoard(board);
    for (int i = 0; i < 9; i++) {
      if (i % 2) {
        isGameEnd = playerMove(board, 'X');
      } else {
        isGameEnd = playerMove(board, 'O');
      }
      if (isGameEnd) {
        break;
      }
    }

    display("Would you like to play another game? 0 = no, 1 = yes", true);
    int input = getInput(0, 1);
    if (input) {
      isGameEnd = false;
    }
  }
  display("Program exited successfully", 1);
}
