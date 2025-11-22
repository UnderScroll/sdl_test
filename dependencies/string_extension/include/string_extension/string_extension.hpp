#include <string>
#include <string_view>
#include <vector>

namespace se {

// Splits a given string into parts given a delimitor (delimitor excluded)
// Can be replaced by std::ranges::view::split or std::ranges::split_view
// But I had issue with it, it was simpler for me to implement one that's less generic
auto split(std::string_view value, std::string delimitor) -> std::vector<std::string_view>;

} // namespace se
