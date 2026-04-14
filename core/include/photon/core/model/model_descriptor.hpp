#pragma once

#include <filesystem>
#include <iostream>
#include <string>
#include <vector>

#include <precision.hpp>

#include <photon/core/model/tensor_info.hpp>

namespace photon::model {

enum class ModelFormat {
  ONNX,
  TensorRTEngine,
  GGUF,
  TorchScript,
  OpenVINO,
  Custom
};

class ModelDescriptor {
public:
  ModelDescriptor() = default;

  [[nodiscard]]
  const std::string &name() const noexcept {
    return m_name;
  }

  void setName(std::string name) { m_name = std::move(name); }

  [[nodiscard]]
  const std::filesystem::path &path() const noexcept {
    return m_path;
  }

  void setPath(std::filesystem::path path) {
    std::cout << "Setting model path: " << path << std::endl;
    m_path = std::move(path);
  }

  [[nodiscard]]
  ModelFormat format() const noexcept {
    return m_format;
  }

  void setFormat(ModelFormat format) { m_format = format; }

  [[nodiscard]]
  Precision precision() const noexcept {
    return m_precision;
  }

  void setPrecision(Precision precision) { m_precision = precision; }

  [[nodiscard]]
  const std::vector<TensorInfo> &inputs() const noexcept {
    return m_inputs;
  }

  void addInput(TensorInfo tensor) { m_inputs.emplace_back(std::move(tensor)); }

  [[nodiscard]]
  const std::vector<TensorInfo> &outputs() const noexcept {
    return m_outputs;
  }

  void addOutput(TensorInfo tensor) {
    m_outputs.emplace_back(std::move(tensor));
  }

private:
  std::string m_name;

  std::filesystem::path m_path;

  ModelFormat m_format{ModelFormat::Custom};

  Precision m_precision;

  std::vector<TensorInfo> m_inputs;

  std::vector<TensorInfo> m_outputs;
};

} // namespace photon::model