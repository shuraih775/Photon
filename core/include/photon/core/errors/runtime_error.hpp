#pragma once

#include <photon/core/errors/error_code.hpp>
#include <stdexcept>
#include <string>

namespace photon::errors {

class RuntimeError : public std::runtime_error {
public:
  RuntimeError(RuntimeErrorCode code, std::string message)
      : std::runtime_error(std::move(message)), m_code(code) {}

  [[nodiscard]]
  RuntimeErrorCode code() const noexcept {
    return m_code;
  }

private:
  RuntimeErrorCode m_code;
};

} // namespace photon::errors