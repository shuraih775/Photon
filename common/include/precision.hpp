#pragma once

#include <cstddef>
#include <string_view>

enum class Precision : int {
  UNKNOWN = 0,
  // Floating point types
  FP32,
  FP16,
  BF16,
  FP8_E4M3,
  FP8_E5M2,
  // Integer types
  INT64,
  INT32,
  INT8,
  INT4
};

inline constexpr std::size_t get_element_size(Precision precision) {
  switch (precision) {
  case Precision::FP32:
    return 4;
  case Precision::FP16:
    return 2;
  case Precision::BF16:
    return 2;
  case Precision::INT64:
    return 8;
  case Precision::INT32:
    return 4;
  case Precision::INT8:
    return 1;
  case Precision::FP8_E4M3:
    return 1;
  case Precision::FP8_E5M2:
    return 1;
  case Precision::INT4:
    return 0; // Handled as packed data
  default:
    return 0;
  }
}

inline constexpr std::string_view to_string(Precision precision) {
  switch (precision) {
  case Precision::FP32:
    return "FP32";
  case Precision::FP16:
    return "FP16";
  case Precision::BF16:
    return "BF16";
  case Precision::FP8_E4M3:
    return "FP8_E4M3";
  case Precision::FP8_E5M2:
    return "FP8_E5M2";
  case Precision::INT64:
    return "INT64";
  case Precision::INT32:
    return "INT32";
  case Precision::INT8:
    return "INT8";
  case Precision::INT4:
    return "INT4";
  default:
    return "UNKNOWN";
  }
}
