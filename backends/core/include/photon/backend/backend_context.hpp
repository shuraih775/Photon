#pragma once

#include <filesystem>

namespace photon::backend {

class BackendContext {
public:
  void setCacheDirectory(std::filesystem::path path) {
    m_cacheDirectory = std::move(path);
  }

  [[nodiscard]]
  const std::filesystem::path &cacheDirectory() const noexcept {
    return m_cacheDirectory;
  }

private:
  std::filesystem::path m_cacheDirectory;
};

} // namespace photon::backend