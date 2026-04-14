enum class RuntimeErrorCode {
  InvalidConfiguration,

  InvalidModel,

  ModelNotFound,

  InvalidRequest,

  BackendFailure,

  SchedulerFailure,

  ComputeFailure,

  MemoryFailure,

  Timeout,

  InternalError
};