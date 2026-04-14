#include <photon/sdk/runtime/runtime_builder.hpp>

#include <utility>

namespace photon::sdk::runtime {

Runtime RuntimeBuilder::build() {
  Runtime runtime;

  for (const auto &descriptor : m_models) {
    runtime.registerModel(descriptor);
  }

  return runtime;
}

} // namespace photon::sdk::runtime