/**
 * @file snapshot.cpp
 * Implementation of the Snapshot class.
 */

#include "snapshot.h"

Snapshot::Snapshot(): width_(0), height_(0), cells_(nullptr) {}

Snapshot::Snapshot(unsigned width, unsigned height): width_(width), height_(height) {
  cells_ = new bool[width_ * height_];
  for (unsigned i = 0; i < width_ * height_; i++) {
    cells_[i] = false;
  }
}

Snapshot::Snapshot(const Snapshot& other) {
  copy(other);
}

Snapshot& Snapshot::operator=(const Snapshot& other) {
  if (this != &other) {
    destroy();
    copy(other);
  }
  return *this;
}

Snapshot::~Snapshot() {
  destroy();
}

bool Snapshot::operator==(const Snapshot& other) const {
  if (width() != other.width() || height() != other.height()) return false;
  for (unsigned x = 0; x < width(); x++) {
    for (unsigned y = 0; y < width(); y++) {
      if (getCell(x, y) != other.getCell(x, y)) return false;
    }
  }
  return true;
}

bool& Snapshot::_getCell(unsigned x, unsigned y) const {
  if (x >= width_ || y >= height_) {
    std::cerr << "ERROR: Call to Snapshot::getCell("
    << x << "," << y << ") out of bounds (height = " << height() << ", width = " << width() << ")" << std::endl;
    assert(false);
  }
  return cells_[height_ * x + y];
}

bool& Snapshot::getCell(unsigned x, unsigned y) {
  return _getCell(x, y);
}

bool Snapshot::getCell(unsigned x, unsigned y) const {
  return _getCell(x, y);
}


void Snapshot::destroy() {
  if (cells_ != nullptr) {
    delete cells_;
  }
  cells_ = nullptr;
}
void Snapshot::copy(const Snapshot& other) {
  width_ = other.width_;
  height_ = other.height_;
  cells_ = other.cells_;
}

cs225::PNG Snapshot::render(unsigned scale) const {
  cs225::PNG png(width_ * scale, height_ * scale);
  for (unsigned x = 0; x < width_; x++) {
    for (unsigned y = 0; y < height_; y++) {
      for (unsigned i = 0; i < scale; i++) {
        for (unsigned j = 0; j < scale; j++) {
          if (getCell(x, y)) {
            png.getPixel(scale * x + i, scale * y + j) = cs225::HSLAPixel(0, 0, 1, 1);
          } else {
            png.getPixel(scale * x + i, scale * y + j) = cs225::HSLAPixel(0, 0, 0, 1);
          }
        }
      }
    }
  }
  return png;
}

