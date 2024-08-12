#include <iostream>
#include "GameBoard.h"
#include "Tile.h"
#include <sstream>

// Constructor
GameBoard::GameBoard() : rows(26), cols(26), board(rows, std::vector<Tile*>(cols, nullptr)) {
}

// Parameterized Constructor
GameBoard::GameBoard(int rows, int cols) : rows(rows), cols(cols), board(rows, std::vector<Tile*>(cols, nullptr)) {
}

// Destructor
GameBoard::~GameBoard() {
    // Destructor cleans up the dynamically allocated tiles
    for (int row = 0; row < rows; ++row) {
        for (int col = 0; col < cols; ++col) {
            if (board[row][col] != nullptr) {
                delete board[row][col];  
            }
        }
    }
}

// Copy constructor
GameBoard::GameBoard(const GameBoard& other)
    : rows(other.rows),
      cols(other.cols),
      board(rows, std::vector<Tile*>(cols, nullptr)) {
  for (int row = 0; row < rows; ++row) {
    for (int col = 0; col < cols; ++col) {
      if (other.board[row][col] != nullptr) {
        board[row][col] = new Tile(*other.board[row][col]);
      }
    }
  }
}

// Copy assignment operator
GameBoard& GameBoard::operator=(const GameBoard& other) {
  if (this != &other) {
    // Clean up current resources
    for (int row = 0; row < rows; ++row) {
      for (int col = 0; col < cols; ++col) {
        delete board[row][col];
      }
    }

    // Copy new values
    rows = other.rows;
    cols = other.cols;
    board = std::vector<std::vector<Tile*>>(rows,
                                            std::vector<Tile*>(cols, nullptr));

    for (int row = 0; row < rows; ++row) {
      for (int col = 0; col < cols; ++col) {
        if (other.board[row][col] != nullptr) {
          board[row][col] = new Tile(*other.board[row][col]);
        }
      }
    }
  }
  return *this;
}

// Move constructor
GameBoard::GameBoard(GameBoard&& other)
    : rows(other.rows), cols(other.cols), board(std::move(other.board)) {
  other.rows = 0;
  other.cols = 0;
}

// Move assignment operator
GameBoard& GameBoard::operator=(GameBoard&& other) {
  if (this != &other) {
    // Clean up current resources
    for (int row = 0; row < rows; ++row) {
      for (int col = 0; col < cols; ++col) {
        delete board[row][col];
      }
    }

    // Move new values
    rows = other.rows;
    cols = other.cols;
    board = std::move(other.board);

    other.rows = 0;
    other.cols = 0;
  }
  return *this;
}

// Place a tile on the board
void GameBoard::placeTile(int row, int col, Tile* tile) {
    if (row >= 0 && row < rows && col >= 0 && col < cols) {
        board[row][col] = tile;
    } else {
        std::cout << "Invalid position [" << row << "][" << col << "] for placing tile." << std::endl;
    }
}

// Get a tile from the board
Tile* GameBoard::getTile(int row, int col) const {
    if (row >= 0 && row < rows && col >= 0 && col < cols) {
        return board[row][col];
    }
    std::cout << "Invalid position [" << row << "][" << col << "] for accessing tile." << std::endl;
    return nullptr;
}

// Get the number of rows
int GameBoard::getRows() const {
    return rows;
}

// Get the number of columns
int GameBoard::getCols() const {
    return cols;
}


// Display the board as a string - enhanced function
std::string GameBoard::displayBoard(bool enhanced) const {
    std::string output;
    std::string dashes = "\n--";

    // Print column headers
    output += "   ";
    for (int col = 0; col < cols; ++col) {
        // Uses extra spacing for 0-9 for correct formatting
        if (col < 9){
        output += std::to_string(col) + "  ";
        } else {
        // Must change to one less space for double digit column headers for correect formatting
        output += std::to_string(col) + " ";
        }
        // Appends for variable header formatting
        dashes += "---"; 
    }
    // Appends the dash spacing beneath row header
    output += dashes + "\n";
    // Print each row with its row header
    for (int row = 0; row < rows; ++row) {
        output += std::string(1, 'A' + row) + "|";
        for (int col = 0; col < cols; ++col) {
          if (board[row][col] != nullptr) {
            if (enhanced) {
              // Use color codes for enhanced display
              output += board[row][col]->toColouredString() + "|";
            } else {
              output += board[row][col]->getColour();
              output += std::to_string(board[row][col]->getShape()) + "|";
            }
          } else {
            output += "  |";
          }
        }
        output += "\n";
    }

    return output;
}

// Check if the board is empty
bool GameBoard::isEmpty() const {
    for (int row = 0; row < rows; ++row) {
        for (int col = 0; col < cols; ++col) {
            if (board[row][col] != nullptr) {
                return false;
            }
        }
    }
    return true;
}

// Resize the board
void GameBoard::resize(int newRows, int newCols) {
    std::cout << "Resizing board to " << newRows << "x" << newCols << "." << std::endl;
    board.resize(newRows);
    for (auto& row : board) {
        row.resize(newCols, nullptr);
    }
    rows = newRows;
    cols = newCols;
}
