#include "string_extension/string_extension.hpp"

#include <unders_helpers/types.hpp>

auto se::split(std::string_view value, std::string delimitor) -> std::vector<std::string_view> {
  const usize delimitor_length = delimitor.length();

  std::vector<std::string_view> parts;
  usize current_index = 0, next_index;

  // Finds all delimitor until end of value, taking each substring in between
  while ((next_index = value.find_first_of(delimitor, current_index)) != std::string::npos) {
    // Add substring
    const usize substring_length = next_index - current_index;
    parts.push_back(value.substr(current_index, substring_length));

    // Update current index with delimitor offset
    current_index = next_index + delimitor_length;
  }
  // Add last substring
  parts.push_back(value.substr(current_index));

  return parts;
}
