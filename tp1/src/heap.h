#ifndef __HEAP_H__
#define __HEAP_H__

#include <vector>

using namespace std;

template<class T>
void minHeapify(vector<T> &array, const int pos, const int n) { 
  int max = 2 * pos + 1;

  if (max < n) { 
    if ((max+1) < n && array.at(max) > array.at(max+1)) {
      ++max;
    }
    if (array.at(max) < array.at(pos)) { 
      swap(array[max], array[pos]);
      minHeapify(array, max, n);
    }
  }
}

template<class T>
void buildMinHeap(vector<T> &array) {
  int size = array.size();
  int i;

  for (i = size/2 - 1; i >= 0; --i) {
    minHeapify(array, i , size);
  }
}

template<class T>
T popMin(vector<T> &array) {
  T min = array[0];
  array[0] = array.back();
  array.pop_back();
  minHeapify(array, 0, array.size());
  return min;
}

template<class T>
T popMinAndPush(vector<T> &array, T newElem) {
  T min = array[0];
  array[0] = newElem;
  minHeapify(array, 0, array.size());
  return min;
}

#endif