#ifndef ASSIGN2_FILEHANDLER_H
#define ASSIGN2_FILEHANDLER_H

#include <string>
#include "Player.h"
#include "TileBag.h"
#include "GameBoard.h"

class FileHandler {
public:
    void saveGame(const std::string& filename, Player* player1, Player* player2, TileBag* tileBag, GameBoard* board, Player* currentPlayer);
    bool loadGame(const std::string& filename, Player* player1, Player* player2, TileBag* tileBag, GameBoard*& board, Player* currentPlayer);
    static bool fileExists(const std::string& filename);
    std::string readFileContent(const std::string& filename) const;

private:
    static std::string serialisePlayer(Player* player);
    static std::string serialiseTileBag(TileBag* tileBag);
    static std::string serialiseBoard(GameBoard* board);
    std::string serialiseCurrentPlayer(Player* currentPlayer);

    static void deserialisePlayer(Player* player, const std::string& data);
    static void deserialiseTileBag(TileBag* tileBag, const std::string& data);
    GameBoard* deserialiseBoard(const std::string& data);
    void deserialiseCurrentPlayer(Player* currentPlayer, const std::string& data);

    static std::vector<Tile*> linkedListToVector(LinkedList* list);
    static void vectorToLinkedList(const std::vector<Tile*>& vec, LinkedList* list);
};

#endif // ASSIGN2_FILEHANDLER_H
