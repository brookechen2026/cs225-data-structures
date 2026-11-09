/**
 * @file game.h
 * Declaration of the Game class.
 */

#include "snapshot.h"
#include "animation/Animation.h"

#pragma once
/**
 * A simulation of the Game of Life that remembers the game's history.
 */
class Game {
public:
  Game(Snapshot start, unsigned totalRounds);
  void run();

  /**
   * Run and store one more step of the game.
   */
  void step();

  /**
   * Return the state of the game after some number of steps
   * @param idx The number of steps before the returned snapshot.
   */
  const Snapshot& getRound(unsigned idx);

  /**
   * Return an animation of all currently simulated rounds.
   * @param scale The side length of a single game square, in pixels.
   */
  Animation animate(unsigned scale = 1);

private:

  /**
   * Helper method to compute the next game state.
   */
  Snapshot getNext();
  Snapshot* history_;
  Snapshot current_;
  unsigned round_;
  unsigned totalRounds_;
};