#pragma once

#include <string>
#include <vector>

#include <datatype.hpp>

namespace photon::model {

class TensorInfo {
public:
  TensorInfo() = default;

  TensorInfo(std::string name, std::vector<int64_t> shape, DataType dataType,
             bool dynamic = false)
      : m_name(std::move(name)), m_shape(std::move(shape)),
        m_dataType(dataType), m_dynamic(dynamic) {}

  [[nodiscard]]
  const std::string &name() const noexcept {
    return m_name;
  }

  [[nodiscard]]
  const std::vector<int64_t> &shape() const noexcept {
    return m_shape;
  }

  [[nodiscard]]
  DataType dataType() const noexcept {
    return m_dataType;
  }

  [[nodiscard]]
  bool isDynamic() const noexcept {
    return m_dynamic;
  }

private:
  std::string m_name;

  std::vector<int64_t> m_shape;

  DataType m_dataType;

  bool m_dynamic{false};
};

} // namespace photon::model