#include "Rules.h"

#include <set>

bool Rules::validateMove(GameBoard* board, Tile* tile, int x, int y) {
  return isValidPlacement(board, tile, x, y);
}

bool Rules::validateMoveEnhanced(
    GameBoard* board, const std::vector<Tile*>& tiles,
    const std::vector<std::pair<int, int>>& positions) {
  if (tiles.size() != positions.size()) {
    return false;  // Number of tiles must match number of positions
  }
  for (size_t i = 0; i < tiles.size(); ++i) {
    if (!isValidPlacement(board, tiles[i], positions[i].first,
                          positions[i].second)) {
      return false;  // Check each tile's placement validity
    }
  }
  return true;  // All tiles are validly placed
}

int Rules::calculateScore(GameBoard* board, int x, int y) {
  int rowTiles = 1;
  int colTiles = 1;

  // Check horizontal (row) tiles
  for (int i = x - 1; i >= 0 && board->getTile(i, y) != nullptr; --i) {
    rowTiles++;
  }
  for (int i = x + 1; i < board->getRows() && board->getTile(i, y) != nullptr;
       ++i) {
    rowTiles++;
  }

  // Check vertical (column) tiles
  for (int j = y - 1; j >= 0 && board->getTile(x, j) != nullptr; --j) {
    colTiles++;
  }
  for (int j = y + 1; j < board->getCols() && board->getTile(x, j) != nullptr;
       ++j) {
    colTiles++;
  }

  // Calculate the score
  int score = 0;
  if (rowTiles > 1) {
    score += rowTiles;
  }
  if (colTiles > 1) {
    score += colTiles;
  }

  // Check for QWIRKLEs and add bonus points if applicable
  if (rowTiles == 6) {
    score += 6;
  }
  if (colTiles == 6) {
    score += 6;
  }

  // If this is the first move, add one point
  if (score == 0) {
    score = 1;
  }

  return score;
}

bool Rules::isGameOver(Player* player1, Player* player2, TileBag* tileBag) {
  return (player1->getHand()->getHead() == nullptr &&
          player2->getHand()->getHead() == nullptr) &&
         tileBag->isEmpty();
}

bool Rules::isValidPlacement(GameBoard* board, Tile* tile, int x, int y) {
  // Check if the position is within the board's boundaries
  if (x < 0 || x >= board->getRows() || y < 0 || y >= board->getCols()) {
    return false;
  }

  // Allow the first move to be placed anywhere
  if (board->isEmpty()) {
    return true;
  }

  // Check if the position is already occupied
  if (board->getTile(x, y) != nullptr) {
    return false;
  }

  // Check if tile placement is valid
  if (isRowInvalid(board, tile, x, y)) {
    return false;
  }

  return true;
}

bool Rules::isRowInvalid(GameBoard* board, Tile* tile, int x, int y) {
  // Check if horizontal (row) already has the tile
  std::set<std::string> horizontalRowTileMatch;
  std::set<std::string> verticalRowTileMatch;

  // build set of matches for horizontal rows
  for (int i = x - 1; i >= 0 && board->getTile(i, y) != nullptr; --i) {
    horizontalRowTileMatch.insert(tile->matchType(*board->getTile(i, y)));
  }
  for (int i = x + 1; i < board->getRows() && board->getTile(i, y) != nullptr;
       ++i) {
    horizontalRowTileMatch.insert(tile->matchType(*board->getTile(i, y)));
  }

  // build set of matches for vertical rows
  for (int j = y - 1; j >= 0 && board->getTile(x, j) != nullptr; --j) {
    verticalRowTileMatch.insert(tile->matchType(*board->getTile(x, j)));
  }
  for (int j = y + 1; j < board->getCols() && board->getTile(x, j) != nullptr;
       ++j) {
    verticalRowTileMatch.insert(tile->matchType(*board->getTile(x, j)));
  }

  // Check if the tile does not have a valid match in either row
  if (horizontalRowTileMatch.count("no-match") > 0 ||
      verticalRowTileMatch.count("no-match") > 0) {
    return true;
  }
  // Check if the tile has a match where it is the same tile
  if (horizontalRowTileMatch.count("same") > 0 ||
      verticalRowTileMatch.count("same") > 0) {
    return true;
  }
  // Check if the tile has a match by both color and shape
  if (horizontalRowTileMatch.count("color") > 0 &&
      horizontalRowTileMatch.count("shape") > 0) {
    return true;
  }
  // Check if the tile has a match by both color and shape
  if (verticalRowTileMatch.count("color") > 0 &&
      verticalRowTileMatch.count("shape") > 0) {
    return true;
  }
  // Check if the tile has a match by both color and shape
  if (horizontalRowTileMatch.size() > 1 || verticalRowTileMatch.size() > 1) {
    return true;
  }

  // Check if the tile has no matches
  if (horizontalRowTileMatch.size() == 0 && verticalRowTileMatch.size() == 0) {
    return true;
  }

  return false;
}