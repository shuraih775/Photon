#pragma once

#include <cstdint>

namespace photon::response {

enum class InferenceStatus : std::uint8_t {
  Success = 0,

  InvalidRequest,

  InvalidModel,

  InvalidInput,

  ModelNotLoaded,

  BackendUnavailable,

  BackendFailure,

  Timeout,

  Cancelled,

  RuntimeError,

  UnknownError
};

} // namespace photon::response