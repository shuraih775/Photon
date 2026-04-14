#include <photon/backend/onnx/onnx_backend.hpp>
#include <photon/backend/onnx/onnx_model.hpp>

#include <photon/core/tensor/tensor.hpp>

#include <iostream>
#include <stdexcept>

namespace photon::backend::onnx {

ONNXBackend::ONNXBackend() = default;

ONNXBackend::~ONNXBackend() = default;

std::string ONNXBackend::name() const { return "ONNX Runtime"; }

bool ONNXBackend::initialize(BackendContext &) {
  if (m_initialized) {
    return true;
  }

  m_environment =
      std::make_unique<Ort::Env>(ORT_LOGGING_LEVEL_WARNING, "Photon");

  m_sessionOptions.SetGraphOptimizationLevel(
      GraphOptimizationLevel::ORT_ENABLE_ALL);

  m_initialized = true;

  return true;
}

std::shared_ptr<photon::backend::ModelInstance>
ONNXBackend::loadModel(const photon::model::ModelDescriptor &descriptor) {
  if (!m_initialized) {
    throw std::runtime_error("ONNX backend not initialized.");
  }

  auto session = std::make_unique<Ort::Session>(
      *m_environment, descriptor.path().string().c_str(), m_sessionOptions);

  return std::make_shared<ONNXModel>(descriptor, std::move(session));
}

namespace {

ONNXTensorElementDataType toOrtDataType(DataType dtype) {
  switch (dtype) {
  case DataType::DT_FLOAT32:
    return ONNX_TENSOR_ELEMENT_DATA_TYPE_FLOAT;

  case DataType::DT_INT32:
    return ONNX_TENSOR_ELEMENT_DATA_TYPE_INT32;

  case DataType::DT_INT64:
    return ONNX_TENSOR_ELEMENT_DATA_TYPE_INT64;

  case DataType::DT_UINT8:
    return ONNX_TENSOR_ELEMENT_DATA_TYPE_UINT8;

  case DataType::DT_INT16:
    return ONNX_TENSOR_ELEMENT_DATA_TYPE_INT16;

  case DataType::DT_INT8:
    return ONNX_TENSOR_ELEMENT_DATA_TYPE_INT8;

  case DataType::DT_BOOL:
    return ONNX_TENSOR_ELEMENT_DATA_TYPE_BOOL;

  case DataType::DT_FLOAT16:
    return ONNX_TENSOR_ELEMENT_DATA_TYPE_FLOAT16;

  default:
    throw std::runtime_error("Unsupported DataType for ONNX backend.");
  }
}

DataType fromOrtDataType(ONNXTensorElementDataType type) {
  switch (type) {
  case ONNX_TENSOR_ELEMENT_DATA_TYPE_FLOAT:
    return DataType::DT_FLOAT32;

  case ONNX_TENSOR_ELEMENT_DATA_TYPE_INT32:
    return DataType::DT_INT32;

  case ONNX_TENSOR_ELEMENT_DATA_TYPE_INT64:
    return DataType::DT_INT64;

  case ONNX_TENSOR_ELEMENT_DATA_TYPE_UINT8:
    return DataType::DT_UINT8;

  case ONNX_TENSOR_ELEMENT_DATA_TYPE_INT16:
    return DataType::DT_INT16;

  case ONNX_TENSOR_ELEMENT_DATA_TYPE_INT8:
    return DataType::DT_INT8;

  case ONNX_TENSOR_ELEMENT_DATA_TYPE_BOOL:
    return DataType::DT_BOOL;

  case ONNX_TENSOR_ELEMENT_DATA_TYPE_FLOAT16:
    return DataType::DT_FLOAT16;

  default:
    throw std::runtime_error("Unsupported ONNX output datatype.");
  }
}

std::size_t ortElementSize(ONNXTensorElementDataType type) {

  switch (type) {
  case ONNX_TENSOR_ELEMENT_DATA_TYPE_FLOAT:
    return sizeof(float);

  case ONNX_TENSOR_ELEMENT_DATA_TYPE_FLOAT16:
    return 2;

  case ONNX_TENSOR_ELEMENT_DATA_TYPE_INT8:
    return sizeof(int8_t);

  case ONNX_TENSOR_ELEMENT_DATA_TYPE_UINT8:
    return sizeof(uint8_t);

  case ONNX_TENSOR_ELEMENT_DATA_TYPE_INT16:
    return sizeof(int16_t);

  case ONNX_TENSOR_ELEMENT_DATA_TYPE_INT32:
    return sizeof(int32_t);

  case ONNX_TENSOR_ELEMENT_DATA_TYPE_INT64:
    return sizeof(int64_t);

  case ONNX_TENSOR_ELEMENT_DATA_TYPE_BOOL:
    return sizeof(bool);

  default:
    throw std::runtime_error("Unsupported ONNX output datatype.");
  }
}

} // namespace

photon::response::InferenceResult
ONNXBackend::infer(photon::backend::ModelInstance &instance,
                   const photon::request::InferenceRequest &request) {

  photon::response::InferenceResult result;

  const auto start = std::chrono::steady_clock::now();

  try {

    //  Validate model instance

    auto *onnxModel = dynamic_cast<ONNXModel *>(&instance);

    if (!onnxModel) {
      std::cerr << "[ONNXBackend] Model instance is not "
                   "an ONNX model.\n";

      result.setStatus(photon::response::InferenceStatus::InvalidModel);

      return result;
    }

    result.setModel(request.model());

    //  Validate inputs

    const auto &inputs = request.inputs();

    if (inputs.empty()) {
      std::cerr << "[ONNXBackend] No inputs provided "
                   "for inference.\n";

      result.setStatus(photon::response::InferenceStatus::InvalidRequest);

      return result;
    }

    Ort::Session &session = onnxModel->session();

    Ort::AllocatorWithDefaultOptions allocator;

    // Validate input count

    const std::size_t modelInputCount = session.GetInputCount();

    if (inputs.size() != modelInputCount) {

      std::cerr << "[ONNXBackend] Input count mismatch: "
                << "expected " << modelInputCount << ", got " << inputs.size()
                << '\n';

      result.setStatus(photon::response::InferenceStatus::InvalidRequest);

      return result;
    }

    // Build ONNX input tensors

    std::vector<Ort::Value> ortInputs;
    ortInputs.reserve(inputs.size());

    std::vector<const char *> inputNames;
    inputNames.reserve(inputs.size());

    /*
     * AllocatedStringPtr owns the strings.
     *
     * inputNames contains raw pointers into those strings,
     * therefore allocatedInputNames must remain alive until
     * Session::Run() completes.
     */
    std::vector<Ort::AllocatedStringPtr> allocatedInputNames;

    allocatedInputNames.reserve(inputs.size());

    for (std::size_t i = 0; i < inputs.size(); ++i) {

      const auto &tensor = inputs[i];

      if (tensor.data() == nullptr || tensor.bytes() == 0) {

        std::cerr << "[ONNXBackend] Input tensor "
                     "contains no data.\n";

        result.setStatus(photon::response::InferenceStatus::InvalidRequest);

        return result;
      }

      // Input name
      allocatedInputNames.emplace_back(
          session.GetInputNameAllocated(i, allocator));

      inputNames.push_back(allocatedInputNames.back().get());

      // Tensor metadata
      const auto shape = tensor.shape();

      const auto ortType = toOrtDataType(tensor.dtype());

      // CPU tensor
      Ort::MemoryInfo memoryInfo =
          Ort::MemoryInfo::CreateCpu(OrtArenaAllocator, OrtMemTypeDefault);

      ortInputs.emplace_back(Ort::Value::CreateTensor(
          memoryInfo, const_cast<void *>(tensor.data()), tensor.bytes(),
          shape.data(), shape.size(), ortType));
    }

    // Obtain output names

    const std::size_t outputCount = session.GetOutputCount();

    std::vector<const char *> outputNames;
    outputNames.reserve(outputCount);

    std::vector<Ort::AllocatedStringPtr> allocatedOutputNames;

    allocatedOutputNames.reserve(outputCount);

    for (std::size_t i = 0; i < outputCount; ++i) {

      allocatedOutputNames.emplace_back(
          session.GetOutputNameAllocated(i, allocator));

      outputNames.push_back(allocatedOutputNames.back().get());
    }

    // Execute inference

    auto ortOutputs =
        session.Run(Ort::RunOptions{nullptr},

                    inputNames.data(), ortInputs.data(), ortInputs.size(),

                    outputNames.data(), outputNames.size());

    // Convert ONNX outputs -> Photon tensors

    std::vector<photon::tensor::Tensor> inferenceOutputs;

    inferenceOutputs.reserve(ortOutputs.size());

    for (auto &output : ortOutputs) {

      if (!output.IsTensor()) {

        std::cerr << "[ONNXBackend] Model produced "
                     "a non-tensor output.\n";

        result.setStatus(photon::response::InferenceStatus::BackendFailure);

        return result;
      }

      auto tensorInfo = output.GetTensorTypeAndShapeInfo();

      const auto elementType = tensorInfo.GetElementType();

      const auto elementCount = tensorInfo.GetElementCount();

      const std::size_t elementSize = ortElementSize(elementType);

      const std::size_t byteCount = elementCount * elementSize;

      const void *outputData = output.GetTensorRawData();

      if (outputData == nullptr && byteCount != 0) {

        std::cerr << "[ONNXBackend] ONNX Runtime "
                     "returned null output data.\n";

        result.setStatus(photon::response::InferenceStatus::BackendFailure);

        return result;
      }

      /*
       * ONNX Runtime owns the memory behind outputData.
       *
       * We therefore copy the output into Photon-owned
       * storage before returning from this function.
       */
      std::vector<std::byte> buffer(byteCount);

      if (byteCount != 0) {
        std::memcpy(buffer.data(), outputData, byteCount);
      }

      const auto photonDataType = fromOrtDataType(elementType);

      auto shape = tensorInfo.GetShape();

      inferenceOutputs.emplace_back(photonDataType, std::move(shape),
                                    std::move(buffer));
    }

    // Store outputs in result

    result.setOutputs(std::move(inferenceOutputs));

    // Record latency

    const auto end = std::chrono::steady_clock::now();

    result.setLatency(
        std::chrono::duration_cast<std::chrono::microseconds>(end - start));

    result.setStatus(photon::response::InferenceStatus::Success);

    return result;

  }

  catch (const Ort::Exception &e) {

    std::cerr << "[ONNXBackend] ONNX Runtime error: " << e.what() << '\n';

    result.setStatus(photon::response::InferenceStatus::BackendFailure);

    return result;
  }

  catch (const std::exception &e) {

    std::cerr << "[ONNXBackend] Error: " << e.what() << '\n';

    result.setStatus(photon::response::InferenceStatus::BackendFailure);

    return result;
  }
}

void ONNXBackend::shutdown() {

  m_environment.reset();

  m_initialized = false;
}

} // namespace photon::backend::onnx