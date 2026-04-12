#pragma once

#include <cstddef>
#include <vector>

#include <datatype.hpp>
#include <shape.hpp>

namespace photon::tensor {

class Tensor {
public:
  Tensor() = default;

  Tensor(DataType dtype, Shape shape, void *data, size_t bytes)
      : m_dtype(dtype), m_shape(shape), m_data(data), m_bytes(bytes) {}

  [[nodiscard]] DataType dtype() const noexcept { return m_dtype; }

  [[nodiscard]] Shape shape() const noexcept { return m_shape; }

  [[nodiscard]] void *data() const noexcept { return m_data; }

  [[nodiscard]] size_t bytes() const noexcept { return m_bytes; }

private:
  DataType m_dtype;
  Shape m_shape;
  void *m_data;
  size_t m_bytes;
};

} // namespace photon::tensor
