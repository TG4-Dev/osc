// app/log.h
#pragma once
#include "spdlog/logger.h"

namespace platform {
class Log {
public:
  static void Init();

  inline static std::shared_ptr<spdlog::logger> &GetCoreLogger() {
    return s_core_logger;
  }

  inline static std::shared_ptr<spdlog::logger> &GetClientLogger() {
    return s_client_logger;
  }

private:
  static std::shared_ptr<spdlog::logger> s_core_logger;
  static std::shared_ptr<spdlog::logger> s_client_logger;
};

// core::log::getcorelogger()->warn("init log");

#define TE_CORE_TRACE(...) ::platform::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define TE_CORE_INFO(...) ::platform::Log::GetCoreLogger()->info(__VA_ARGS__)
#define TE_CORE_WARN(...) ::platform::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define TE_CORE_ERROR(...) ::platform::Log::GetCoreLogger()->error(__VA_ARGS__)
#define TE_CORE_CRITICAL(...)                                                  \
  ::platform::Log::GetCoreLogger()->critical(__VA_ARGS__)

#define TE_TRACE(...) ::platform::Log::GetClientLogger()->trace(__VA_ARGS__)
#define TE_INFO(...) ::platform::Log::GetClientLogger()->info(__VA_ARGS__)
#define TE_WARN(...) ::platform::Log::GetClientLogger()->warn(__VA_ARGS__)
#define TE_ERROR(...) ::platform::Log::GetClientLogger()->error(__VA_ARGS__)
#define TE_CRITICAL(...)                                                       \
  ::platform::Log::GetClientLogger()->critical(__VA_ARGS__)
} // namespace platform
