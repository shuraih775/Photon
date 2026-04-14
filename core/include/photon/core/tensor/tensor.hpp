#pragma once

#include <cstddef>
#include <cstdint>
#include <utility>
#include <vector>

#include <datatype.hpp>
#include <shape.hpp>

namespace photon::tensor {

class Tensor {
public:
  using Storage = std::vector<std::byte>;

  Tensor() = default;

  /*
   * The input buffer is moved into the Tensor, so the Tensor
   * becomes responsible for its lifetime.
   */
  Tensor(DataType dtype, Shape shape, Storage data)
      : m_dtype(dtype), m_shape(std::move(shape)), m_data(std::move(data)) {}

  [[nodiscard]]
  DataType dtype() const noexcept {
    return m_dtype;
  }

  [[nodiscard]]
  const Shape &shape() const noexcept {
    return m_shape;
  }

  [[nodiscard]]
  const void *data() const noexcept {
    return m_data.empty() ? nullptr : m_data.data();
  }

  [[nodiscard]]
  void *data() noexcept {
    return m_data.empty() ? nullptr : m_data.data();
  }

  [[nodiscard]]
  std::size_t bytes() const noexcept {
    return m_data.size();
  }

  [[nodiscard]]
  bool empty() const noexcept {
    return m_data.empty();
  }

  [[nodiscard]]
  const Storage &storage() const noexcept {
    return m_data;
  }

  [[nodiscard]]
  Storage &storage() noexcept {
    return m_data;
  }

private:
  DataType m_dtype{DataType::DT_INVALID};

  Shape m_shape;

  Storage m_data;
};

} // namespace photon::tensor