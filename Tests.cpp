#include <fstream>
#include <iostream>
#include <sstream>

#include "FileHandler.h"
#include "TileBag.h"
#include "TileCodes.h"

class Tests {
 public:
  static void run() {
    std::cout << "Running tests\n" << std::endl;

    tileBagConstructorOverloadTest();
    tileBagShuffleTest();
    readFileContentTest();
    saveGameTest();
    // enhancedBoardTest();
    // handleEnhancedPlayerTurnTest();
  }

  static void tileBagConstructorOverloadTest() {
    std::cout << "#tileBagConstructorOverloadTest" << std::endl;
    bool enhanced = false;

    // given
    std::vector<Tile*> tileVector =
        std::vector<Tile*>({new Tile(RED, CIRCLE), new Tile(RED, STAR_4)});

    // when
    TileBag* tilebag = new TileBag(tileVector);

    // then
    std::string tileString = tilebag->getTiles()->toString(enhanced);
    std::cout << "Tilebag instantiated with tiles: " << tileString << std::endl;

    assert_equality("R1, R2", tileString);
  }

  static void tileBagShuffleTest() {
    std::cout << "#tileShuffleTest" << std::endl;
    bool enhanced = false;
    // given
    TileBag* tilebag = new TileBag();
    LinkedList* tiles = tilebag->getTiles();
    std::string tileString = tiles->toString(enhanced);
    int randSeed = (unsigned int)time(NULL);

    // when
    tilebag->shuffle(randSeed);
    std::string shuffledTileString = tiles->toString(enhanced);

    // then
    std::cout << "Original: " << tileString << std::endl;
    std::cout << "Shuffled: " << shuffledTileString << std::endl;

    assert_inequality(tileString, shuffledTileString);
  }

  static void readFileContentTest() {
    std::cout << "#readFileContentTest" << std::endl;
    // given
    FileHandler fileHandler;
    std::string savedGame =
        "ABBY\n3\nR1,G2\nBILL\n1\nB3,Y4\n12,12\nR1@A0,G2@B1,B3@C2,Y4@D3\nR1,G2,"
        "B3,Y4\nABBY";

    // when
    std::string fileContent =
        fileHandler.readFileContent("tests/stubs/load-game-test-stub.txt");

    // then
    std::cout << "File content: " << fileContent << std::endl;

    assert_equality(savedGame, fileContent);
  }

  static void saveGameTest() {
    std::cout << "#saveGameTest" << std::endl;
    // given
    Player player1("ALICE");
    Player player2("BOB");

    Tile* tile1 = new Tile('R', 1);  // Red, Shape 1
    Tile* tile2 = new Tile('G', 2);  // Green, Shape 2
    Tile* tile3 = new Tile('B', 3);  // Blue, Shape 3
    Tile* tile4 = new Tile('Y', 4);  // Yellow, Shape 4

    player1.addTileToHand(tile1);
    player2.addTileToHand(tile2);

    GameBoard board(6, 6);         // 6x6 board
    board.placeTile(3, 3, tile3);  // Place tile4 at D3

    std::vector<Tile*> tiles = {tile4};
    TileBag* tileBag = new TileBag(tiles);

    Player currentPlayer("ALICE");  // Example current player

    // when
    std::string filename = "./tests/stubs/save-game-test-stub.txt";
    FileHandler fileHandler;
    fileHandler.saveGame(filename, &player1, &player2, tileBag, &board,
                         &currentPlayer);

    // // then
    std::string fileContent =
        fileHandler.readFileContent("tests/stubs/save-game-test-stub.txt");
    std::string savedGame = "ALICE\n0\nR1\nBOB\n0\nG2\n6,6\nB3@D3\nY4\nALICE";

    assert_equality(savedGame, fileContent);
  }

  static void assert_equality(std::string expected, std::string actual) {
    if (expected != actual) {
      std::cout << "\033[91m" << "Failed \n" << "\033[0m" << std::endl;
      throw std::runtime_error("Expected: \n" + expected + "\nReceived: \n" +
                               actual);
    } else {
      std::cout << "\033[32m" << "Passed \n" << "\033[0m" << std::endl;
    }
  }

  static void assert_inequality(std::string expected, std::string actual) {
    if (expected == actual) {
      std::cout << "\033[91m" << "Failed \n" << "\033[0m" << std::endl;
      throw std::runtime_error("Expected: \n" + expected +
                               "\nNot to equal: \n" + actual);
    } else {
      std::cout << "\033[32m" << "Passed \n" << "\033[0m" << std::endl;
    }
  }

  static void enhancedBoardTest() {
    bool enhanced = true;
    std::cout << "#enhancedBoardTest" << std::endl;

    // given
    GameBoard board(4, 4);                   // Example board size
    Tile* tile1 = new Tile(RED, CIRCLE);     // Red, Shape 1
    Tile* tile2 = new Tile(GREEN, STAR_4);   // Green, Shape 2
    Tile* tile3 = new Tile(BLUE, DIAMOND);   // Blue, Shape 3
    Tile* tile4 = new Tile(YELLOW, SQUARE);  // Yellow, Shape 4

    board.placeTile(0, 0, tile1);
    board.placeTile(1, 1, tile2);
    board.placeTile(2, 2, tile3);
    board.placeTile(3, 3, tile4);

    // When
    std::string enhancedBoardOutput =
        board.displayBoard(enhanced);  // true for enhanced mode
    std::string expectedOutput =
        "   0  1  2  3 \n"
        "--------------\n"
        "A|\033[31mR1\033[0m|  |  |  |\n"   // Red color
        "B|  |\033[32mG2\033[0m|  |  |\n"   // Green color
        "C|  |  |\033[34mB3\033[0m|  |\n"   // Blue color
        "D|  |  |  |\033[33mY4\033[0m|\n";  // Yellow color

    // Then
    assert_equality(expectedOutput, enhancedBoardOutput);
  }

  static void handleEnhancedPlayerTurnTest() {
    std::cout << "#handleEnhancedPlayerTurnTest" << std::endl;

    // given
    Player player1("ALICE");
    Player player2("BOB");

    GameBoard board(6, 6);  // 6x6 board

    // Mocking the player input with a multi-tile placement
    std::string playerInput = "place R4 at A0 place G5 at B1 place B6 at C2";

    // Simulate the function handling player turn
    std::stringstream stringstream(playerInput);
    std::string extractedWord;
    std::vector<std::string> moveBreakdown;

    while (stringstream >> extractedWord) {
      moveBreakdown.push_back(extractedWord);

      // Check if we have a full move instruction
      if (moveBreakdown.size() == 4 && moveBreakdown[0] == "place" &&
          moveBreakdown[2] == "at") {
        // Extract tile and position
        std::string tileCode = moveBreakdown[1];
        char row = moveBreakdown[3][0];  // Extract row (A, B, C, etc.)
        int col = std::stoi(
            moveBreakdown[3].substr(1));  // Extract column (1, 2, 3, etc.)

        // Place tile on the board
        Tile* tile = new Tile(tileCode[0], std::stoi(tileCode.substr(1)));
        board.placeTile(row - 'A', col - 1, tile);

        // Clear to process the next move
        moveBreakdown.clear();
      }
    }

    // Then
    std::string expectedBoardOutput =
        "   0  1  2  3  4  5 \n"
        "--------------------\n"
        "A|R4|  |  |  |  |  |\n"
        "B|  |G5|  |  |  |  |\n"
        "C|  |  |B6|  |  |  |\n"
        "D|  |  |  |  |  |  |\n"
        "E|  |  |  |  |  |  |\n"
        "F|  |  |  |  |  |  |\n";

    std::string actualBoardOutput =
        board.displayBoard(false);  // false for base mode
    assert_equality(expectedBoardOutput, actualBoardOutput);
  }
};