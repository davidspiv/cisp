#ifndef INTERPOLATION_H
#define INTERPOLATION_H

#include <iostream>
#include <vector>


std::vector<std::vector<int>>
bilinearResize(std::vector<std::vector<int>> inputArr, size_t outHeight,
               size_t outWidth);

#endif
