#include <catch2/catch_test_macros.hpp>

#include "game.h"
#include <sstream>

// to prevent fudging by making operator== always return true
bool snapshotsEqual(const Snapshot& s1, const Snapshot& s2) {
  std::stringstream str1, str2;
  print(s1, str1);
  print(s2, str2);
  return str1.str() == str2.str();
}

TEST_CASE("Snapshot works as expected", "[weight=30][valgrind]")
{
  Snapshot snapshot(30, 30);
  for (unsigned i = 0; i < 30; i++) {
    for (unsigned j = 0; j < 30; j++) {
      bool val = !((i + j) % 5) || !(i % 7);
      snapshot.getCell(i, j) = val;
      REQUIRE(snapshot.getCell(i, j) == val);
    }
  }
}


TEST_CASE("Test run() (valgrind)", "[weight=20][valgrind]")
{
  Game game(readFromFile("../data/Ships/Glider.txt", 50, 50, 5, 5), 20);
  game.run();
}


TEST_CASE("Test run() for output (glider)", "[weight=30]")
{
  Snapshot start = readFromFile("../data/Ships/Glider.txt", 20, 20, 5, 5);
  Game game(start, 30);
  game.run();

  REQUIRE(snapshotsEqual(game.getRound(0), start));


  Snapshot end(20, 20);
  end.getCell(14, 13) = end.getCell(12, 14) = end.getCell(14, 14) = end.getCell(13, 15) = end.getCell(14, 15) = true;


  REQUIRE(snapshotsEqual(game.getRound(30), end));
}


TEST_CASE("Test run() for output (R-pent)", "[weight=30]")
{
  Snapshot start = readFromFile("../data/Methuselahs/R-pent.txt", 100, 100, 45, 45);
  Game game(start, 200);
  game.run();
  for (unsigned round = 0; round <= 200; round += 50) {
    std::string fname = "../data/Methuselahs/RP_Stages/RP_" + std::to_string(round) + ".txt";
    Snapshot correct = readFromFile(fname, 100, 100);
    REQUIRE(snapshotsEqual(correct, game.getRound(round)));
  }
}