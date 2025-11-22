#pragma once

#include <cstring>
#include <format>
#include <ranges>
#include <source_location>
#include <string>
#include <string_extension/string_extension.hpp>
#include <string_view>
#include <unders_helpers/term_colors.hpp>
#include <unders_helpers/types.hpp>
#include <vector>

// std::source_location std::formatter template specialization
// Formatting:
// - [default]: file_path/file_name: function_full_name() - line:column
// - [?]      : file_path/file_name: function_return_type function_full_name(function_parameter_types) - line:column
template <>
struct std::formatter<std::source_location> {
  // Pretty mode ['#'](default: false):
  // - Displays colors
  bool pretty = false;
  // Verbose mode ['?'](default: false):
  // - Displays function return type
  // - Displays function parameters type
  bool verbose = false;

  // Color settings
  const char* DEFAULT_COLOR = tcolor::RESET;
  const char* LOW_COLOR = tcolor::BLACK;
  const char* NAMESPACE_COLOR = tcolor::YELLOW;
  const char* TYPE_COLOR = tcolor::CYAN;
  const char* FUNCTION_NAME_COLOR = tcolor::BLUE;

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

  auto format(const std::source_location& value, std::format_context& ctx) const {
    // Basic values
    std::string_view file_path = value.file_name();
    std::string_view function_signature = value.function_name();
    const uint line = value.line();
    const uint column = value.column();

    // !Pretty + Verbose case
    if (!pretty && verbose) {
      return std::format_to(ctx.out(), "{0}: {1} - {2}:{3}", file_path, function_signature, line, column);
    }

    // Function signature parsing
    usize parameter_begin_index = function_signature.find_first_of('(');
    usize function_name_begin_index = function_signature.substr(0, parameter_begin_index).find_last_of(' ') + 1;
    std::string_view function_full_name = function_signature.substr(function_name_begin_index, parameter_begin_index - function_name_begin_index);

    // !Pretty + !Verbose case
    if (!pretty && !verbose) {
      return std::format_to(ctx.out(), "{0}: {1} - {2}:{3}", file_path, function_full_name, line, column);
    }

    // Parse file path
    usize file_name_index = file_path.find_last_of('/') + 1;
    std::string displayed_file_path;
    if (file_name_index != std::string::npos) { // if directory path found
      std::string_view directory_path = file_path.substr(0, file_name_index);
      std::string_view file_name = file_path.substr(file_name_index);
      displayed_file_path = std::format("{2}{0}{3}{1}", directory_path, file_name, LOW_COLOR, DEFAULT_COLOR);
    } else {
      displayed_file_path = file_path;
    }

    // TODO: Proper type(as std::string) parsing but not trivial because of weird spaces, chevrons and distinguishing types from types' namespaces

    // Parse function signature
    // Parse function name
    std::vector<std::string_view> function_full_name_parts = se::split(function_full_name, "::");
    // Manual join as function_name (i.e. last of function_full_name_parts) is different
    std::string displayed_function_name;
    // Reserve for fullname + 1st namespace color + all but one delimitor styling + function actual name + default color end
    displayed_function_name.reserve(
        function_full_name.size() +
        strlen(NAMESPACE_COLOR) +
        (function_full_name_parts.size() - 2) * (strlen(LOW_COLOR) + strlen(NAMESPACE_COLOR)) +
        strlen(FUNCTION_NAME_COLOR) +
        strlen(DEFAULT_COLOR));
    displayed_function_name.append(NAMESPACE_COLOR);
    for (usize i = 0; i < function_full_name_parts.size() - 1; i++) {
      displayed_function_name.append(function_full_name_parts[i]);
      displayed_function_name.append(LOW_COLOR);
      displayed_function_name.append("::");
      displayed_function_name.append(NAMESPACE_COLOR);
    }
    displayed_function_name.append(FUNCTION_NAME_COLOR);
    displayed_function_name.append(function_full_name_parts[function_full_name_parts.size() - 1]);
    displayed_function_name.append(DEFAULT_COLOR);

    // Pretty + !Verbose case
    if (!verbose) {
      return std::format_to(ctx.out(), "{0}{5}: {1}() {5}- {4}{2}{5}:{4}{3}", displayed_file_path, displayed_function_name, line, column, DEFAULT_COLOR, LOW_COLOR);
    }

    std::string_view function_return_type = function_signature.substr(0, function_name_begin_index - 1);

    std::string_view function_parameters = function_signature.substr(parameter_begin_index + 1, function_signature.size() - parameter_begin_index - 2);
    std::vector<std::string_view> split_function_parameters = se::split(function_parameters, ", ");
    std::string displayed_function_parameters = split_function_parameters |
                                                std::views::join_with(std::format("{0}, {1}", LOW_COLOR, TYPE_COLOR)) |
                                                std::ranges::to<std::string>();
    displayed_function_parameters.insert(0, TYPE_COLOR);
    displayed_function_parameters.append(DEFAULT_COLOR);

    return std::format_to(ctx.out(), "{0}{6}: {8}{1} {2}({3}) {7}- {6}{4}{7}:{6}{5}",
                          /*0*/ displayed_file_path, /*1*/ function_return_type, /*2*/ displayed_function_name, /*3*/ displayed_function_parameters, /*4*/ line, /*5*/ column,
                          /*6*/ DEFAULT_COLOR, /*7*/ LOW_COLOR, /*8 */ TYPE_COLOR);
  }
};
