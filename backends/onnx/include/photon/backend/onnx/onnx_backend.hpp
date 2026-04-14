#pragma once

#include <memory>

#include <onnxruntime_cxx_api.h>

#include <photon/backend/backend.hpp>
#include <photon/backend/backend_context.hpp>

namespace photon::backend::onnx {

class ONNXBackend final : public photon::backend::IBackend {
public:
  ONNXBackend();

  ~ONNXBackend() override;

  ONNXBackend(const ONNXBackend &) = delete;
  ONNXBackend &operator=(const ONNXBackend &) = delete;

  ONNXBackend(ONNXBackend &&) noexcept = default;
  ONNXBackend &operator=(ONNXBackend &&) noexcept = default;

  [[nodiscard]]
  std::string name() const override;

  bool initialize(BackendContext &context) override;

  std::shared_ptr<ModelInstance>
  loadModel(const photon::model::ModelDescriptor &descriptor) override;

  photon::response::InferenceResult
  infer(ModelInstance &model,
        const photon::request::InferenceRequest &request) override;

  void shutdown() override;

private:
  bool m_initialized{false};

  std::unique_ptr<Ort::Env> m_environment;

  Ort::SessionOptions m_sessionOptions;
};

} // namespace photon::backend::onnx