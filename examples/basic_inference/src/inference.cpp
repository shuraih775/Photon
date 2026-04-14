#include <cstring>
#include <iomanip>
#include <iostream>
#include <vector>

#include <photon/sdk/sdk.hpp>

int main() {
  using namespace photon::sdk::runtime;

  RuntimeConfig config;
  ModelDescriptor model;

  model.setName("matmul_1");
  model.setPath("/home/mohammed-shuraih/Desktop/Photon/"
                "examples/basic_inference/src/matmul_1.onnx");
  model.setFormat(ModelFormat::ONNX);

  auto runtime = RuntimeBuilder().configuration(config).build();

  // Register model

  auto modelHandle = runtime.registerModel(model);

  std::cout << "Model registered.\n";

  // Construct inference request

  InferenceRequest request;
  request.setModel(modelHandle);

  float inputData[] = {1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f};

  std::vector<std::byte> inputStorage(sizeof(inputData));

  std::memcpy(inputStorage.data(), inputData, sizeof(inputData));

  photon::tensor::Tensor input(DataType::DT_FLOAT32, Shape{3, 2},
                               std::move(inputStorage));

  request.addInput(std::move(input));

  // Execute inference

  auto result = runtime.infer(request);

  if (std::holds_alternative<RuntimeError>(result)) {
    const auto &error = std::get<RuntimeError>(result);

    std::cerr << "Inference failed with error code: "
              << static_cast<int>(error.code()) << '\n';

    runtime.shutdown();
    return 1;
  }

  const auto &inferenceResult = std::get<InferenceResult>(result);

  if (!inferenceResult.success()) {
    std::cerr << "Inference failed with status: "
              << static_cast<int>(inferenceResult.status()) << '\n';

    runtime.shutdown();
    return 1;
  }

  // Print results

  std::cout << "\nInference successful!\n";

  const auto &outputs = inferenceResult.outputs();

  std::cout << "Outputs: " << outputs.size() << '\n';

  for (std::size_t i = 0; i < outputs.size(); ++i) {
    const auto &output = outputs[i];

    std::cout << "\nOutput[" << i << "]\n";

    std::cout << "  dtype: " << static_cast<int>(output.dtype()) << '\n';

    std::cout << "  shape: [";

    const auto &shape = output.shape();

    for (std::size_t j = 0; j < shape.size(); ++j) {
      if (j != 0)
        std::cout << ", ";

      std::cout << shape[j];
    }

    std::cout << "]\n";

    std::cout << "  bytes: " << output.bytes() << '\n';

    // v0.1: We know this particular model produces FP32.

    if (output.dtype() == DataType::DT_FLOAT32) {
      const auto *data = static_cast<const float *>(output.data());

      const std::size_t elementCount = output.bytes() / sizeof(float);

      std::cout << "  values: [";

      for (std::size_t j = 0; j < elementCount; ++j) {
        if (j != 0)
          std::cout << ", ";

        std::cout << std::fixed << std::setprecision(4) << data[j];
      }

      std::cout << "]\n";
    }

    std::cout << "  latency: " << inferenceResult.latency().count() << " us\n";
  }

  runtime.shutdown();

  return 0;
}