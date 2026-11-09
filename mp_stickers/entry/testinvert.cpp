#include "Image.h"
#include <iostream>

class BadImage : public cs225::PNG {
public:
  void invert() {
    getPixel(0, 0).h += 5; // This better fail!
  }
};

int main() {

  std::cout << "On your own Image class, test_invert returns " << (test_invert<Image>() ? "true" : "false") << "\n";
  
  std::cout << "On the BadImage class, test_invert returns " << (test_invert<BadImage>() ? "true" : "false") << "\n";

  return 0;
}