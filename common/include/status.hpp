#pragma once

enum class Status {
  Success,

  InvalidArgument,

  NotFound,

  InternalError,

  Timeout,

  Cancelled,
};