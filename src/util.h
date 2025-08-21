#ifndef UTIL_H
#define UTIL_H

#include <algorithm>
#include <iostream>


inline float clip(float n, float lower, float upper) {
  return std::max(lower, std::min(n, upper));
};

#endif