#pragma once

#include <memory>
#include <variant>

#include <photon/core/errors/runtime_error.hpp>
#include <photon/core/model/model_descriptor.hpp>
#include <photon/core/model/model_handle.hpp>
#include <photon/core/request/inference_request.hpp>
#include <photon/core/response/inference_result.hpp>

namespace photon::sdk::runtime {

using InferenceResult = photon::response::InferenceResult;
using InferenceRequest = photon::request::InferenceRequest;
using RuntimeError = photon::errors::RuntimeError;

class Runtime {
public:
  Runtime();
  ~Runtime();

  Runtime(const Runtime &) = delete;
  Runtime &operator=(const Runtime &) = delete;

  Runtime(Runtime &&) noexcept;
  Runtime &operator=(Runtime &&) noexcept;

  [[nodiscard]]
  photon::model::ModelHandle
  registerModel(const photon::model::ModelDescriptor &descriptor);

  [[nodiscard]]
  std::variant<photon::response::InferenceResult, photon::errors::RuntimeError>
  infer(const photon::request::InferenceRequest &request);

  void shutdown();

private:
  class Impl;
  std::unique_ptr<Impl> m_impl;
};

} // namespace photon::sdk::runtime