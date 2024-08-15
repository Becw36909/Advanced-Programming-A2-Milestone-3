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
#define NUM_BOARD_ROWS 12
#define NUM_BOARD_COLS 12
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
// bool handleSave(Player* player, Player* opponent, TileBag* tileBag, GameBoard* gameBoard, bool& quit);
// bool handleReplace(Player* player, TileBag* tileBag, const std::string& tileToReplace);
// bool handlePlaceCommand(Player* player, TileBag* tileBag, GameBoard* gameBoard, const std::string& playerMove);
// bool validateMove(Player* player, GameBoard* gameBoard, const std::string& move);
// std::vector<std::string> parseCommands(const std::string& input);

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
    std::string versionChoice;
    bool isEnhanced = false; // Default to false (Base Qwirkle)
    bool validChoice = false; // Flag to track if the choice is valid

    while (!validChoice) {
        std::cout << "Choose version to run:" << std::endl;
        std::cout << "1. Base Qwirkle" << std::endl;
        std::cout << "2. Enhanced Qwirkle" << std::endl;
        std::cout << "> ";

        std::getline(std::cin, versionChoice);

        if (versionChoice == "1") {
            isEnhanced = false; // Base Qwirkle
            validChoice = true; // Valid choice
        } else if (versionChoice == "2") {
            isEnhanced = true; // Enhanced Qwirkle
            validChoice = true; // Valid choice
        } else {
            std::cout << "Invalid choice. Please enter '1' for Base Qwirkle or '2' for Enhanced Qwirkle." << std::endl;
        }
    }

    return isEnhanced;
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



// void handleEnhancedPlayerTurn(Player *player, Player *opponent, TileBag *tileBag, GameBoard *gameBoard, bool &quit, bool enhanced) {
//     bool validInput = false;

//     while (!validInput && !quit) {
//         std::cout << gameBoard->displayBoard(enhanced) << std::endl;
//         std::cout << "Tiles in hand: " << player->getHand()->toString() << std::endl;
//         std::cout << "Your move " << player->getName() << ": ";
//         std::string playerMove = handleInput(quit);

//         if (playerMove == "quit" || quit) {
//             quit = true;
//             validInput = true;  // Quit is a valid input
//         } else if (playerMove == "save") {
//             std::cout << "Enter filename to save: ";
//             std::string filename = handleInput(quit);

//             FileHandler fileHandler;
//             fileHandler.saveGame(filename, player, opponent, tileBag, gameBoard, player);
//             std::cout << "Game saved to " << filename << std::endl;
//             validInput = true;  // Save action is a valid input

//         } else if (playerMove.substr(0, 7) == "replace") {
//       // Check if there's a tile specified after 'replace'
//       if (playerMove.length() > 7) {
//         std::string tileToReplace = playerMove.substr(8);

//         if (tileToReplace.empty()) {
//           std::cout << "Invalid input. Please specify a tile to replace. Use "
//                        "'replace <tile>' format."
//                     << std::endl;
//           // Ensure tile has a color and a shape
//         } else if (tileToReplace.size() == 2) {
//           char colour = tileToReplace[0];
//           int shape = tileToReplace[1] - '0';
//           Tile *tile = new Tile(colour, shape);
//           Tile *removedTile = player->removeTileFromHand(tile);

//           if (removedTile != nullptr) {
//             std::cout << removedTile->print()
//                       << " tile removed from hand and added to the bag."
//                       << std::endl;
//             delete removedTile;
//             tileBag->addTile(tile);
//             Tile *newTile = tileBag->drawTile();

//             if (newTile != nullptr) {
//               player->addTileToHand(newTile);
//               std::cout << newTile->print()
//                         << " tile drawn and added to your hand." << std::endl;
//             } else {
//               std::cout << "No tiles left to draw from the tile bag."
//                         << std::endl;
//             }
//             validInput = true;
//           } else {
//             std::cout << "You don't have that tile in your hand." << std::endl;
//           }
//         } else {
//           std::cout << "Invalid tile format. Use <colour><shape>." << std::endl;
//         }
//       } else {
//         std::cout << "Invalid input. Please specify a tile to replace. Use "
//                      "'replace <tile>' format."
//                   << std::endl;
//       }
//         } else if (playerMove.find("place") != std::string::npos) {
//             std::stringstream stringstream(playerMove);
//             std::string extractedWord;
//             std::vector<std::string> moveBreakdown;
//             std::vector<std::tuple<Tile*, int, int>> validMoves;
//             bool allMovesValid = true;

//             // Split the input and validate each command
//             while (stringstream >> extractedWord) {
//                 moveBreakdown.push_back(extractedWord);

//                 if (moveBreakdown.size() == 4 && moveBreakdown[0] == "place" && moveBreakdown[2] == "at") {
//                     char tileColour = moveBreakdown[1][0];
//                     int tileShape = moveBreakdown[1][1] - '0';
//                     char rowChar = moveBreakdown[3][0];
//                     int col = std::stoi(moveBreakdown[3].substr(1));
//                     int row = rowChar - 'A';
                    
//                     Tile* tile = new Tile(tileColour, tileShape);

//                     if (!player->containsTile(tile)) {
//                         std::cout << "You don't have that tile in your hand: " << tile->print() << std::endl;
//                         delete tile;
//                         allMovesValid = false;
//                     } else if (!Rules::validateMove(gameBoard, tile, row, col)) {
//                         std::cout << "Invalid move at " << rowChar << col << ". Try again." << std::endl;
//                         delete tile;
//                         allMovesValid = false;
//                     } else {
//                         validMoves.push_back(std::make_tuple(tile, row, col));
//                     }
                    
//                     // Prepare to process the next move
//                     moveBreakdown.clear();
//                 }
//             }

//             if (allMovesValid && moveBreakdown.empty()) {
//                 // Execute all valid moves
//                 for (const auto& move : validMoves) {
//                     Tile* tile;
//                     int row, col;
//                     std::tie(tile, row, col) = move;
//                     gameBoard->placeTile(row, col, tile);
//                     Tile* removedTile = player->removeTileFromHand(tile);

//                     if (removedTile != nullptr) {
//                         delete removedTile;
//                         Tile* newTile = tileBag->drawTile();
//                         if (newTile != nullptr) {
//                             player->addTileToHand(newTile);
//                         }
//                         int score = Rules::calculateScore(gameBoard, row, col);
//                         player->setScore(player->getScore() + score);
//                         if (score > 6) {
//                             std::cout << "QWIRKLE!!!" << std::endl;
//                         }
//                     } else {
//                         std::cout << "Error: Failed to remove tile from hand." << std::endl;
//                     }
//                 }

//                 validInput = true;  // End turn after valid moves are executed
//             } else {
//                 std::cout << "Invalid move format. Use 'place <tile> at <position>' for single tile OR 'place <tile> at <position> place <tile> at <position>' and so on for multi-tile placement." << std::endl;
//                 // Clean up any unused tile objects in validMoves
//                 for (const auto& move : validMoves) {
//                     Tile* tile;
//                     int row, col;
//                     std::tie(tile, row, col) = move;
//                     delete tile;
//                 }
//             }

//         } else {
//             std::cout << "Invalid input. Please enter a valid command ('place <tile> at <position>', 'replace <tile>', 'save', or 'quit')." << std::endl;
//         }
//     }
// }

void handleEnhancedPlayerTurn(Player *player, Player *opponent, TileBag *tileBag, GameBoard *gameBoard, bool &quit, bool enhanced) {
    bool validInput = false;

    while (!validInput && !quit) {
        std::cout << gameBoard->displayBoard(enhanced) << std::endl;
        std::cout << "Tiles in hand: " << player->getHand()->toString() << std::endl;
        std::cout << "Your move " << player->getName() << ": ";
        std::string playerMove = handleInput(quit);

        if (playerMove == "quit" || quit) {
            quit = true;
            validInput = true;  // Quit is a valid input
        } else if (playerMove == "save") {
            std::cout << "Enter filename to save: ";
            std::string filename = handleInput(quit);

            FileHandler fileHandler;
            fileHandler.saveGame(filename, player, opponent, tileBag, gameBoard, player);
            std::cout << "Game saved to " << filename << std::endl;
            validInput = true;  // Save action is a valid input

        } else if (playerMove.substr(0, 7) == "replace") {
            if (playerMove.length() > 7) {
                std::string tileToReplace = playerMove.substr(8);

                if (tileToReplace.empty()) {
                    std::cout << "Invalid input. Please specify a tile to replace. Use 'replace <tile>' format." << std::endl;
                } else if (tileToReplace.size() == 2) {
                    char colour = tileToReplace[0];
                    int shape = tileToReplace[1] - '0';
                    Tile *tile = new Tile(colour, shape);
                    Tile *removedTile = player->removeTileFromHand(tile);

                    if (removedTile != nullptr) {
                        std::cout << removedTile->print() << " tile removed from hand and added to the bag." << std::endl;
                        delete removedTile;
                        tileBag->addTile(tile);
                        Tile *newTile = tileBag->drawTile();

                        if (newTile != nullptr) {
                            player->addTileToHand(newTile);
                            std::cout << newTile->print() << " tile drawn and added to your hand." << std::endl;
                        } else {
                            std::cout << "No tiles left to draw from the tile bag." << std::endl;
                        }
                        validInput = true;
                    } else {
                        std::cout << "You don't have that tile in your hand." << std::endl;
                    }
                } else {
                    std::cout << "Invalid tile format. Use <colour><shape>." << std::endl;
                }
            } else {
                std::cout << "Invalid input. Please specify a tile to replace. Use 'replace <tile>' format." << std::endl;
            }
        } else if (playerMove.find("place") != std::string::npos) {
            std::stringstream stringstream(playerMove);
            std::string extractedWord;
            std::vector<std::string> moveBreakdown;
            std::vector<std::tuple<Tile*, int, int>> validMoves;
            bool allMovesValid = true;
            bool formatValid = true;
            std::string movesPlayed;

            // Split the input and validate each command
            while (stringstream >> extractedWord) {
                moveBreakdown.push_back(extractedWord);

                if (moveBreakdown.size() == 4 && moveBreakdown[0] == "place" && moveBreakdown[2] == "at") {
                    char tileColour = moveBreakdown[1][0];
                    int tileShape = moveBreakdown[1][1] - '0';
                    char rowChar = moveBreakdown[3][0];
                    int col = std::stoi(moveBreakdown[3].substr(1));
                    int row = rowChar - 'A';

                    Tile* tile = new Tile(tileColour, tileShape);
                    std::string moveCommand = "place " + std::string(1, tileColour) + std::to_string(tileShape) + " at " + rowChar + std::to_string(col);

                    if (!player->containsTile(tile)) {
                        std::cout << "You don't have that tile in your hand: " << tile->print() << std::endl;
                        delete tile;
                        allMovesValid = false;
                        // formatValid = false;  // Overall format becomes invalid
                    } else if (!Rules::validateMove(gameBoard, tile, row, col)) {
                        std::cout << "Invalid move at " << rowChar << col << ". Try again." << std::endl;
                        delete tile;
                        allMovesValid = false;
                        // formatValid = false;  // Overall format becomes invalid
                    } else {
                        validMoves.push_back(std::make_tuple(tile, row, col));
                        movesPlayed += moveCommand + " ";
                    }

                    moveBreakdown.clear();
                }
            }

            if (allMovesValid && moveBreakdown.empty() && formatValid) {
                // Execute all valid moves
                for (const auto& move : validMoves) {
                    Tile* tile;
                    int row, col;
                    std::tie(tile, row, col) = move;
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
                        std::cout << "Error: Failed to remove tile from hand." << std::endl;
                    }
                }

                std::cout << "Moves played: " << movesPlayed << std::endl;
                validInput = true;  // End turn after valid moves are executed
            } else {
                if (!formatValid) {
                    std::cout << "Invalid move format. Use 'place <tile> at <position>' for single tile OR 'place <tile> at <position> place <tile> at <position>' and so on for multi-tile placement." << std::endl;
                }
                // Clean up any unused tile objects in validMoves
                for (const auto& move : validMoves) {
                    Tile* tile;
                    int row, col;
                    std::tie(tile, row, col) = move;
                    delete tile;
                }
            }

        } else {
            std::cout << "Invalid input. Please enter a valid command ('place <tile> at <position>', 'replace <tile>', 'save', or 'quit')." << std::endl;
        }
    }
}





// void handleEnhancedPlayerTurn(Player* player, Player* opponent, TileBag* tileBag,
//                               GameBoard* gameBoard, bool& quit, bool enhanced) {
//     bool validInput = false;

//     while (!quit) {
//         std::cout << gameBoard->displayBoard(enhanced) << std::endl;
//         std::cout << "Tiles in hand: " << player->getHand()->toString() << std::endl;
//         std::cout << "Your move " << player->getName() << ": ";
//         std::string playerInput = handleInput(quit);

//         if (quit || playerInput == "quit") {
//             quit = true;
//         } else if (playerInput.substr(0, 4) == "save") {
//             handleSave(player, opponent, tileBag, gameBoard, quit);
//             validInput = false; // Continue to allow the user to make further moves
//         } else if (playerInput.substr(0, 7) == "replace") {
//             std::string tileToReplace = playerInput.substr(8);
//             validInput = handleReplace(player, tileBag, tileToReplace);
//         } else if (playerInput.find("place") != std::string::npos) {
//             // Split the input into commands
//             std::vector<std::string> commands = parseCommands(playerInput);

//             // Validate all commands
//             bool allMovesValid = true;
//             std::vector<std::string> invalidCommands;
//             for (const std::string& command : commands) {
//                 if (!InputValidator::isValidPlaceCommand(command) || !validateMove(player, gameBoard, command)) {
//                     allMovesValid = false;
//                     invalidCommands.push_back(command);
//                 }
//             }

//             if (allMovesValid) {
//                 // Apply moves if all are valid
//                 for (const std::string& command : commands) {
//                     if (handlePlaceCommand(player, tileBag, gameBoard, command)) {
//                         validInput = true;
//                     }
//                 }
//             } else {
//                 std::cout << "One or more moves were invalid. Please try again." << std::endl;
//                 for (const std::string& command : invalidCommands) {
//                     std::cout << "Invalid move detected: " << command << std::endl;
//                 }
//             }
//         } else {
//             std::cout << "Invalid input. Please enter a valid command ('place <tile> at <position>', 'replace <tile>', 'save', or 'quit')." << std::endl;
//         }

//         // Exit the loop only if valid input is made
//         if (validInput) {
//             return;
//         }
//     }
// }


// std::vector<std::string> parseCommands(const std::string& input) {
//     std::vector<std::string> commands;
//     std::stringstream stringstream(input);
//     std::string word;
//     std::string currentCommand;

//     while (stringstream >> word) {
//         if (word == "place") {
//             // Save the current command if not empty and start a new one
//             if (!currentCommand.empty()) {
//                 commands.push_back(currentCommand);
//                 currentCommand.clear();
//             }
//             currentCommand += word + " ";
//         } else {
//             currentCommand += word + " ";
//         }
//     }

//     // Add the last command if it's not empty
//     if (!currentCommand.empty()) {
//         commands.push_back(currentCommand);
//     }

//     return commands;
// }


// bool validateMove(Player* player, GameBoard* gameBoard, const std::string& move) {
//     std::stringstream stringstream(move);
//     std::string extractedWord;
//     std::vector<std::string> moveBreakdown;

//     while (stringstream >> extractedWord) {
//         moveBreakdown.push_back(extractedWord);

//         if (moveBreakdown.size() == 4 && moveBreakdown[0] == "place" && moveBreakdown[2] == "at") {
//             char tileColour = moveBreakdown[1][0];
//             int tileShape = moveBreakdown[1][1] - '0';
//             char rowChar = moveBreakdown[3][0];
//             int col = std::stoi(moveBreakdown[3].substr(1));

//             int row = rowChar - 'A';
//             Tile* tile = new Tile(tileColour, tileShape);

//             if (player->containsTile(tile)) {
//                 if (Rules::validateMove(gameBoard, tile, row, col)) {
//                     delete tile;
//                     return true;
//                 } else {
//                     delete tile;
//                     std::cout << "Invalid move at " << rowChar << col << ". Move rejected." << std::endl;
//                     return false;
//                 }
//             } else {
//                 delete tile;
//                 std::cout << "You don't have that tile in your hand: " << tile->print() << std::endl;
//                 return false;
//             }
//         }
//     }
//     std::cout << "Invalid move format. Use 'place <tile> at <position>'." << std::endl;
//     return false;
// }


// bool handleSave(Player* currentPlayer, Player* otherPlayer, TileBag* tileBag, GameBoard* gameBoard, bool& quit) {
//     std::cout << "Enter filename to save: ";
//     std::string filename = handleInput(quit);

//     FileHandler fileHandler;
//     fileHandler.saveGame(filename, currentPlayer, otherPlayer, tileBag, gameBoard, currentPlayer);
//     std::cout << "Game successfully saved to " << filename << std::endl;
//     return true;  // Save action is a valid input, but doesn't change the turn
// }


// bool handleReplace(Player* player, TileBag* tileBag, const std::string& tileToReplace) {
//     if (tileToReplace.empty()) {
//         std::cout << "Invalid input. Please specify a tile to replace. Use 'replace <tile>' format." << std::endl;
//         return false;
//     }

//     if (tileToReplace.size() == 2) {
//         char colour = tileToReplace[0];
//         int shape = tileToReplace[1] - '0';
//         Tile* tile = new Tile(colour, shape);
//         Tile* removedTile = player->removeTileFromHand(tile);

//         if (removedTile != nullptr) {
//             std::cout << removedTile->print() << " tile removed from hand and added to the bag." << std::endl;
//             delete removedTile;
//             tileBag->addTile(tile);
//             Tile* newTile = tileBag->drawTile();
//             if (newTile != nullptr) {
//                 player->addTileToHand(newTile);
//                 std::cout << newTile->print() << " tile drawn and added to your hand." << std::endl;
//             } else {
//                 std::cout << "No tiles left to draw from the tile bag." << std::endl;
//             }
//             return true;
//         } else {
//             std::cout << "You don't have that tile in your hand." << std::endl;
//         }
//     } else {
//         std::cout << "Invalid tile format. Use <colour><shape>." << std::endl;
//     }
//     return false;
// }

// bool handlePlaceCommand(Player* player, TileBag* tileBag, GameBoard* gameBoard, const std::string& playerMove) {
//     std::stringstream stringstream(playerMove);
//     std::string extractedWord;
//     std::vector<std::string> moveBreakdown;

//     while (stringstream >> extractedWord) {
//         moveBreakdown.push_back(extractedWord);

//         if (moveBreakdown.size() == 4 && moveBreakdown[0] == "place" && moveBreakdown[2] == "at") {
//             char tileColour = moveBreakdown[1][0];
//             int tileShape = moveBreakdown[1][1] - '0';
//             char rowChar = moveBreakdown[3][0];
//             int col = std::stoi(moveBreakdown[3].substr(1));

//             int row = rowChar - 'A';
//             Tile* tile = new Tile(tileColour, tileShape);

//             if (player->containsTile(tile)) {
//                 if (Rules::validateMove(gameBoard, tile, row, col)) {
//                     gameBoard->placeTile(row, col, tile);
//                     Tile* removedTile = player->removeTileFromHand(tile);
//                     if (removedTile != nullptr) {
//                         delete removedTile;
//                         Tile* newTile = tileBag->drawTile();
//                         if (newTile != nullptr) {
//                             player->addTileToHand(newTile);
//                         }
//                         int score = Rules::calculateScore(gameBoard, row, col);
//                         player->setScore(player->getScore() + score);
//                         if (score > 6) {
//                             std::cout << "QWIRKLE!!!" << std::endl;
//                         }
//                         delete tile;
//                         return true;
//                     } else {
//                         return false; 
//                     }
//                 } else {
//                     delete tile;
//                     return false; 
//                 }
//             } else {
//                 delete tile;
//                 return false; 
//             }
//         }
//     }

//     // If moveBreakdown does not meet criteria, return false
//     return false;
// }




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