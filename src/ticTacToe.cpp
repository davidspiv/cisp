#include <iostream>
#include <string>

using String = std::string;

struct Cord {
  int row;
  int col;
};

void display(const String &output, bool returnFlag = false) {
  if (returnFlag) {
    std::cout << output << std::endl;
  } else {
    std::cout << output;
  };
}

int getIntegerInput(int minVal, int maxVal) {
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
  // three horizontal "slices" per row
  String outputString = "";
  for (int row = 0; row < 3; row++) {
    const String topSlice = "     /     /     ";
    const String bottomSlice = (row != 2) ? "_____/_____/_____" : topSlice;
    String middleSlice = "";

    for (int col = 0; col < 3; col++) {
      middleSlice +=
          boardArr[row][col] ? "  " + String({boardArr[row][col]}) : "   ";
      if (col != 2) {
        middleSlice += "  /";
      }
    }
    outputString += topSlice + "\n" + middleSlice + "\n" + bottomSlice + "\n";
  }
  display(outputString);
}

Cord getCord() {
  display("row: ");
  const int playerRow = getIntegerInput(1, 3);
  display("col: ");
  const int playerCol = getIntegerInput(1, 3);
  return {playerRow - 1, playerCol - 1};
}

Cord playerMove(char boardArr[3][3], char player) {
  Cord targetCord;
  display("Player " + String(1, player) + "'s turn to move.", true);
  targetCord = getCord();
  while (boardArr[targetCord.row][targetCord.col] != 0) {
    display("Square already occupied, try again.", true);
    targetCord = getCord();
  }

  boardArr[targetCord.row][targetCord.col] = player;
  return targetCord;
}

bool isWinningMove(char boardArr[3][3], Cord cord) {
  const char player = boardArr[cord.row][cord.col];
  const bool diagCheckRequired = (cord.row + cord.col) % 2 == 0;
  bool rowCheck, colCheck, diagCheck1, diagCheck2;
  rowCheck = colCheck = diagCheck1 = diagCheck2 = true;

  for (int i = 0; i < 3; i++) {
    if (rowCheck && boardArr[cord.row][i] != player) rowCheck = false;
    if (colCheck && boardArr[i][cord.col] != player) colCheck = false;
    if (!diagCheckRequired) {
      diagCheck1 = diagCheck2 = false;
    } else {
      if (diagCheck1 && boardArr[i][i] != player) diagCheck1 = false;
      if (diagCheck2 && boardArr[i][2 - i] != player) diagCheck2 = false;
    }
  }

  return rowCheck || colCheck || diagCheck1 || diagCheck2;
}

int main() {
  do {
    char board[3][3] = {{}};

    displayBoard(board);

    for (int moveCount = 0; moveCount < 9; moveCount++) {
      const char player = moveCount % 2 ? 'X' : 'O';
      const Cord lastPlayedCord = playerMove(board, player);

      displayBoard(board);

      if (isWinningMove(board, lastPlayedCord)) {
        display("Game over: Player " + String({player}) + " wins!!", true);
        break;
      }
      if (moveCount == 8) {
        display("Game over: Tie!!", true);
      }
    }

    display("Would you like to play another game? 0 = no, 1 = yes: ");

  } while (getIntegerInput(0, 1));

  display("Program exited successfully", true);
}
