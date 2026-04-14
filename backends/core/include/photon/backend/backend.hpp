#pragma once

#include <memory>
#include <string>

#include <photon/backend/backend_context.hpp>
#include <photon/backend/model_instance.hpp>

#include <photon/core/request/inference_request.hpp>
#include <photon/core/response/inference_result.hpp>

namespace photon::backend {

class IBackend {
public:
  virtual ~IBackend() = default;

  [[nodiscard]]
  virtual std::string name() const = 0;

  virtual bool initialize(BackendContext &context) = 0;

  virtual std::shared_ptr<ModelInstance>
  loadModel(const photon::model::ModelDescriptor &descriptor) = 0;

  virtual photon::response::InferenceResult
  infer(ModelInstance &model,
        const photon::request::InferenceRequest &request) = 0;

  virtual void shutdown() = 0;
};

using BackendPtr = std::shared_ptr<IBackend>;

} // namespace photon::backend