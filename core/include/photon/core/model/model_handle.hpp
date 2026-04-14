#pragma once

#include <cstdint>

namespace photon::model {

class ModelHandle {
public:
  using IdType = std::uint64_t;

  constexpr ModelHandle() noexcept = default;

  explicit constexpr ModelHandle(IdType id) noexcept : m_id(id) {}

  [[nodiscard]]
  constexpr IdType id() const noexcept {
    return m_id;
  }

  [[nodiscard]]
  constexpr bool valid() const noexcept {
    return m_id != InvalidId;
  }

  constexpr explicit operator bool() const noexcept { return valid(); }

  [[nodiscard]] constexpr bool
  operator==(const ModelHandle &) const noexcept = default;

private:
  static constexpr IdType InvalidId = 0;

  IdType m_id{InvalidId};
};

} // namespace photon::model