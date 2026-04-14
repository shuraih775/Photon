#pragma once

#include <memory>

#include <photon/core/model/model_descriptor.hpp>
#include <photon/core/model/model_handle.hpp>

#include <photon/core/request/inference_request.hpp>
#include <photon/core/response/inference_result.hpp>

#include <photon/runtime/runtime_context.hpp>

namespace photon::runtime {

class RuntimeManager {
public:
  explicit RuntimeManager(std::shared_ptr<RuntimeContext> context);

  ~RuntimeManager();

  RuntimeManager(const RuntimeManager &) = delete;
  RuntimeManager &operator=(const RuntimeManager &) = delete;

  RuntimeManager(RuntimeManager &&) noexcept = default;
  RuntimeManager &operator=(RuntimeManager &&) noexcept = default;

  void initialize();

  void shutdown();

  [[nodiscard]]
  photon::model::ModelHandle
  registerModel(const photon::model::ModelDescriptor &descriptor);

  [[nodiscard]]
  photon::response::InferenceResult
  infer(const photon::request::InferenceRequest &request);

private:
  std::shared_ptr<RuntimeContext> m_context;

  photon::model::ModelHandle::IdType m_nextModelId{1};
};

} // namespace photon::runtime