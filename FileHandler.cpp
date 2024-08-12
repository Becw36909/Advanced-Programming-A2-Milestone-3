#include "FileHandler.h"
#include <fstream>
#include <iostream>
#include <vector>

/*
 * Helper function to convert LinkedList to std::vector
 * This function iterates through the linked list, extracts each tile,
 * and adds it to a std::vector.
 */
std::vector<Tile*> FileHandler::linkedListToVector(LinkedList* list) {
    std::vector<Tile*> vec;
    Node* current = list->getHead();
    while (current) {
        vec.push_back(current->getTile());
        current = current->getNext();
    }
    return vec;
}

/*
 * Helper function to convert std::vector to LinkedList
 * This function iterates through the std::vector,
 * and adds each tile to the linked list.
 */
void FileHandler::vectorToLinkedList(const std::vector<Tile*>& vec, LinkedList* list) {
    list->clear();
    for (Tile* tile : vec) {
        list->addBack(tile);
    }
}

/*
 * Save the game state to a file
 * This function serializes the players, board, tile bag,
 * and current player, then writes them to a file.
 */
void FileHandler::saveGame(const std::string& filename, Player* player1, Player* player2, TileBag* tileBag, GameBoard* board, Player* currentPlayer) {
    std::ofstream outFile(filename);
    if (outFile.is_open()) {
        outFile << serialisePlayer(player1) << std::endl;
        outFile << serialisePlayer(player2) << std::endl;
        outFile << serialiseBoard(board) << std::endl;
        outFile << serialiseTileBag(tileBag) << std::endl;
        outFile << serialiseCurrentPlayer(currentPlayer);
        outFile.close();
        std::cout << "Game successfully saved" << std::endl;
    } else {
        std::cerr << "Error: Unable to open file for writing" << std::endl;
    }
}

/*
 * Load the game state from a file
 * This function reads the serialized players, board, tile bag,
 * and current player from a file and deserializes them.
 * THE BELOW INCLUDES PRINT OUTS TO CHECK FILE IS READING
 * CORRECTLY AND MAY BE DELETED AND THE ABOVE FUNCTION USED
 * INSTEAD.
 */
bool FileHandler::loadGame(const std::string& filename, Player* player1, Player* player2, TileBag* tileBag, GameBoard*& board, Player* currentPlayer) {
    std::ifstream inFile(filename);
    if (inFile.is_open()) {
        std::string player1Data, player2Data, boardData, tileBagData, currentPlayerData;

        // Read and deserialize Player 1
        std::getline(inFile, player1Data);
        std::string player1Score;
        std::getline(inFile, player1Score);
        player1Data += "\n" + player1Score;

        std::string player1Hand;
        std::getline(inFile, player1Hand);
        player1Data += "\n" + player1Hand;

        // std::cout << "Player 1 Data: " << player1Data << std::endl;
        deserialisePlayer(player1, player1Data);

        // Read and deserialize Player 2
        std::getline(inFile, player2Data);
        std::string player2Score;
        std::getline(inFile, player2Score);
        player2Data += "\n" + player2Score;

        std::string player2Hand;
        std::getline(inFile, player2Hand);
        player2Data += "\n" + player2Hand;

        // std::cout << "Player 2 Data: " << player2Data << std::endl;
        deserialisePlayer(player2, player2Data);

        // Read and deserialize Board
        std::getline(inFile, boardData);
        std::string boardTiles;
        std::getline(inFile, boardTiles);
        boardData += "\n" + boardTiles;

         // Create a new GameBoard with the deserialized data
        GameBoard* newBoard = deserialiseBoard(boardData);
        if (newBoard) {
          if (board) {
            // Clean up the old board if it exists
            delete board;
            // Set to nullptr after deletion to avoid dangling pointer
            board = nullptr;  
          }
          board = newBoard;
        } else {
          std::cerr << "Error: Invalid board data" << std::endl;
          inFile.close();
          return false;
        }

        // Read and deserialize Tile Bag
        std::getline(inFile, tileBagData);
        // std::cout << "Tile Bag Data: " << tileBagData << std::endl;
        deserialiseTileBag(tileBag, tileBagData);

        // Read and deserialize Current Player
        std::getline(inFile, currentPlayerData);
        // std::cout << "Current Player Data: " << currentPlayerData << std::endl;
        deserialiseCurrentPlayer(currentPlayer, currentPlayerData);

        inFile.close();
        return true;
    } else {
        std::cerr << "Error: Unable to open file for reading" << std::endl;
        return false;
    }
}

/*
 * Check if a file exists
 * This function tries to open a file and returns true if successful.
 */
bool FileHandler::fileExists(const std::string& filename) {
    std::ifstream file(filename);
    return file.good();
}

/*
 * Method to read file content into a string.
 */
std::string FileHandler::readFileContent(const std::string& filename) const {
    std::ifstream inFile(filename);
    if (!inFile.is_open()) {
        std::cerr << "Error: Unable to open file for reading" << std::endl;

        return "";

    }

    std::string fileContent;
    std::string line;
    while (std::getline(inFile, line)) {
        fileContent += line + "\n";
    }

    inFile.close();

    // Remove trailing newline
    if (!fileContent.empty() && fileContent[fileContent.length() - 1] == '\n') {
        fileContent.erase(fileContent.length() - 1);
    }

    return fileContent;
}




/*
 * Serialize a Player object to a string
 * This function converts the player's name, score, and hand
 * to a string representation.
 */
std::string FileHandler::serialisePlayer(Player* player) {
    std::string result;
    result += player->getName() + "\n";
    result += std::to_string(player->getScore()) + "\n";

    LinkedList* hand = player->getHand();
    Node* current = hand->getHead();
    while (current) {
        Tile* tile = current->getTile();
        result += tile->getColour() + std::to_string(tile->getShape());
        if (current->getNext()) {
            result += ",";
        }
        current = current->getNext();
    }

    return result;
}

/*
 * Serialize a TileBag object to a string
 * This function converts the tiles in the bag to a string representation.
 */
std::string FileHandler::serialiseTileBag(TileBag* tileBag) {
    std::string result;
    LinkedList* tiles = tileBag->getTiles();
    Node* current = tiles->getHead();
    while (current) {
        Tile* tile = current->getTile();
        result += tile->getColour() + std::to_string(tile->getShape());
        if (current->getNext()) {
            result += ",";
        }
        current = current->getNext();
    }

    return result;
}

/*
 * Serialize a GameBoard object to a string
 * This function converts the board size and tiles
 * to a string representation.
 */
std::string FileHandler::serialiseBoard(GameBoard* board) {
    std::string result;
    result += std::to_string(board->getRows()) + "," + std::to_string(board->getCols()) + "\n";

    bool firstTile = true;
    for (int row = 0; row < board->getRows(); ++row) {
        for (int col = 0; col < board->getCols(); ++col) {
            Tile* tile = board->getTile(row, col);
            if (tile) {
                if (!firstTile) {
                    result += ",";
                }
                result += tile->getColour() + std::to_string(tile->getShape()) + "@" + std::string(1, 'A' + row) + std::to_string(col);
                firstTile = false;
            }
        }
    }

    return result;
}


/*
 * Serialize the current player to a string
 * This function returns the name of the current player.
 */
std::string FileHandler::serialiseCurrentPlayer(Player* currentPlayer) {
    return currentPlayer->getName();
}

/*
 * Deserialize a Player object from a string
 * This function extracts the player's name, score, and hand
 * from a string representation.
 */
void FileHandler::deserialisePlayer(Player* player, const std::string& data) {
    size_t pos = 0;
    size_t nextPos = data.find('\n', pos);
    std::string name = data.substr(pos, nextPos - pos);
    player->setName(name);

    pos = nextPos + 1;
    nextPos = data.find('\n', pos);
    int score = std::stoi(data.substr(pos, nextPos - pos));
    player->setScore(score);

    pos = nextPos + 1;
    std::string handData = data.substr(pos);
    std::vector<Tile*> hand;
    size_t start = 0;
    size_t end = handData.find(',', start);
    while (end != std::string::npos) {
        std::string tileData = handData.substr(start, end - start);
        Colour colour = tileData[0];
        Shape shape = std::stoi(tileData.substr(1));
        hand.push_back(new Tile(colour, shape));

        start = end + 1;
        end = handData.find(',', start);
    }
    if (start < handData.length()) { // Last tile
        std::string tileData = handData.substr(start);
        Colour colour = tileData[0];
        Shape shape = std::stoi(tileData.substr(1));
        hand.push_back(new Tile(colour, shape));
    }
    vectorToLinkedList(hand, player->getHand());
}


/*
 * Deserialize a TileBag object from a string
 * This function extracts the tiles from a string representation
 * and adds them to the tile bag.
 */
void FileHandler::deserialiseTileBag(TileBag* tileBag, const std::string& data) {
    std::vector<Tile*> tiles;
    size_t start = 0;
    size_t end = data.find(',', start);
    while (end != std::string::npos) {
        std::string tileEntry = data.substr(start, end - start);
        Colour colour = tileEntry[0];
        Shape shape = std::stoi(tileEntry.substr(1));
        tiles.push_back(new Tile(colour, shape));
        start = end + 1;
        end = data.find(',', start);
    }
    if (start < data.length()) { // Last tile
        std::string tileEntry = data.substr(start);
        Colour colour = tileEntry[0];
        Shape shape = std::stoi(tileEntry.substr(1));
        tiles.push_back(new Tile(colour, shape));
    }
    vectorToLinkedList(tiles, tileBag->getTiles());
}

/*
 * Deserialize a GameBoard object from a string
 * This function extracts the board size and tiles
 * from a string representation and sets up the board.
 */
GameBoard* FileHandler::deserialiseBoard(const std::string& data) {
  size_t pos = 0;
  size_t nextPos = data.find('\n', pos);
  std::string sizeData = data.substr(pos, nextPos - pos);
  size_t commaPos = sizeData.find(',');
  if (commaPos == std::string::npos) {
    std::cerr << "Error: Invalid board size format" << std::endl;
    return nullptr;
  }

  int rows = std::stoi(sizeData.substr(0, commaPos));
  int cols = std::stoi(sizeData.substr(commaPos + 1));

  // Create a new GameBoard with the specified size
  GameBoard* board = new GameBoard(rows, cols);

  pos = nextPos + 1;
  std::string boardData = data.substr(pos);
  size_t start = 0;
  size_t end = boardData.find(',', start);
  while (end != std::string::npos) {
    std::string tileData = boardData.substr(start, end - start);
    Colour colour = tileData[0];
    size_t atPos = tileData.find('@');
    if (atPos == std::string::npos) {
      std::cerr << "Error: Invalid tile data format - " << tileData
                << std::endl;
      return nullptr;
    }

    Shape shape = std::stoi(tileData.substr(1, atPos - 1));
    std::string position = tileData.substr(atPos + 1);

    if (position.length() < 2) {
      std::cerr << "Error: Invalid tile position format - " << position
                << std::endl;
      return nullptr;
    }

    int row = position[0] - 'A';
    int col = std::stoi(position.substr(1));
    board->placeTile(row, col, new Tile(colour, shape));

    start = end + 1;
    end = boardData.find(',', start);
  }

  // Process the last tile data
  if (start < boardData.length()) {
    std::string tileData = boardData.substr(start);

    Colour colour = tileData[0];
    size_t atPos = tileData.find('@');
    if (atPos == std::string::npos) {
      std::cerr << "Error: Invalid tile data format - " << tileData
                << std::endl;
      return nullptr;
    }

    Shape shape = std::stoi(tileData.substr(1, atPos - 1));
    std::string position = tileData.substr(atPos + 1);

    if (position.length() < 2) {
      std::cerr << "Error: Invalid tile position format - " << position
                << std::endl;
      return nullptr;
    }

    int row = position[0] - 'A';
    int col = std::stoi(position.substr(1));
    board->placeTile(row, col, new Tile(colour, shape));
  }

  return board;
}

/*
 * Deserialize the current player from a string
 * This function sets the current player's name
 * from a string representation.
 */
void FileHandler::deserialiseCurrentPlayer(Player* currentPlayer, const std::string& data) {
    currentPlayer->setName(data);
}