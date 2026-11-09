/**
 * @file game.cpp
 * Implementation of the Game class.
 */

#include "game.h"

bool nextCell(const Snapshot& ss, unsigned x, unsigned y) {
  int aliveNbs = 0;
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++) {
      unsigned nx = x + i - 1;
      unsigned ny = y + j - 1;
      if (nx >= ss.width() || ny >= ss.height()) {
        continue;
      }
      aliveNbs += ss.getCell(nx, ny);
    }
  }
  aliveNbs -= ss.getCell(x, y);
  return aliveNbs == 3 || (ss.getCell(x, y) && aliveNbs == 2);
}

Game::Game(Snapshot start, unsigned totalRounds) {
  round_ = 0;
  current_ = start;
  history_ = new Snapshot[totalRounds_];
}


void Game::run() {
  while (round_ != totalRounds_) step();
}

const Snapshot& Game::getRound(unsigned idx) {
  if (idx == round_) return current_;
  return history_[idx];
}

Snapshot Game::getNext() {
  Snapshot next = current_;
  for (unsigned x = 0; x < next.width(); x++) {
    for (unsigned y = 0; y < next.height(); y++) {
      next.getCell(x, y) = nextCell(current_, x, y);
    }
  }
  return next;
}

void Game::step() {
  if (round_ == totalRounds_) return;
  Snapshot next = getNext();
  history_[++round_] = current_;
  current_ = next;
}

Animation Game::animate(unsigned scale) {
  Animation ani;
  for (unsigned i = 0; i < round_; i++) {
    ani.addFrame(getRound(i).render(scale));
  }
  ani.addFrame(current_.render(scale));
  return ani;
}