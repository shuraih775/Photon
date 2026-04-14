#pragma once

#include <chrono>
#include <cstdint>
#include <string>
#include <unordered_map>
#include <vector>

#include <photon/core/model/model_handle.hpp>
#include <photon/core/response/inference_status.hpp>
#include <photon/core/tensor/tensor.hpp>

namespace photon::response {

class InferenceResult {
public:
  using Metadata = std::unordered_map<std::string, std::string>;

  InferenceResult() = default;

  // Status

  void setStatus(InferenceStatus status) { m_status = status; }

  [[nodiscard]]
  InferenceStatus status() const noexcept {
    return m_status;
  }

  [[nodiscard]]
  bool success() const noexcept {
    return m_status == InferenceStatus::Success;
  }

  // Model

  void setModel(photon::model::ModelHandle handle) { m_model = handle; }

  [[nodiscard]]
  const photon::model::ModelHandle &model() const noexcept {
    return m_model;
  }

  // Output Buffers

  void setOutputs(std::vector<photon::tensor::Tensor> outputs) {
    m_outputs = std::move(outputs);
  }

  [[nodiscard]]
  const std::vector<photon::tensor::Tensor> &outputs() const noexcept {
    return m_outputs;
  }

  // Latency

  void setLatency(std::chrono::microseconds latency) { m_latency = latency; }

  [[nodiscard]]
  std::chrono::microseconds latency() const noexcept {
    return m_latency;
  }

  // Metadata

  void addMetadata(std::string key, std::string value) {
    m_metadata.emplace(std::move(key), std::move(value));
  }

  [[nodiscard]]
  const Metadata &metadata() const noexcept {
    return m_metadata;
  }

private:
  InferenceStatus m_status{InferenceStatus::UnknownError};

  photon::model::ModelHandle m_model;

  std::vector<photon::tensor::Tensor> m_outputs;

  std::chrono::microseconds m_latency{0};

  Metadata m_metadata;
};

} // namespace photon::response