#include <photon/backend/backend.hpp>
#include <photon/backend/onnx/onnx_model.hpp>

namespace photon::backend::onnx {

ONNXModel::ONNXModel(photon::model::ModelDescriptor descriptor,
                     std::unique_ptr<Ort::Session> session)
    : m_descriptor(std::make_shared<const photon::model::ModelDescriptor>(
          std::move(descriptor))),
      m_session(std::move(session)) {}

std::shared_ptr<const photon::model::ModelDescriptor>
ONNXModel::descriptor() const {
  return m_descriptor;
}

photon::backend::ExecutionEngine ONNXModel::backendName() const {
  return photon::backend::ExecutionEngine::ONNXRuntime;
}

Ort::Session &ONNXModel::session() noexcept { return *m_session; }

const Ort::Session &ONNXModel::session() const noexcept { return *m_session; }

} // namespace photon::backend::onnx