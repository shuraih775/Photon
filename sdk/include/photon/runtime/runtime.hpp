#pragma once

#include <memory>
#include <photon/errors/runtime_error.hpp>
#include <variant>

#include <photon/model/model_descriptor.hpp>
#include <photon/model/model_handle.hpp>

#include <photon/request/inference_request.hpp>
#include <photon/response/inference_result.hpp>

namespace photon::runtime {

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

} // namespace photon::runtime