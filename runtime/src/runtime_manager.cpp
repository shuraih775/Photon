#include <photon/runtime/backend_bootstrap.hpp>
#include <photon/runtime/execution_engine_selector.hpp>
#include <photon/runtime/runtime_context.hpp>
#include <photon/runtime/runtime_manager.hpp>

#include <algorithm>
#include <iostream>
#include <stdexcept>

namespace photon::runtime {

RuntimeManager::RuntimeManager(std::shared_ptr<RuntimeContext> context)
    : m_context(std::move(context)) {}

RuntimeManager::~RuntimeManager() = default;

void RuntimeManager::initialize() {
  registerBuiltInBackends(m_context->backendRegistry());
}

void RuntimeManager::shutdown() {}

photon::model::ModelHandle RuntimeManager::registerModel(
    const photon::model::ModelDescriptor &descriptor) {
  if (!descriptor.name().empty()) {
    std::cout << "Registering model: " << descriptor.name() << std::endl;
  } else {
    std::cout << "Registering unnamed model." << std::endl;
  }
  if (descriptor.path().empty()) {
    throw std::runtime_error("Model path is empty.");
  }
  if (descriptor.format() == photon::model::ModelFormat::Custom) {
    throw std::runtime_error("Model format is not specified.");
  }
  if (!std::filesystem::exists(descriptor.path())) {
    throw std::runtime_error("Model file does not exist.");
  }
  photon::runtime::ExecutionEngineSelector selector;
  auto executionEngine =
      selector.select(descriptor, m_context->configuration());
  std::cout << "Selected execution engine: "
            << photon::backend::ExecutionEngineToString(executionEngine)
            << std::endl;
  auto *backend = m_context->backend(executionEngine);

  if (!backend) {
    auto instance = m_context->backendRegistry().create(executionEngine);

    if (!instance) {
      throw std::runtime_error(
          "No backend registered for requested execution engine.");
    }

    photon::backend::BackendContext backendContext;

    if (!instance->initialize(backendContext)) {
      throw std::runtime_error("Failed to initialize backend.");
    }

    backend = instance.get();

    m_context->registerBackend(executionEngine, std::move(instance));
  }

  auto loadedModel = backend->loadModel(descriptor);

  if (!loadedModel) {
    throw std::runtime_error("Backend failed to load model.");
  }

  photon::model::ModelHandle handle(m_nextModelId++);

  m_context->registerModel(handle, std::move(loadedModel));

  return handle;
}

photon::response::InferenceResult
RuntimeManager::infer(const photon::request::InferenceRequest &request) {
  auto *model = m_context->model(request.model());

  if (!model) {
    photon::response::InferenceResult result;

    result.setStatus(photon::response::InferenceStatus::InvalidModel);

    return result;
  }

  photon::runtime::ExecutionEngineSelector selector;
  auto executionEngine = model->backendName();

  auto *backend = m_context->backend(executionEngine);

  if (!backend) {
    photon::response::InferenceResult result;

    result.setStatus(photon::response::InferenceStatus::BackendUnavailable);

    return result;
  }

  return backend->infer(*model, request);
}

} // namespace photon::runtime