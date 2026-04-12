#pragma once

#include <cstdint>
#include <string>
#include <unordered_map>
#include <vector>

#include <photon/model/model_handle.hpp>
#include <photon/request/request_options.hpp>
#include <photon/tensor/tensor.hpp>

namespace photon::request {

class InferenceRequest {
public:
  using Metadata = std::unordered_map<std::string, std::string>;

  InferenceRequest() = default;

  // Model

  void setModel(photon::model::ModelHandle model) { m_model = model; }

  [[nodiscard]]
  const photon::model::ModelHandle &model() const noexcept {
    return m_model;
  }

  // Inputs

  void addInput(photon::tensor::Tensor tensor) {
    m_inputs.emplace_back(std::move(tensor));
  }

  [[nodiscard]]
  const std::vector<photon::tensor::Tensor> &inputs() const noexcept {
    return m_inputs;
  }

  // Metadata

  void addMetadata(std::string key, std::string value) {
    m_metadata.emplace(std::move(key), std::move(value));
  }

  [[nodiscard]]
  const Metadata &metadata() const noexcept {
    return m_metadata;
  }

  // Options

  void setOptions(RequestOptions options) { m_options = std::move(options); }

  [[nodiscard]]
  const RequestOptions &options() const noexcept {
    return m_options;
  }

private:
  photon::model::ModelHandle m_model;

  std::vector<photon::tensor::Tensor> m_inputs;

  Metadata m_metadata;

  RequestOptions m_options;
};

} // namespace photon::request