#pragma once

#include <memory>

#include <onnxruntime_cxx_api.h>

#include <photon/backend/model_instance.hpp>

namespace photon::backend::onnx {

class ONNXModel final : public photon::backend::ModelInstance {
public:
  ONNXModel(photon::model::ModelDescriptor descriptor,
            std::unique_ptr<Ort::Session> session);

  ~ONNXModel() override = default;

  [[nodiscard]]
  std::shared_ptr<const photon::model::ModelDescriptor>
  descriptor() const override;

  [[nodiscard]]
  photon::backend::ExecutionEngine backendName() const override;

  [[nodiscard]]
  Ort::Session &session() noexcept;

  [[nodiscard]]
  const Ort::Session &session() const noexcept;

private:
  std::shared_ptr<const photon::model::ModelDescriptor> m_descriptor;

  std::unique_ptr<Ort::Session> m_session;
};

} // namespace photon::backend::onnx