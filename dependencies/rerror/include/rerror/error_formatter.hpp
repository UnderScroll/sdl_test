#pragma once

#include <functional>
#include <optional>
#include <unders_helpers/term_colors.hpp>

#include "rerror/error.hpp"
#include "rerror/location_formatter.hpp"

// re::AnyError std::formatter template specialization
// Formatting:
// - [default]: [ERROR:Kind] message (location)
// - [?]      : [ERROR:Kind] message (location)
//              ├╴[CAUSE:Kind] message (location)
//              ├╴...
//              └╴[CAUSE:Kind] message (location)
template <>
struct std::formatter<re::AnyError> {
  // Pretty mode ['#'](default: false):
  // - Displays colors
  bool pretty = false;
  // Verbose mode ['?'](default: false):
  // - Displays error cause(s)
  bool verbose = false;

  // Color settings
  const char* DEFAULT_COLOR = tcolor::RESET;
  const char* LOW_COLOR = tcolor::BLACK;
  const char* ERROR_COLOR = tcolor::BOLD_RED;
  const char* ERROR_KIND_COLOR = tcolor::BLUE;
  const char* CAUSE_COLOR = tcolor::RED;

  constexpr auto parse(std::format_parse_context& ctx) {
    auto it = ctx.begin();

    for (; it != ctx.end() && *it != '}'; it++) {
      if (*it == '#')
        pretty = true;
      if (*it == '?')
        verbose = true;
    }

    return it;
  }

  auto format(const re::AnyError& value, std::format_context& ctx) const {
    std::string displayed_error = pretty ? std::format("[{3}ERROR{5}:{4}{0}{6}] {1} ({2:#})",
                                                       /*0*/ value->kind_name(), /*1*/ value->message(), /*2*/ value->location(),
                                                       /*3*/ ERROR_COLOR, /*4*/ ERROR_KIND_COLOR, /*5*/ LOW_COLOR, /*6*/ DEFAULT_COLOR)
                                         : std::format("[ERROR:{0}] {1} ({2})",
                                                       value->kind_name(), value->message(), value->location());

    std::format_to(ctx.out(), "{}", displayed_error);

    // !Verbose + Pretty & !Verbose + !Pretty cases
    if (!verbose) {
      return ctx.out();
    }

    // Get all causes
    std::vector<const re::AnyError*> causes{};
    auto cause = std::ref(value->cause());
    while (cause.get() != std::nullopt) {
      auto* current_cause = &cause.get().value();

      causes.push_back(current_cause);                 // Add current cause
      cause = std::ref(current_cause->get()->cause()); // Go to next std::optional<cause>
    }

    // Verbose + !Pretty case
    if (!pretty) {
      for (usize i = 0; i < causes.size() - 1; i++) {
        const re::AnyError* current_cause = causes[i];
        std::format_to(ctx.out(), "\n├─[CAUSE:{0}] {1} ({2})", current_cause->get()->kind_name(), current_cause->get()->message(), current_cause->get()->location());
      }
      const re::AnyError* last_cause = causes[causes.size() - 1];
      return std::format_to(ctx.out(), "\n└─[CAUSE:{0}] {1} ({2})", last_cause->get()->kind_name(), last_cause->get()->message(), last_cause->get()->location());
    }

    // Verbose + Pretty case
    if (causes.size() > 0) {
      for (usize i = 0; i < causes.size() - 1; i++) {
        const re::AnyError* current_cause = causes[i];
        std::format_to(ctx.out(), "\n├─[{3}CAUSE{5}:{4}{0}{6}] {1} ({2:#})",
                       /*0*/ current_cause->get()->kind_name(), /*1*/ current_cause->get()->message(), /*2*/ current_cause->get()->location(),
                       /*3*/ CAUSE_COLOR, /*4*/ ERROR_KIND_COLOR, /*5*/ LOW_COLOR, /*6*/ DEFAULT_COLOR);
      }
      const re::AnyError* last_cause = causes[causes.size() - 1];
      return std::format_to(ctx.out(), "\n└─[{3}CAUSE{5}:{4}{0}{6}] {1} ({2:#})",
                            /*0*/ last_cause->get()->kind_name(), /*1*/ last_cause->get()->message(), /*2*/ last_cause->get()->location(),
                            /*3*/ CAUSE_COLOR, /*4*/ ERROR_KIND_COLOR, /*5*/ LOW_COLOR, /*6*/ DEFAULT_COLOR);
    }

    return ctx.out();
  }
};

// re::AnyError std::formatter template specialization
// Formatting:
// - [default]: [ERROR:Kind] message (location)
// - [?]      : [ERROR:Kind] message (location)
//              ├╴[CAUSE:Kind] message (location)
//              ├╴...
//              └╴[CAUSE:Kind] message (location)
template <typename TKind>
  requires std::is_enum_v<TKind>
struct std::formatter<re::Error<TKind>> {
  // Pretty mode ['#'](default: false):
  // - Displays colors
  bool pretty = false;
  // Verbose mode ['?'](default: false):
  // - Displays error cause(s)
  bool verbose = false;

  // Color settings
  const char* DEFAULT_COLOR = tcolor::RESET;
  const char* LOW_COLOR = tcolor::BLACK;
  const char* ERROR_COLOR = tcolor::BOLD_RED;
  const char* ERROR_KIND_COLOR = tcolor::BLUE;
  const char* CAUSE_COLOR = tcolor::RED;

  constexpr auto parse(std::format_parse_context& ctx) {
    auto it = ctx.begin();

    for (; it != ctx.end() && *it != '}'; it++) {
      if (*it == '#')
        pretty = true;
      if (*it == '?')
        verbose = true;
    }

    return it;
  }

  auto format(const re::Error<TKind>& value, std::format_context& ctx) const {
    std::string displayed_error = pretty ? std::format("[{3}ERROR{5}:{4}{0}{6}] {1} ({2:#})",
                                                       /*0*/ value.kind_name(), /*1*/ value.message(), /*2*/ value.location(),
                                                       /*3*/ ERROR_COLOR, /*4*/ ERROR_KIND_COLOR, /*5*/ LOW_COLOR, /*6*/ DEFAULT_COLOR)
                                         : std::format("[ERROR:{0}] {1} ({2})",
                                                       value.kind_name(), value.message(), value.location());

    std::format_to(ctx.out(), "{}", displayed_error);

    // !Verbose + Pretty & !Verbose + !Pretty cases
    if (!verbose) {
      return ctx.out();
    }

    // Get all causes
    std::vector<const re::AnyError*> causes{};
    auto cause = std::ref(value.cause());
    while (cause.get() != std::nullopt) {
      auto* current_cause = &cause.get().value();

      causes.push_back(current_cause);                 // Add current cause
      cause = std::ref(current_cause->get()->cause()); // Go to next std::optional<cause>
    }

    // Verbose + !Pretty case
    if (!pretty) {
      for (usize i = 0; i < causes.size() - 1; i++) {
        const re::AnyError* current_cause = causes[i];
        std::format_to(ctx.out(), "\n├─[CAUSE:{0}] {1} ({2})", current_cause->get()->kind_name(), current_cause->get()->message(), current_cause->get()->location());
      }
      const re::AnyError* last_cause = causes[causes.size() - 1];
      return std::format_to(ctx.out(), "\n└─[CAUSE:{0}] {1} ({2})", last_cause->get()->kind_name(), last_cause->get()->message(), last_cause->get()->location());
    }

    // Verbose + Pretty case
    if (causes.size() > 0) {
      for (usize i = 0; i < causes.size() - 1; i++) {
        const re::AnyError* current_cause = causes[i];
        std::format_to(ctx.out(), "\n├─[{3}CAUSE{5}:{4}{0}{6}] {1} ({2:#})",
                       /*0*/ current_cause->get()->kind_name(), /*1*/ current_cause->get()->message(), /*2*/ current_cause->get()->location(),
                       /*3*/ CAUSE_COLOR, /*4*/ ERROR_KIND_COLOR, /*5*/ LOW_COLOR, /*6*/ DEFAULT_COLOR);
      }
      const re::AnyError* last_cause = causes[causes.size() - 1];
      return std::format_to(ctx.out(), "\n└─[{3}CAUSE{5}:{4}{0}{6}] {1} ({2:#})",
                            /*0*/ last_cause->get()->kind_name(), /*1*/ last_cause->get()->message(), /*2*/ last_cause->get()->location(),
                            /*3*/ CAUSE_COLOR, /*4*/ ERROR_KIND_COLOR, /*5*/ LOW_COLOR, /*6*/ DEFAULT_COLOR);
    }

    return ctx.out();
  }
};
