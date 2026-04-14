#pragma once

#include <cstdint>
#include <string>

namespace photon::runtime {

enum class LogLevel { Trace, Debug, Info, Warn, Error, Off };

class RuntimeConfig {
public:
  void setWorkerThreads(std::uint32_t workers) { m_workerThreads = workers; }

  [[nodiscard]]
  std::uint32_t workerThreads() const noexcept {
    return m_workerThreads;
  }

  void setLoggingEnabled(bool enabled) { m_loggingEnabled = enabled; }

  [[nodiscard]]
  bool loggingEnabled() const noexcept {
    return m_loggingEnabled;
  }

  void setLogLevel(LogLevel level) { m_logLevel = level; }

  [[nodiscard]]
  LogLevel logLevel() const noexcept {
    return m_logLevel;
  }

private:
  std::uint32_t m_workerThreads{1};

  bool m_loggingEnabled{true};

  LogLevel m_logLevel{LogLevel::Info};
};

} // namespace photon::runtime