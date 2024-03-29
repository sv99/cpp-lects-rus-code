#include <iostream>
#include <tuple>
#include <utility>

using std::cout;
using std::decay_t;
using std::endl;
using std::forward;
using std::get;
using std::index_sequence;
using std::make_index_sequence;
using std::make_tuple;
using std::ostream;
using std::size_t;
using std::tuple;
using std::tuple_cat;
using std::tuple_size;
using std::void_t;

template<typename Tuple, size_t... Is>
void 
print_tuple_impl(ostream& os, Tuple &&t, index_sequence<Is...>) 
{
  ((os << (Is == 0 ? "" : ", ") << get<Is>(forward<Tuple>(t))), ...);
}

template<typename Tuple, typename = void_t<decltype(get<0>(Tuple{}))>>
decltype(auto) operator<<(ostream& os, const Tuple &t)
{
  constexpr size_t sz = tuple_size<decay_t<Tuple>>::value;
  os << "(";
  print_tuple_impl(os, t, make_index_sequence<sz>{});
  return os << ")";
}

template<size_t start, typename Tuple, size_t... Is>
constexpr auto subtuple_impl(Tuple t, index_sequence<Is...>) {
  return make_tuple(get<Is + start>(t)...);
}

template<size_t start, size_t finish, typename Tuple>
constexpr auto subtuple(Tuple t) {
  static_assert(start <= finish);
  constexpr size_t sz = finish - start;
  return subtuple_impl<start>(t, make_index_sequence<sz>{});
}

template<size_t rotval, typename Tuple>
constexpr auto rotate_r (Tuple t) {
  constexpr size_t sz = tuple_size<decay_t<Tuple>>::value;
  constexpr size_t fh_size = sz - (rotval % sz);
  auto first_half = subtuple<0, fh_size>(t);
  auto second_half = subtuple<fh_size, sz>(t);
  return tuple_cat(second_half, first_half);
}

template<size_t rotval, typename Tuple>
constexpr auto rotate_l (Tuple t) {
  constexpr size_t sz = tuple_size<decay_t<Tuple>>::value;
  return rotate_r<sz - (rotval % sz)>(t);
}

template<typename Tuple>
constexpr auto tuple_tail(Tuple t) {
  constexpr size_t sz = tuple_size<decay_t<Tuple>>::value;
  return subtuple<1, sz>(t);
}

template<size_t Nx, size_t Mx, typename Tuple>
constexpr auto swap_elts (Tuple t) {
  constexpr size_t sz = tuple_size<decay_t<Tuple>>::value;
  if constexpr (Nx == Mx) return t;
  constexpr size_t Ny = Nx % sz;
  constexpr size_t My = Mx % sz;
  constexpr size_t N = (Ny > My) ? My : Ny;
  constexpr size_t M = (Ny > My) ? Ny : My;

  // now N < M, N < sz, M < sz

  auto&& nth = make_tuple(get<N>(t));
  auto&& mth = make_tuple(get<M>(t));
  auto&& tmp1 = tuple_cat(mth, tuple_tail(rotate_l<N>(t)));
  auto&& tmp2 = tuple_cat(nth, tuple_tail(rotate_l<M-N>(tmp1)));
  return rotate_r<M>(tmp2);
}

// based on observation, that:
// (x1, x2, x3, x4, ...) = (x1, x2) * (x1, x3, x4, ...)
template<size_t I, size_t J, size_t... Is, typename Tuple>
constexpr auto permute_elts (Tuple t) {
  if constexpr(sizeof...(Is) > 0) {
    return permute_elts<I, Is...>(swap_elts<I, J>(t));
  }
  else
    return swap_elts<I, J>(t);
}

template<typename Tuple, size_t... Is>
void
print_rotates(Tuple t, index_sequence<Is...>) {  
  cout << "Left rotations: " << endl;
  ((cout << Is << ": " << rotate_l<Is>(t) << endl), ...);
}

int
main () {
  constexpr auto test = make_tuple(1, 1.5, 2, 2.5, 3, 3.5);
  cout << "Simple: " << test << endl;
  constexpr auto subtest = subtuple<1, 4>(test);
  cout << "Subtuple: " << subtest << endl;
  constexpr auto testrot3 = rotate_r<3>(test);
  cout << "Rotated right by 3: " << testrot3 << endl;
  print_rotates(test, make_index_sequence<10>{});
  constexpr auto testswap24 = swap_elts<2, 4>(test);
  cout << "Swapped #2 and #4: " << testswap24 << endl;
  constexpr auto testperm23140 = permute_elts<2, 3, 1, 4, 0>(test);
  cout << "After (2 3 1 4 0) permutation: " << testperm23140 << endl;

  // run-time lvalue and rvalue tests
  auto ltest = make_tuple(1, 1.5, 2, 2.5, 3, 3.5);
  auto ltestperm23140 = permute_elts<2, 3, 1, 4, 0>(ltest);
  auto rtestperm23140 = permute_elts<2, 3, 1, 4, 0>(make_tuple(1, 1.5, 2, 2.5, 3, 3.5));
  cout << "After (2 3 1 4 0) permutation: " << ltestperm23140 << endl;
  cout << "After (2 3 1 4 0) permutation: " << rtestperm23140 << endl;
}

