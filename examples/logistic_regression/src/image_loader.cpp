#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "../include/image_loader.hpp"

#include <algorithm>
#include <cmath>
#include <stdexcept>

namespace example {

Image loadGrayscaleImage(const std::string &path) {

  int width = 0;
  int height = 0;
  int channels = 0;

  unsigned char *data = stbi_load(path.c_str(), &width, &height, &channels, 1);

  if (!data) {
    throw std::runtime_error("Failed to load image: " + path);
  }

  Image image;
  image.width = width;
  image.height = height;
  image.channels = 1;

  image.pixels.resize(static_cast<std::size_t>(width) *
                      static_cast<std::size_t>(height));

  for (std::size_t i = 0; i < image.pixels.size(); ++i) {
    image.pixels[i] = static_cast<float>(data[i]) / 255.0f;
  }

  stbi_image_free(data);

  return image;
}

Image resizeTo28x28(const Image &image) {

  if (image.width == 28 && image.height == 28) {
    return image;
  }

  Image resized;
  resized.width = 28;
  resized.height = 28;
  resized.channels = 1;

  resized.pixels.resize(28 * 28);

  /*
   * Simple nearest-neighbor resize.
   *
   * This is sufficient for our first E2E example.
   * It is NOT necessarily the same preprocessing used
   * to train every MNIST model.
   */
  for (int y = 0; y < 28; ++y) {

    for (int x = 0; x < 28; ++x) {

      const int srcX = std::min(image.width - 1, (x * image.width) / 28);

      const int srcY = std::min(image.height - 1, (y * image.height) / 28);

      resized.pixels[static_cast<std::size_t>(y) * 28 +
                     static_cast<std::size_t>(x)] =
          image.pixels[static_cast<std::size_t>(srcY) * image.width +
                       static_cast<std::size_t>(srcX)];
    }
  }

  return resized;
}

std::vector<float> normalizeMNIST(const Image &image) {

  std::vector<float> result = image.pixels;

  /*
   * Image is already converted to [0, 1]
   * by loadGrayscaleImage().
   *
   * Keep this function separate because this is
   * where model-specific preprocessing belongs.
   */

  return result;
}

} // namespace example