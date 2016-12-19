#ifndef TEST_UTILS_H
#define TEST_UTILS_H
#include <cstdlib>

template<typename T>
struct remove_const { typedef T type; };

// value_type of a std::map is std::pair<const KeyType, ValueType>
template<typename T>
struct remove_const<std::pair<const T, T>> { typedef std::pair<T, T> type; };

template<typename T>
T get_rand(int max) {
  return T(0) % max;
}

template<>
int get_rand<int>(int max) {
  return rand() % max;
}

template<>
float get_rand<float>(int max) {
  return (float)(rand() % max);
}

template<>
double get_rand<double>(int max) {
  return (double)(rand() % max);
}

template<>
std::pair<int, int> get_rand<std::pair<int, int>>(int max) {
  return std::make_pair(rand() % max, rand() % max);
}

template<typename T>
T increment(T &i) { // do-nothing
  return i;
}

// Generalize this for is_integral
template<>
int increment<int>(int &i) {
  return i++;
}

// value_type of a std::map is std::pair<const KeyType, ValueType>
template<>
std::pair<int, int> increment<std::pair<int, int>>(std::pair<int, int> &i) {
  return std::make_pair(i.first++, i.second);
}

template <template <class, class> class Container, class value_type>
void fill_random(Container<value_type, std::allocator<value_type>> &v,
                unsigned max = RAND_MAX) {
  for (auto &e : v)
    e = get_rand<value_type>(max);
}

template <typename T>
void fill_random(T begin, T end, unsigned max = RAND_MAX) {
  typedef typename std::iterator_traits<T>::value_type value_type;
  for (auto it = begin; it != end; ++it)
    *it = get_rand<value_type>(max);
}

// It can work with char* or std::string.
template<typename T>
void fill_random_chars(T begin, T end, bool upper) {
  char max = upper ? 'Z' : 'z';
  char min = upper ? 'A' : 'a';
  auto it = begin;
  typedef typename std::iterator_traits<T>::value_type value_type;
  for (; it != end -1; ++it) {
    *it = get_rand<value_type>(max) * (max - min) + min;
    assert(*it >= min);
    assert(*it <= max);
  }
  *it = '\0';
}

// TODO: Create a template class such that all the
// APIs of STL containers can be exercised in a concise way.
// for example insert, push_back, pop_back, push_front, pop_front, advance

// TODO: Benchmark memory allocated on heap vs. stack.

template <template <class, class> class Container, class value_type>
void fill_seq(Container<value_type, std::allocator<value_type>> &v) {
  value_type j = get_rand<value_type>(RAND_MAX);
  for (auto &e : v)
    e = increment(j);
}

template <typename T>
void fill_seq(T begin, T end) {
  typedef typename std::iterator_traits<T>::value_type value_type;
  value_type j = get_rand<value_type>(RAND_MAX);
  for (auto it = begin; it != end; ++it)
    *it = increment(j);
}

// c-style comparator for integral types.
template<typename T>
static int compare(const void * a, const void * b)
{
  static_assert(std::is_integral<T>::value, "Not an integral type.");
  return (*(T*)a - *(T*)b);
}

#endif // TEST_UTILS_H

