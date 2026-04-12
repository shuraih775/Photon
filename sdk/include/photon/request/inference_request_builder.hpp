#pragma once

#include <string>
#include <utility>

#include <photon/model/model_handle.hpp>
#include <photon/request/inference_request.hpp>
#include <photon/request/request_options.hpp>
#include <photon/tensor/tensor.hpp>

namespace photon::request {

class InferenceRequestBuilder {
public:
  InferenceRequestBuilder() = default;

  InferenceRequestBuilder &model(photon::model::ModelHandle model) {
    m_request.setModel(model);
    return *this;
  }

  InferenceRequestBuilder &input(photon::tensor::Tensor tensor) {
    m_request.addInput(std::move(tensor));
    return *this;
  }

  InferenceRequestBuilder &metadata(std::string key, std::string value) {
    m_request.addMetadata(std::move(key), std::move(value));

    return *this;
  }

  InferenceRequestBuilder &options(RequestOptions options) {
    m_request.setOptions(std::move(options));
    return *this;
  }

  [[nodiscard]]
  InferenceRequest build() {
    return std::move(m_request);
  }

private:
  InferenceRequest m_request;
};

} // namespace photon::request