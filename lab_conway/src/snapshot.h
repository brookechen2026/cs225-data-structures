/**
 * @file snapshot.h
 * Declaration of the Snapshot class.
 */

#include <iostream>
#include <assert.h>
#include <fstream>
#include "cs225/PNG.h"

#pragma once
/**
 * A single instant (snapshot) in the Game of Life board
 */
class Snapshot {
public:
  Snapshot();

  /**
   * Constructor for an all-dead snapshot.
   * @param width The width of the board in cells.
   * @param height The height of the board in cells.
   */
  Snapshot(unsigned width, unsigned height);

  Snapshot(const Snapshot& other);

  ~Snapshot();

  Snapshot& operator=(const Snapshot& other);

  bool operator==(const Snapshot& other) const;

  unsigned width() const { return width_; };
  unsigned height() const { return height_; };

  bool& getCell(unsigned x, unsigned y);
  bool getCell(unsigned x, unsigned y) const;
  
  /**
   * Produce a png image of the snapshop
   * @param idx The side length of a game square, in pixels.
   */
  cs225::PNG render(unsigned scale = 1) const;

private:
  bool& _getCell(unsigned x, unsigned y) const;
  void destroy();
  void copy(const Snapshot& other);
  unsigned width_, height_;
  bool* cells_;
};

inline Snapshot readFromFile(std::string filename, unsigned width, unsigned height, unsigned x = 0, unsigned y = 0) {
  Snapshot ss(width, height);
  std::ifstream file(filename);
  if (!file) {
    std::cerr << "File not found: " << filename << std::endl;
    return Snapshot(width, height);
  }
  string line;
  unsigned row = y;
  while (getline(file, line)) {
    // Empty lines and comments
    if (line == "") continue;
    if (line.size() >= 2 && line[0] == '/' && line[1] == '/') continue;

    unsigned col = x;
    for (char c : line) {
      if (c == ' ') continue;
      if (c == '*') ss.getCell(col, row) = true;
      col++;
    }
    row++;
  }

  return ss;
}

/**
 * Print an ascii render of the snapshot.
 */
inline void print(const Snapshot& ss, std::ostream& stream) {
  for (unsigned y = 0; y < ss.height(); y++) {
    for (unsigned x = 0; x < ss.width(); x++) {
      if (ss.getCell(x, y)) stream << "*";
      else stream << "_";
      if (x != ss.width() - 1) stream << " ";
      else stream << "\n";
    }
  }
}