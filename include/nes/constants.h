#pragma once

#include <cstddef>

namespace nes {

constexpr size_t kChrBankSize = 0x2000;
constexpr size_t kTileMapSize = kChrBankSize / 2;
constexpr size_t kPrgBankSize = 0x4000;

} // namespace nes
