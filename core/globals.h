#pragma once

#include "utils/logger.h"
#include <stdint.h>
extern Logger Log;
const unsigned int WIDTH = 800;
const unsigned int HEIGHT = 600;
constexpr unsigned long long VARIANT_ENUM_SIZE = sizeof(short);

// Helper for enum conversions and casting

template <typename E>
constexpr int enum_get_underlying(E e) noexcept {
    return static_cast<std::underlying_type_t<E>>(e);
}

template <typename E>
constexpr E enum_from_underlying(int v) noexcept {
    return static_cast<E>(v);
}
