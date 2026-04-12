#pragma once

#include <memory>
#include <string>

#include <photon/backend/backend.hpp>

namespace photon::backend {

class BackendFactory {
public:
  virtual ~BackendFactory() = default;

  [[nodiscard]]
  virtual std::string backendName() const = 0;

  [[nodiscard]]
  virtual std::unique_ptr<IBackend> create() const = 0;
};

} // namespace photon::backend