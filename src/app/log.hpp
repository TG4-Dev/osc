// app/log.h
#pragma once
#include "spdlog/logger.h"

namespace app {
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

#define TE_CORE_TRACE(...) ::app::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define TE_CORE_INFO(...) ::app::Log::GetCoreLogger()->info(__VA_ARGS__)
#define TE_CORE_WARN(...) ::app::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define TE_CORE_ERROR(...) ::app::Log::GetCoreLogger()->error(__VA_ARGS__)
#define TE_CORE_CRITICAL(...)                                                  \
  ::core::Log::GetCoreLogger()->critical(__VA_ARGS__)

#define TE_TRACE(...) ::app::Log::GetClientLogger()->trace(__VA_ARGS__)
#define TE_INFO(...) ::app::Log::GetClientLogger()->info(__VA_ARGS__)
#define TE_WARN(...) ::app::Log::GetClientLogger()->warn(__VA_ARGS__)
#define TE_ERROR(...) ::app::Log::GetClientLogger()->error(__VA_ARGS__)
#define TE_CRITICAL(...) ::app::Log::GetClientLogger()->critical(__VA_ARGS__)
} // namespace app
