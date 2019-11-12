/**
 * @copyright 2019
 * @author Brian Cairl
 *
 * @brief Implements the C++17 utility <code>std::integer_sequence</code> and related
 *        compile-time utility objects
 *
 *        For reference on how this works, see
 *        <a href="https://blog.galowicz.de/2016/06/24/integer_sequences_at_compile_time/</a>.
 */
#ifndef NARG_IMPL_INTEGER_SEQUENCE_H
#define NARG_IMPL_INTEGER_SEQUENCE_H
#ifndef DOXYGEN_SKIP

// C++ Standard Library
#include <utility>
#include <type_traits>

namespace narg
{

/**
 * @brief Represents a compile-time sequence of integers
 * @tparam IntT  integer element type
 * @tparam Ns...  pack of integer values of type \p IntT
 */
template<typename IntT, IntT... Ns> struct integer_sequence
{
  static_assert(std::is_integral<IntT>(), "'IntT' should be an integral type");

  /**
   * @brief Returns the number of elements in <code>IntT... Ns</code>
   */
  static constexpr std::size_t size() noexcept
  {
    return sizeof...(Ns);
  }
};

namespace details
{
/**
 * @brief Helper which simplifies creation of <code>sequence</code>
 */
template<typename IntT, std::size_t... Ns> struct make_sequence;

/**
 * @copydoc make_sequence
 */
template<typename IntT, std::size_t Index, std::size_t... Ns>
struct make_sequence<IntT, Index, Ns...>
{
  using type = typename make_sequence<IntT, Index-1, Index-1, Ns...>::type;
};

/**
 * @copydoc make_sequence
 */
template<typename IntT, std::size_t... Ns>
struct make_sequence<IntT, 0, Ns...>
{
  using type = integer_sequence<IntT, Ns...>;
};

}  // namespace details

/**
 * @brief Integer sequence where the integer type is <code>std::std::size_t</code>
 * @tparam IntT  integer element type
 * @tparam N  number of indices in sequence, i.e. <code>[0, 1, ..., N-2, N-1]</code>
 */
template<typename IntT, std::size_t N>
using make_integer_sequence = typename details::make_sequence<IntT, N>::type;

/**
 * @brief Integer sequence where the integer type is <code>std::size_t</code>
 * @tparam Ns...  pack of integer values of type <code>std::std::size_t</code>
 */
template<std::size_t... Ns>
using index_sequence = integer_sequence<std::size_t, Ns...>;

/**
 * @brief Makes an integer sequence where the integer type is <code>std::size_t</code>
 * @tparam N  number of indices in sequence, i.e. <code>[0, 1, ..., N-2, N-1]</code>
 */
template<std::size_t N>
using make_index_sequence = make_integer_sequence<std::size_t, N>;

/**
 * @brief Makes an index sequence type with elements for each type in <code>T...</code>
 * @param TPack...  pack of arbitrary types
 */
template<class... TPack>
using index_sequence_for = make_integer_sequence<std::size_t, sizeof...(TPack)>;

}  // namespace narg

#endif  // DOXYGEN_SKIP
#endif  // NARG_IMPL_INTEGER_SEQUENCE_H
