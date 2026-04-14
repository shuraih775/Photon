#include <photon/sdk/runtime/runtime.hpp>

#include <memory>
#include <utility>

#include <photon/runtime/runtime_context.hpp>
#include <photon/runtime/runtime_manager.hpp>

namespace photon::sdk::runtime {

class Runtime::Impl {
public:
  Impl()
      : context(std::make_shared<photon::runtime::RuntimeContext>()),
        manager(context) {}

  std::shared_ptr<photon::runtime::RuntimeContext> context;
  photon::runtime::RuntimeManager manager;
};

Runtime::Runtime() : m_impl(std::make_unique<Impl>()) {
  m_impl->manager.initialize();
}

Runtime::~Runtime() = default;

Runtime::Runtime(Runtime &&) noexcept = default;

Runtime &Runtime::operator=(Runtime &&) noexcept = default;

photon::model::ModelHandle
Runtime::registerModel(const photon::model::ModelDescriptor &descriptor) {
  return m_impl->manager.registerModel(descriptor);
}

std::variant<photon::response::InferenceResult, photon::errors::RuntimeError>
Runtime::infer(const photon::request::InferenceRequest &request) {
  return m_impl->manager.infer(request);
}

void Runtime::shutdown() {
  if (m_impl) {
    m_impl->manager.shutdown();
  }
}

} // namespace photon::sdk::runtime