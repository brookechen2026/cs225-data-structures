#include "cs225/HSLAPixel.h"
#include "../Point.h"

#include "ColorPicker.h"
#include "MyColorPicker.h"

using namespace cs225;

/**
 * Picks the color for pixel (x, y).
 * Using your own algorithm
 */
HSLAPixel MyColorPicker::getColor(unsigned x, unsigned y) {
  /* @todo [Part 3] */
  HSLAPixel pixel;

  pixel.h = (x * 5) % 60;
  pixel.s = 0.9;
  pixel.l = (y % 99) / 99;
  pixel.a = 0.8;

  return pixel;
}
