#ifndef UTIL_H
#define UTIL_H

#include <algorithm>
#include <iostream>

inline float clip(float n, float lower, float upper) {
  if (n < lower)
    return lower;
  if (n > upper)
    return upper;
  return n;
};

#endif