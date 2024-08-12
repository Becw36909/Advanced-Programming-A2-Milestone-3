#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include "Tile.h"
#include "TileBag.h"
#include "Player.h"
#include "LinkedList.h"
#include "FileHandler.h"
#include "GameBoard.h"
#include "Rules.h"
#include "InputValidator.h"
#include "Student.h"
#include "StudentInfo.h"
#include "Rules.h"
#include "Tests.cpp"
#include <random>

#define EXIT_SUCCESS 0
#define NUM_BOARD_ROWS 26
#define NUM_BOARD_COLS 26
#define STARTING_HAND_SIZE 6

// Function prototypes
void displayWelcomeMessage();
void displayMainMenu(bool enhanced);
void startNewGame(bool &quit, bool enhanced);
void loadGame(bool &quit, bool enhanced);
void showCredits();
void handleMenuChoice(int choice, bool &quit, unsigned int randSeed, bool enhanced);
void playTurn(Player *player, Player *opponent, TileBag *tileBag, GameBoard *board, bool &quit, bool enhanced);
void gameLoop(Player *player1, Player *player2, TileBag *tileBag, GameBoard *board, bool enhanced);
void printScores(Player *player1, Player *player2, TileBag *tileBag, GameBoard* GameBoard, bool &quit, bool enhanced);
std::string handleInput(bool &quit);
bool chooseVersion();
void handleEnhancedPlayerTurn(Player* currentPlayer, Player* otherPlayer, TileBag* tileBag, GameBoard* gameBoard, bool& quit, bool enhanced);

int main(int argc, char **argv)
{
  bool quit = false;
  int randSeed = (unsigned int)time(NULL);

  
  if (argc > 1) {
    if (std::string(argv[1]) == "test") {
      // run unit tetsts
      Tests::run();
      return EXIT_SUCCESS;
    }
    if (std::string(argv[1]) == "e2etest") {
      randSeed = 0;
    }
  }

  displayWelcomeMessage();

  bool enhanced = chooseVersion();

    //  std::cout << "Choose version to run:" << std::endl;
    // std::cout << "1. Base Qwirkle" << std::endl;
    // std::cout << "2. Enhanced Qwirkle" << std::endl;
    // std::cout << "> ";
    
    // std::string versionChoice;
    // std::getline(std::cin, versionChoice);

    //     bool enhanced = false;
    // if (versionChoice == "2") {
    //     enhanced = true;
    // }

  while (!quit)
  {
    displayMainMenu(enhanced);
    std::string input = handleInput(quit);

    if(!quit) {
      // Convert input to an integer
      try
      {
        int choice = std::stoi(input);
        handleMenuChoice(choice, quit, randSeed, enhanced);
      }
      catch (const std::invalid_argument &)
      {
        std::cout << "Invalid input. Please enter a number between 1 and 4." << std::endl;
      }
      catch (const std::out_of_range &)
      {
        std::cout << "Invalid input. Number is out of range. Please enter a number between 1 and 4." << std::endl;
      }
    }  
    if(quit) {
      std::cout << "Goodbye!" << std::endl;
    }
  }

  return EXIT_SUCCESS;
}

void displayWelcomeMessage()
{
  std::cout << "Welcome to Qwirkle!" << std::endl;
  std::cout << "-------------------" << std::endl;
}

void displayMainMenu(bool enhanced)
{
  if (enhanced) {
    std::cout << "Qwirkle Version: Enhanced" << std::endl;
  } else {
    std::cout << "Qwirkle Version: Base" << std::endl;
  }
  std::cout << "Menu" << std::endl;
  std::cout << "1. New Game" << std::endl;
  std::cout << "2. Load Game" << std::endl;
  std::cout << "3. Credits" << std::endl;
  std::cout << "4. Quit" << std::endl;
  std::cout << "> ";
}

bool chooseVersion() {
  std::cout << "Choose version to run:" << std::endl;
  std::cout << "1. Base Qwirkle" << std::endl;
  std::cout << "2. Enhanced Qwirkle" << std::endl;
  std::cout << "> ";

  std::string versionChoice;
  std::getline(std::cin, versionChoice);

  if (versionChoice == "2") {
    return true;
  }
  return false;
}

void startNewGame(bool &quit, unsigned int randSeed, bool enhanced)
{
  std::cout << "Enter a name for player 1 (uppercase characters only)" << std::endl;
  std::cout << "> ";
  std::string player1Name = handleInput(quit);

  while (!InputValidator::isValidName(player1Name) && !quit)
  {
    std::cout << "Invalid name. Please enter uppercase letters only." << std::endl;
    std::cout << "> ";
    player1Name = handleInput(quit);
  }

  std::cout << "Enter a name for player 2 (uppercase characters only)" << std::endl;
  std::cout << "> ";
  std::string player2Name = handleInput(quit);

  while (!InputValidator::isValidName(player2Name) && !quit)
  {
    std::cout << "Invalid name. Please enter uppercase letters only." << std::endl;
    std::cout << "> ";
    player2Name = handleInput(quit);
  }

  Player player1(player1Name);
  Player player2(player2Name);
  
  GameBoard gameBoard(NUM_BOARD_ROWS, NUM_BOARD_COLS);

  TileBag tileBag;
  // Shuffle the tile bag
  tileBag.shuffle(randSeed);

  std::cout << "Let's Play!" << std::endl;

  // Draws 6 tiles for each player to start the game
  player1.drawQuantityTiles(&tileBag, STARTING_HAND_SIZE);
  player2.drawQuantityTiles(&tileBag, STARTING_HAND_SIZE);

  // Primary functions used to run recursive gameplay operations
  gameLoop(&player1, &player2, &tileBag, &gameBoard, enhanced);
}

void loadGame(bool& quit, bool enhanced) {

    std::cout << "Enter the filename from which to load a game:" << std::endl;
    std::cout << "> ";
    std::string filename = handleInput(quit);

    if (!quit && !InputValidator::isFileNameValid(filename)) {
        std::cerr << "Error: Invalid file name or format." << std::endl;
        return;
    }

    FileHandler fileHandler;

    if (!fileHandler.fileExists(filename)) {
        std::cerr << "Error: File does not exist." << std::endl;
        return;
    }

    // Load the game state
    Player* loadedPlayer1 = new Player("Temp1");
    Player* loadedPlayer2 = new Player("Temp2");
    TileBag* loadedTileBag = new TileBag();
    GameBoard* loadedBoard = new GameBoard(); 
    Player* currentPlayer = new Player("Current");

    bool gameLoaded = fileHandler.loadGame(filename, loadedPlayer1, loadedPlayer2, loadedTileBag, loadedBoard, currentPlayer);
    
    if (!gameLoaded) {
        std::cerr << "Error: Invalid file format." << std::endl;
    } else {
      std::cout << "Qwirkle game successfully loaded" << std::endl;

      if (currentPlayer->getName() == loadedPlayer1->getName())
      {
        gameLoop(loadedPlayer1, loadedPlayer2, loadedTileBag, loadedBoard, enhanced);
      } else
      {
        gameLoop(loadedPlayer2, loadedPlayer1, loadedTileBag, loadedBoard, enhanced);
      }
    }
    
    delete loadedPlayer1;
    delete loadedPlayer2;
    delete loadedTileBag;
    delete loadedBoard;
    delete currentPlayer;
}

void playTurn(Player *player, Player *opponent, TileBag *tileBag,
              GameBoard *gameBoard, bool &quit, bool enhanced) {
  bool validInput = false;
  while (!validInput && !quit) {
    std::cout << gameBoard->displayBoard(enhanced) << std::endl;
    std::cout << "Tiles in hand: " << player->getHand()->toString()
              << std::endl;
    std::cout << "Your move " << player->getName() << ": ";
    std::string playerMove = handleInput(quit);

    if (playerMove == "quit" || quit) {
      quit = true;
    } else if (playerMove == "save") {
      // Implement save game logic
      std::cout << "Enter filename to save: ";
      std::string filename = handleInput(quit);

      FileHandler fileHandler;
      fileHandler.saveGame(filename, player, opponent, tileBag, gameBoard,
                           player);
      std::cout << "Game saved to " << filename << std::endl;
    } else if (playerMove.substr(0, 7) == "replace") {
      std::string tileToReplace = playerMove.substr(8);
      // Ensure input is valid
      if (tileToReplace.size() == 2) {
        char colour = tileToReplace[0];
        int shape = tileToReplace[1] - '0';
        Tile *tile = new Tile(colour, shape);
        Tile *removedTile = player->removeTileFromHand(tile);
        if (removedTile != nullptr) {
          std::cout << removedTile->print()
                    << " tile removed from hand and added to the bag."
                    << std::endl;
          delete removedTile;
          tileBag->addTile(tile);
          Tile *newTile = tileBag->drawTile();
          if (newTile != nullptr) {
            player->addTileToHand(newTile);
            std::cout << newTile->print()
                      << " tile drawn and added to your hand." << std::endl;
          } else {
            std::cout << "No tiles left to draw from the tile bag."
                      << std::endl;
          }
          validInput = true;
        } else {
          std::cout << "You don't have that tile in your hand." << std::endl;
        }
      } else {
        std::cout << "Invalid tile format. Use <colour><shape>." << std::endl;
      }
    } else {
      std::stringstream stringstream(playerMove);
      std::string extractedWord;
      std::vector<std::string> moveBreakdown;

      while (stringstream >> extractedWord) {
        moveBreakdown.push_back(extractedWord);
      }

      if (moveBreakdown.size() == 4 && moveBreakdown[0] == "place" &&
          moveBreakdown[2] == "at") {
        char tileColour = moveBreakdown[1][0];
        int tileShape = moveBreakdown[1][1] - '0';
        char rowChar = moveBreakdown[3][0];
        int col = std::stoi(moveBreakdown[3].substr(1));

        int row = rowChar - 'A';

        Tile *tile = new Tile(tileColour, tileShape);

        // Checks if the tile exists in the player's hand
        if (player->containsTile(tile)) {
          if (Rules::validateMove(gameBoard, tile, row, col)) {
            gameBoard->placeTile(row, col, tile);
            Tile *removedTile = player->removeTileFromHand(tile);
            if (removedTile != nullptr) {
              delete removedTile;
              Tile *newTile = tileBag->drawTile();
              if (newTile != nullptr) {
                player->addTileToHand(newTile);
              }
              int score = Rules::calculateScore(gameBoard, row, col);
              player->setScore(player->getScore() + score);
              if (score > 6) {
                std::cout << "QWIRKLE!!!" << std::endl;
              }
              validInput = true;
            } else {
              std::cout << "Error: Failed to remove tile from hand."
                        << std::endl;
            }
          } else {
            std::cout << "Invalid move. Try again." << std::endl;
            delete tile;
          }
        } else {
          std::cout << "You don't have that tile in your hand." << std::endl;
          delete tile;
        }
      } else {
        std::cout << "Invalid move format. Use 'place <tile> at <position>'."
                  << std::endl;
      }
    }
  }
}

void handleEnhancedPlayerTurn(Player* player, Player* opponent, TileBag* tileBag,
              GameBoard* gameBoard, bool& quit, bool enhanced) {
    bool validInput = false;
    while (!validInput && !quit) {
        std::cout << gameBoard->displayBoard(enhanced) << std::endl;
        std::cout << "Tiles in hand: " << player->getHand()->toString()
                  << std::endl;
        std::cout << "Your move " << player->getName() << ": ";
        std::string playerMove = handleInput(quit);

        if (playerMove == "quit" || quit) {
            quit = true;
            validInput = true;  // Quit is a valid input
        } else if (playerMove == "save") {
            std::cout << "Enter filename to save: ";
            std::string filename = handleInput(quit);

            FileHandler fileHandler;
            fileHandler.saveGame(filename, player, opponent, tileBag, gameBoard,
                                 player);
            std::cout << "Game saved to " << filename << std::endl;
            //validInput = true;  // If save action is a valid input
        } else if (playerMove.substr(0, 7) == "replace") {
            std::string tileToReplace = playerMove.substr(8);
            if (tileToReplace.size() == 2) {
                char colour = tileToReplace[0];
                int shape = tileToReplace[1] - '0';
                Tile* tile = new Tile(colour, shape);
                Tile* removedTile = player->removeTileFromHand(tile);
                if (removedTile != nullptr) {
                    std::cout << removedTile->print()
                              << " tile removed from hand and added to the bag."
                              << std::endl;
                    delete removedTile;
                    tileBag->addTile(tile);
                    Tile* newTile = tileBag->drawTile();
                    if (newTile != nullptr) {
                        player->addTileToHand(newTile);
                        std::cout << newTile->print()
                                  << " tile drawn and added to your hand." << std::endl;
                    } else {
                        std::cout << "No tiles left to draw from the tile bag."
                                  << std::endl;
                    }
                    validInput = true;
                } else {
                    std::cout << "You don't have that tile in your hand." << std::endl;
                }
            } else {
                std::cout << "Invalid tile format. Use <colour><shape>." << std::endl;
            }
        } else {
            std::stringstream stringstream(playerMove);
            std::string extractedWord;
            std::vector<std::string> moveBreakdown;
            bool allMovesValid = true;

            while (stringstream >> extractedWord) {
                moveBreakdown.push_back(extractedWord);

                // Check if we have a full move instruction
                if (moveBreakdown.size() >= 4 && moveBreakdown[0] == "place" &&
                    moveBreakdown[2] == "at") {
                    char tileColour = moveBreakdown[1][0];
                    int tileShape = moveBreakdown[1][1] - '0';
                    char rowChar = moveBreakdown[3][0];
                    int col = std::stoi(moveBreakdown[3].substr(1));

                    int row = rowChar - 'A';
                    Tile* tile = new Tile(tileColour, tileShape);

                    if (player->containsTile(tile)) {
                        if (Rules::validateMove(gameBoard, tile, row, col)) {
                            gameBoard->placeTile(row, col, tile);
                            Tile* removedTile = player->removeTileFromHand(tile);
                            if (removedTile != nullptr) {
                                delete removedTile;
                                Tile* newTile = tileBag->drawTile();
                                if (newTile != nullptr) {
                                    player->addTileToHand(newTile);
                                }
                                int score = Rules::calculateScore(gameBoard, row, col);
                                player->setScore(player->getScore() + score);
                                if (score > 6) {
                                    std::cout << "QWIRKLE!!!" << std::endl;
                                }
                            } else {
                                std::cout << "Error: Failed to remove tile from hand."
                                          << std::endl;
                                allMovesValid = false;
                            }
                        } else {
                            std::cout << "Invalid move at " << rowChar << col << ". Try again." << std::endl;
                            allMovesValid = false;
                        }
                    } else {
                        std::cout << "You don't have that tile in your hand: " 
                                  << tile->print() << std::endl;
                        allMovesValid = false;
                    }
                    moveBreakdown.clear(); // Clear to process the next move
                }
            }

            if (!allMovesValid) {
                std::cout << "Invalid move format. Use 'place <tile> at <position>' for single tile OR 'place <tile> at <position> place <tile> at <position>' for multi tile placement."
                          << std::endl;
            }

            validInput = allMovesValid; // Only valid if all moves were valid
        }
    }
}




void gameLoop(Player *player1, Player *player2, TileBag *tileBag,
              GameBoard *gameBoard, bool enhanced) {
  bool quit = false;
  while (!quit) {
    printScores(player1, player2, tileBag, gameBoard, quit, enhanced);

    // Check the enhanced mode and call the appropriate turn handler
    if (enhanced) {
      handleEnhancedPlayerTurn(player1, player2, tileBag, gameBoard, quit, enhanced);
    } else {
      playTurn(player1, player2, tileBag, gameBoard, quit, enhanced);
    }

    if (!quit) {
      printScores(player1, player2, tileBag, gameBoard, quit, enhanced);

      if (enhanced) {
        handleEnhancedPlayerTurn(player2, player1, tileBag, gameBoard, quit, enhanced);
      } else {
        playTurn(player2, player1, tileBag, gameBoard, quit, enhanced);
      }
    }
  }
}

void showCredits()
{
  std::cout << "---------------------------------------" << std::endl;
  for (const Student &student : students)
  {
    student.display();
  }
  std::cout << "---------------------------------------" << std::endl;
}

void handleMenuChoice(int choice, bool &quit, unsigned int randSeed,  bool enhanced)
{
  if (choice == 1)
  {
    startNewGame(quit, randSeed, enhanced);
  }
  else if (choice == 2)
  {
    loadGame(quit, enhanced);
  }
  else if (choice == 3)
  {
    showCredits();
  }
  else if (choice == 4)
  {
    quit = true;
  }
  else
  {
    std::cout << "Invalid choice. Please try again." << std::endl;
  }
}

void printScores(Player* player1, Player* player2, TileBag *tileBag, GameBoard *gameBoard, bool &quit, bool enhanced) {
    if (Rules::isGameOver(player1, player2, tileBag))
    {
      std::cout << gameBoard->displayBoard(enhanced) << std::endl;
      Player* winner = player1->getScore() > player2->getScore() ? player1 : player2;
      std::cout << "\nGame over!" << std::endl;
      std::cout << "The winner is " << winner->getName() << " with a score of " << winner->getScore() << "!\n" << std::endl;
      
      quit = true;
      return;
    }
    std::cout << std::endl; 
    std::cout << "Score for " << player1->getName() << ": " << player1->getScore() << std::endl;
    std::cout << "Score for " << player2->getName() << ": " << player2->getScore() << std::endl;
}

std::string handleInput(bool &quit)
{
  std::string input;
  std::getline(std::cin, input);
  std::cout << std::endl;

  // Check for EOF
  if (std::cin.eof())
  {
    quit = true;
  }
  return input;
}