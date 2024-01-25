#include "zonemaps.h"

template <typename T>
zonemap<T>::zonemap(std::vector<T> _elements, uint _num_elements_per_zone) 
      : elements(_elements), num_elements_per_zone(_num_elements_per_zone) {
    num_zones = elements.size() / num_elements_per_zone;
    zones.resize(num_zones);
}

template <typename T> 
void build() {
  sort_elements();
  for (int i = 0; i < num_zones; i++) {
    int start = i * num_elements_per_zone;
    int end = start + num_elements_per_zone;

    zones[i].min = elements[start];
    zones[i].max = elements[end - 1];
    zones[i].size = num_elements_per_zone;

    for (int j = start; j < end; j++) {
      zones[i].elements.push_back(elements[j]);
    }
  }
}

template <typename T> 
void sort_elements() {
  std::sort(elements.begin(), elements.end());
}

template <typename T> 
bool zonemap<T>::query(T key) {
  int low = 0;
  int high = num_zones - 1;
  while (low <= high) {
    int mid = (low + high) / 2;
    zone<T> z = zones[mid];
    if (key >= z.min && key <= z.max) {
      return std::find(z.elements.begin(), z.elements.end(), key) !=
             z.elements.end();
    } else if (key < z.min) {
      high = mid - 1;
    } else {
      low = mid + 1;
    }
  }
  return false;
}

template <typename T> 
std::vector<T> zonemap<T>::query(T low, T high) {
  int low_zone_idx = 0;
  int high_zone_idx = num_zones - 1;
  while (low_zone_idx <= high_zone_idx) {
    int mid = (low_zone_idx + high_zone_idx) / 2;
    zone<T> z = zones[mid];
    if (low >= z.min && low <= z.max) {
      low_zone_idx = mid;
      break;
    } else if (low < z.min) {
      high_zone_idx = mid - 1;
    } else {
      low_zone_idx = mid + 1;
    }
  }

  high_zone_idx = low_zone_idx;
  while (high_zone_idx <= high_zone_idx) {
    int mid = (low_zone_idx + high_zone_idx) / 2;
    zone<T> z = zones[mid];
    if (high >= z.min && high <= z.max) {
      high_zone_idx = mid;
      break;
    } else if (high < z.min) {
      high_zone_idx = mid - 1;
    } else {
      low_zone_idx = mid + 1;
    }
  }

  std::vector<T> res;
  for (int i = low_zone_idx; i <= high_zone_idx; i++) {
    zone<T> z = zones[i];
    for (T element : z.elements) {
      if (element >= low && element <= high) {
        res.push_back(element);
      }
    }
  }
  return res;
}
