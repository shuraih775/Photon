#pragma once

#include <chrono>
#include <cstdint>

namespace photon::request {

enum class RequestPriority : std::uint8_t { Low = 0, Normal, High, Critical };

class RequestOptions {
public:
  RequestOptions() = default;

  void setPriority(RequestPriority priority) { m_priority = priority; }

  [[nodiscard]]
  RequestPriority priority() const noexcept {
    return m_priority;
  }

  void setTimeout(std::chrono::milliseconds timeout) { m_timeout = timeout; }

  [[nodiscard]]
  std::chrono::milliseconds timeout() const noexcept {
    return m_timeout;
  }

  void setBatchable(bool batchable) { m_batchable = batchable; }

  [[nodiscard]]
  bool batchable() const noexcept {
    return m_batchable;
  }

private:
  RequestPriority m_priority{RequestPriority::Normal};

  std::chrono::milliseconds m_timeout{0};

  bool m_batchable{true};
};

} // namespace photon::request