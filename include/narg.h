
/**
 * @copyright 2019
 * @author Brian Cairl
 *
 * @file narg.h
 * @brief Named argument utility
 */
#ifndef NARG_H
#define NARG_H

// C++ Standard Library
#include <tuple>
#include <type_traits>
#include <utility>

// Narg
#include <narg/hash.h>

#define STATIC_ASSERT_MSG(str) "\n--------NARG--------\n\n\n" str "\n\n\n--------NARG--------"

namespace narg
{
namespace detail
{


template<std::size_t HASH, typename T>
struct ArgSpec
{
  using Type = T;
  constexpr static std::size_t Hash = HASH;

  constexpr ArgSpec() :
    value{std::forward<T>(const_cast<std::remove_const_t<T>&>(ArgSpec::dummy))}
  {}

  constexpr ArgSpec(const ArgSpec& other) :
    value{std::forward<T>(other.value)}
  {}

  T&& value;
  static T dummy;
};


template<std::size_t HASH, typename T>
T ArgSpec<HASH, T>::dummy{};


template<std::size_t HASH, typename T>
struct ArgFiller
{
  using Type = T;
  constexpr static std::size_t Hash = HASH;

  constexpr ArgFiller(const T& _value) :
    value{_value}
  {}

  const T& value;
};


template<std::size_t HASH, typename T>
ArgFiller<HASH, T> make_arg_filler(T&& value)
{
  return ArgFiller<HASH, T>{std::forward<T>(value)};
}


template<typename T>
struct IsArgFiller :  std::integral_constant<bool, false> {};


template<std::size_t HASH, typename T>
struct IsArgFiller<ArgFiller<HASH, T>> : std::integral_constant<bool, true> {};


template<std::size_t POS, std::size_t HASH, typename T>
struct PositionalArgSpec : ArgSpec<HASH, T>
{
  constexpr static std::size_t Position = POS;

  constexpr PositionalArgSpec() = default;
  constexpr PositionalArgSpec(const ArgSpec<HASH, T>& spec) :
    ArgSpec<HASH, T>{spec}
  {}
};


template<std::size_t I, typename TupleT>
using tuple_element_no_ref_t = std::remove_reference_t<std::tuple_element_t<I, TupleT>>;


template<typename NArgSpecTupleT, std::size_t... Ints>
constexpr decltype(auto) make_narg_specs_impl(NArgSpecTupleT&& narg_specs, std::integer_sequence<std::size_t, Ints...>)

{
  using NArgsType = 
    std::tuple<detail::PositionalArgSpec<Ints,
                                         tuple_element_no_ref_t<Ints, NArgSpecTupleT>::Hash,
                                         typename tuple_element_no_ref_t<Ints, NArgSpecTupleT>::Type>
                ...>;

  return NArgsType{std::get<Ints>(narg_specs)...};
}


template<typename... ArgSpecTs>
constexpr decltype(auto) make_narg_specs(ArgSpecTs&&... narg_specs)
{
  return make_narg_specs_impl(std::forward_as_tuple(narg_specs...),
                              std::make_integer_sequence<std::size_t, sizeof...(ArgSpecTs)>{});
}


template<typename... ArgSpecTs>
using nargs_base_t = decltype(make_narg_specs<ArgSpecTs...>(std::declval<ArgSpecTs>()...));


template<std::size_t TARGET_HASH,
         typename NArgsBaseT,
         std::size_t INDEX = 0,
         bool MATCHED = (std::tuple_element_t<INDEX, NArgsBaseT>::Hash == TARGET_HASH),
         bool IS_END = (INDEX+1 == std::tuple_size<NArgsBaseT>::value)>
struct FindHashMatch;


template<std::size_t TARGET_HASH, typename NArgsBaseT, std::size_t Index>
struct FindHashMatch<TARGET_HASH, NArgsBaseT, Index, false, true> :
  std::integral_constant<std::size_t, static_cast<std::size_t>(-1)>{};


template<std::size_t TARGET_HASH, typename NArgsBaseT, std::size_t Index>
struct FindHashMatch<TARGET_HASH, NArgsBaseT, Index, false, false> :
  FindHashMatch<TARGET_HASH, NArgsBaseT, Index+1>
{
};


template<std::size_t TARGET_HASH, typename NArgsBaseT, std::size_t Index, bool IS_END>
struct FindHashMatch<TARGET_HASH, NArgsBaseT, Index, true, IS_END> :
  std::integral_constant<std::size_t, tuple_element_no_ref_t<Index, NArgsBaseT>::Position>
{
};


template<std::size_t INDEX, typename NArgsSpecTupleT, typename NArgFillerTupleT>
std::enable_if_t<IsArgFiller<tuple_element_no_ref_t<INDEX, NArgFillerTupleT>>::value, int>
  fill_nargs_assignment_impl(NArgsSpecTupleT& narg_base, NArgFillerTupleT&& narg_fills)
{
  constexpr std::size_t TARGET_HASH = tuple_element_no_ref_t<INDEX, NArgFillerTupleT>::Hash;
  constexpr std::size_t MATCH_INDEX = FindHashMatch<TARGET_HASH, NArgsSpecTupleT>::value;

  static_assert(MATCH_INDEX != static_cast<std::size_t>(-1),
                STATIC_ASSERT_MSG("PLACEMENT FAILURE: Could not find known match for passed argument"));

  std::get<MATCH_INDEX>(narg_base).value = std::get<INDEX>(narg_fills).value;
  return 0;
}


template<std::size_t INDEX, typename NArgsSpecTupleT, typename NArgFillerTupleT>
std::enable_if_t<!IsArgFiller<tuple_element_no_ref_t<INDEX, NArgFillerTupleT>>::value, int>
  fill_nargs_assignment_impl(NArgsSpecTupleT& narg_base, NArgFillerTupleT&& narg_fills)
{
  std::get<INDEX>(narg_base).value = std::get<INDEX>(narg_fills);
  return 0;
}


template<typename NArgsSpecTupleT, typename NArgFillerTupleT, std::size_t... Ints>
constexpr void fill_nargs_impl(NArgsSpecTupleT&& narg_base,
                               NArgFillerTupleT&& narg_fills,
                               std::integer_sequence<std::size_t, Ints...>)
{
  std::make_tuple(fill_nargs_assignment_impl<Ints>(std::forward<NArgsSpecTupleT>(narg_base),
                                                   std::forward<NArgFillerTupleT>(narg_fills))...);
}



template<typename NArgsSpecTupleT, typename... ArgFillerTs>
constexpr NArgsSpecTupleT fill_nargs(ArgFillerTs&&... narg_fills)
{
  NArgsSpecTupleT narg_base;
  fill_nargs_impl(narg_base,
                  std::forward_as_tuple(narg_fills...),
                  std::make_integer_sequence<std::size_t, sizeof...(ArgFillerTs)>{});
  return narg_base;
}


}  // namespace detail


struct use_nargs_t {};

static constexpr use_nargs_t use_nargs;


template<typename... ArgSpecTs>
struct NArgs : detail::nargs_base_t<ArgSpecTs...>
{
  using Base = detail::nargs_base_t<ArgSpecTs...>;
  template<typename... ArgFillerTs>
  NArgs(ArgFillerTs&&... arg_fills) :
    Base{detail::fill_nargs<Base>(arg_fills...)}
  {}

  template<std::size_t HASH>
  const decltype(auto) get() const
  {
    return std::get<detail::FindHashMatch<HASH, Base>::value>(*this).value;
  }
};


#define NARG_SPEC(name, type) ::narg::detail::ArgSpec<NARG_HASH(name), type>

#define NARG_FILL(name, value) ::narg::detail::make_arg_filler<NARG_HASH(name)>(value)

#define NARG_GET(pack, name) pack.template get<NARG_HASH(name)>()


#define NARG_IMPL_CONSTRUCTOR(FnName, pack_name) \
  FnName(const FnName##NamedArgs& pack_name)


#define NARG_DECL_CONSTRUCTOR(FnName, pack_name, specs... /*NARG_SPEC ... */) \
  using FnName##NamedArgs = ::narg::NArgs<specs>;\
  template<typename... NArgTs>\
  explicit FnName(::narg::use_nargs_t, NArgTs&&... args) :\
    FnName{FnName##NamedArgs{std::forward<NArgTs>(args)...}}\
  {}\
  explicit NARG_IMPL_CONSTRUCTOR(FnName, pack_name)


} // namespace narg

#endif  // NARG_H