// Copyright (c) 2023 Petter Nilsson. MIT License. https://github.com/pettni/ezconfig

#pragma once

#include <functional>

#define EZ_CONCATENATE_IMPL(s1, s2) s1##s2
#define EZ_CONCATENATE(s1, s2) EZ_CONCATENATE_IMPL(s1, s2)
#define EZ_ANONYMOUS_VARIABLE(str) EZ_CONCATENATE(str, __COUNTER__)
#define EZ_STATIC_INVOKE(...) \
  static bool EZ_ANONYMOUS_VARIABLE(invoked) = [] { return (std::invoke(__VA_ARGS__), true); }()
