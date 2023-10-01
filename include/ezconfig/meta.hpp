#pragma once

#include <cstddef>
#include <type_traits>

namespace ezconfig {

template<typename _Tp, typename... _Types>
constexpr std::size_t index_in_typepack()
{
  constexpr std::size_t tpsize = sizeof...(_Types);
  constexpr bool found[tpsize] = {std::is_same_v<_Tp, _Types>...};
  std::size_t n                = tpsize;
  for (std::size_t i = 0; i < tpsize; ++i) {
    if (found[i]) {
      if (n < tpsize) { return tpsize; }  // more than one _Tp found
      n = i;
    }
  }
  return n;
}

}  // namespace ezconfig
