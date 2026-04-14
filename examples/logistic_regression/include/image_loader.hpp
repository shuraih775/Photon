#pragma once

#include <cstddef>
#include <string>
#include <vector>

namespace example {

struct Image {
  int width;
  int height;
  int channels;

  std::vector<float> pixels;
};

Image loadGrayscaleImage(const std::string &path);

Image resizeTo28x28(const Image &image);

std::vector<float> normalizeMNIST(const Image &image);

} // namespace example