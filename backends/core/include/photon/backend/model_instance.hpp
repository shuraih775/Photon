#pragma once

#include <memory>

#include <photon/model/model_descriptor.hpp>

namespace photon::backend {

class ModelInstance {
public:
  virtual ~ModelInstance() = default;

  [[nodiscard]]
  virtual const photon::model::ModelDescriptor &descriptor() const = 0;
};

using ModelInstancePtr = std::shared_ptr<ModelInstance>;

} // namespace photon::backend