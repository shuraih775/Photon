#include <cstdint>
#include <cstring>
#include <iomanip>
#include <iostream>
#include <stdexcept>
#include <vector>

#include <photon/sdk/sdk.hpp>

#include "../include/image_loader.hpp"

namespace {

void printTensor(const photon::tensor::Tensor &tensor, std::size_t index) {

  std::cout << "\nOutput[" << index << "]\n";

  std::cout << "  Data type: " << static_cast<int>(tensor.dtype()) << '\n';

  std::cout << "  Shape: [";

  const auto &shape = tensor.shape();

  for (std::size_t i = 0; i < shape.size(); ++i) {

    if (i != 0)
      std::cout << ", ";

    std::cout << shape[i];
  }

  std::cout << "]\n";

  std::cout << "  Bytes: " << tensor.bytes() << '\n';

  if (tensor.data() == nullptr || tensor.bytes() == 0) {

    std::cout << "  Values: <empty>\n";
    return;
  }

  switch (tensor.dtype()) {

  case DataType::DT_FLOAT32: {

    const auto *values = static_cast<const float *>(tensor.data());

    const std::size_t count = tensor.bytes() / sizeof(float);

    std::cout << "  Values: [";

    for (std::size_t i = 0; i < count; ++i) {

      if (i != 0)
        std::cout << ", ";

      std::cout << std::fixed << std::setprecision(6) << values[i];
    }

    std::cout << "]\n";

    break;
  }

  case DataType::DT_INT64: {

    const auto *values = static_cast<const std::int64_t *>(tensor.data());

    const std::size_t count = tensor.bytes() / sizeof(std::int64_t);

    std::cout << "  Values: [";

    for (std::size_t i = 0; i < count; ++i) {

      if (i != 0)
        std::cout << ", ";

      std::cout << values[i];
    }

    std::cout << "]\n";

    break;
  }

  default:

    std::cout << "  Values: <unsupported display type>\n";

    break;
  }
}

int getPredictedClass(const photon::tensor::Tensor &output) {

  if (output.dtype() != DataType::DT_FLOAT32) {
    throw std::runtime_error("Expected FLOAT32 model output.");
  }

  const auto &shape = output.shape();

  if (shape.size() != 2 || shape[0] != 1) {

    throw std::runtime_error("Expected output shape [1, N].");
  }

  const auto *values = static_cast<const float *>(output.data());

  const std::size_t classCount = static_cast<std::size_t>(shape[1]);

  if (classCount == 0) {
    throw std::runtime_error("Model produced zero classes.");
  }

  std::size_t bestClass = 0;

  for (std::size_t i = 1; i < classCount; ++i) {

    if (values[i] > values[bestClass]) {
      bestClass = i;
    }
  }

  return static_cast<int>(bestClass);
}

} // namespace

int main() {

  try {

    photon::sdk::runtime::RuntimeConfig config;

    photon::sdk::runtime::ModelDescriptor model;

    photon::sdk::runtime::InferenceRequest request;

    // ----------------------------------------------------------
    // Model
    // ----------------------------------------------------------

    model.setName("mnist");

    model.setPath("/home/mohammed-shuraih/Desktop/Photon/"
                  "examples/logistic_regression/src/mnist.onnx");

    model.setFormat(photon::sdk::runtime::ModelFormat::ONNX);

    // ----------------------------------------------------------
    // Runtime
    // ----------------------------------------------------------

    auto runtime =
        photon::sdk::runtime::RuntimeBuilder().configuration(config).build();

    auto modelHandle = runtime.registerModel(model);

    request.setModel(modelHandle);

    // ----------------------------------------------------------
    // Load image
    // ----------------------------------------------------------

    const std::string imagePath =
        "/home/mohammed-shuraih/Desktop/Photon/"
        "examples/logistic_regression/data/digit7.png";

    std::cout << "Loading image: " << imagePath << '\n';

    auto image = example::loadGrayscaleImage(imagePath);

    std::cout << "Input image: " << image.width << "x" << image.height << '\n';

    // ----------------------------------------------------------
    // Resize
    // ----------------------------------------------------------

    image = example::resizeTo28x28(image);

    // ----------------------------------------------------------
    // Model preprocessing
    // ----------------------------------------------------------

    std::vector<float> inputData = example::normalizeMNIST(image);

    if (inputData.size() != 28 * 28) {

      std::cerr << "Invalid preprocessed image size: " << inputData.size()
                << '\n';

      return 1;
    }

    // ----------------------------------------------------------
    // Create Photon tensor
    //
    // [1, 1, 28, 28]
    // ----------------------------------------------------------

    std::vector<std::byte> storage(
        reinterpret_cast<std::byte *>(inputData.data()),
        reinterpret_cast<std::byte *>(inputData.data()) +
            inputData.size() * sizeof(float));

    photon::tensor::Tensor input(DataType::DT_FLOAT32, Shape{1, 1, 28, 28},
                                 std::move(storage));

    request.addInput(std::move(input));

    // ----------------------------------------------------------
    // Inference
    // ----------------------------------------------------------

    std::cout << "Running inference...\n";

    auto result = runtime.infer(request);

    if (std::holds_alternative<photon::sdk::runtime::RuntimeError>(result)) {

      const auto &error = std::get<photon::sdk::runtime::RuntimeError>(result);

      std::cerr << "Inference failed with error code: "
                << static_cast<int>(error.code()) << '\n';

      return 1;
    }

    const auto &inferenceResult =
        std::get<photon::sdk::runtime::InferenceResult>(result);

    // ----------------------------------------------------------
    // Result
    // ----------------------------------------------------------

    if (!inferenceResult.success()) {

      std::cerr << "Inference failed with status: "
                << static_cast<int>(inferenceResult.status()) << '\n';

      return 1;
    }

    std::cout << "Inference successful!\n";

    const auto &outputs = inferenceResult.outputs();

    std::cout << "Outputs: " << outputs.size() << '\n';

    for (std::size_t i = 0; i < outputs.size(); ++i) {

      printTensor(outputs[i], i);
    }

    // ----------------------------------------------------------
    // Prediction
    // ----------------------------------------------------------

    if (outputs.size() == 1) {

      const int predicted = getPredictedClass(outputs[0]);

      std::cout << "\nPredicted digit: " << predicted << '\n';
    }

    std::cout << "Latency: " << inferenceResult.latency().count() << " us\n";

    runtime.shutdown();

    return 0;

  } catch (const std::exception &e) {

    std::cerr << "Application error: " << e.what() << '\n';

    return 1;
  }
}