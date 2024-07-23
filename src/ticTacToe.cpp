#include <iostream>
#include <string>

using String = std::string;

struct Tile {
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
    const String topSlice = "     /     /     ";
    const String bottomSlice = (i == 2) ? topSlice : "_____/_____/_____";
    String middleSlice = "";
    for (int j = 0; j < 3; j++) {
      middleSlice += boardArr[i][j] ? "  " + String({boardArr[i][j]}) : "   ";
      if (j < 2) {
        middleSlice += "  /";
      }
    }

    display(topSlice, true);
    display(middleSlice, true);
    display(bottomSlice, true);
  }
}

Tile getTile() {
  display("row: ");
  const int playerRow = getInput(1, 3);
  display("col: ");
  const int playerCol = getInput(1, 3);
  Tile newTile = {playerRow - 1, playerCol - 1};
  return newTile;
}

Tile playerMove(char boardArr[3][3], char player) {
  Tile playerTile;
  display("Player " + String(1, player) + "'s turn to move.", true);
  playerTile = getTile();
  while (boardArr[playerTile.row][playerTile.col] != 0) {
    display("Tile already occupied, try again.", true);
    playerTile = getTile();
  }

  boardArr[playerTile.row][playerTile.col] = player;
  return playerTile;
}

bool isWinningMove(char boardArr[3][3], Tile tile) {
  const char player = boardArr[tile.row][tile.col];
  String rowCheck = "";
  String colCheck = "";
  String diagCheck1 = "";
  String diagCheck2 = "";
  String winCode = {player, player, player};
  bool diagCheckRequired = (tile.row + tile.col) % 2 == 0;

  // change to do while??
  for (int i = 0; i < 3; i++) {
    rowCheck += boardArr[tile.row][i];
    colCheck += boardArr[i][tile.col];
    if (diagCheckRequired) {
      diagCheck1 += boardArr[i][i];
      diagCheck2 += boardArr[i][2 - i];
    }
  }

  if (rowCheck == winCode || colCheck == winCode || diagCheck1 == winCode ||
      diagCheck2 == winCode) {
    return true;
  }
  return false;
}

int main() {
  bool isProgramEnd = false;
  while (!isProgramEnd) {
    char board[3][3] = {{}};

    displayBoard(board);

    for (int i = 0; i < 9; i++) {
      const char player = i % 2 ? 'X' : 'O';
      const Tile lastPlayedTile = playerMove(board, player);

      displayBoard(board);

      if (isWinningMove(board, lastPlayedTile)) {
        display("Game over: Player " + String({player}) + " wins!!", true);
        break;
      }
      if (i == 8) {
        display("Game over: Tie!!", true);
      }
    }

    display("Would you like to play another game? 0 = no, 1 = yes", true);
    int input = getInput(0, 1);
    if (!input) {
      isProgramEnd = true;
    }
  }
  display("Program exited successfully", 1);
}
