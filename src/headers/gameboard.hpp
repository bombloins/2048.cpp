#ifndef GAMEBOARD_H
#define GAMEBOARD_H

#include "point2d.hpp"
#include "tile.hpp"
#include <chrono>
#include <random>
#include <tuple>

class RandInt {
public:
  using clock = std::chrono::system_clock;
  RandInt() : dist{0, std::numeric_limits<int>::max()} {
    seed(clock::now().time_since_epoch().count());
  }
  RandInt(const int low, const int high) : dist{low, high} {
    seed(clock::now().time_since_epoch().count());
  }
  int operator()() { return dist(re); }
  void seed(const unsigned int s) { re.seed(s); }

private:
  std::minstd_rand re;
  std::uniform_int_distribution<> dist;
};

using tile_data_array_t = std::vector<Tile>;

struct gameboard_data_array_t {
  ull playsize{0};
  tile_data_array_t board{};
};

struct GameBoard {
  gameboard_data_array_t gbda;
  bool win{};
  bool moved{true};
  ull score{};
  ull largestTile{2};
  long long moveCount{-1};

  GameBoard() = default;
  explicit GameBoard(ull playsize)
      : gbda{playsize, tile_data_array_t(playsize * playsize)} {}
  explicit GameBoard(ull playsize, const tile_data_array_t &prempt_board)
      : gbda{playsize, prempt_board} {}

  friend bool hasWonOnGameboard(GameBoard gb);
  friend long long MoveCountOnGameBoard(GameBoard gb);
  friend std::string printStateOfGameBoard(GameBoard gb);

  friend void unblockTilesOnGameboard(GameBoard &gb);
  friend bool canMoveOnGameboard(GameBoard &gb);
  friend void registerMoveByOneOnGameboard(GameBoard &gb);

  friend bool addTileOnGameboard(GameBoard &gb);

  friend void tumbleTilesUpOnGameboard(GameBoard &gb);
  friend void tumbleTilesDownOnGameboard(GameBoard &gb);
  friend void tumbleTilesLeftOnGameboard(GameBoard &gb);
  friend void tumbleTilesRightOnGameboard(GameBoard &gb);
};

using load_gameboard_status_t = std::tuple<bool, GameBoard>;

std::ostream &operator<<(std::ostream &os, const GameBoard &gb);

#endif
