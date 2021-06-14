#include "Tool.h"

std::string HTMLSymbolToRaw(const std::string& in_string) {
  auto out_string = in_string;
  // space
  while (true) {
    auto pos = out_string.find("%20");
    if (pos == std::string::npos) break;
    out_string.erase(pos, pos + 3);
    out_string.insert(out_string.begin() + pos, ' ');
  }
  return out_string;
}

std::string RawToHTMLSymbol(const std::string& in_string) {
  auto out_string = in_string;
  // +
  while (true) {
    auto pos = out_string.find("+");
    if (pos == std::string::npos) break;
    out_string.erase(pos, 1);
    out_string.insert(pos, "%2B");
  }
  return out_string;
}