#ifndef LIB_APPLY_HEADER
#define LIB_APPLY_HEADER

#include <utility>
#include <tuple>
#include <type_traits>

#include "index_sequence.hpp"

/** \file
 * \brief Template helper to call a function with its arguments extracted from a tuple.
 *
 * This helper is used for the event \ref lib::dispatch "dispatch"
 *
 * Similar to like std::experimental::apply
 */

namespace lib {

/** \private
 * Apply tuple to ordinary functor
 */
template<typename F, typename Tuple, size_t... I>
auto apply_(F&& f, Tuple&& t, index_sequence<I...> const&) -> decltype(std::forward<F>(f)(std::get<I>(std::forward<Tuple>(t))...))
{
	return std::forward<F>(f)(std::get<I>(std::forward<Tuple>(t))...);
}

/**
 * \brief Apply tuple to ordinary functor
 *
 * Example:
 *
 * \code
 *
 * int f(int value, std::string const& str) { return value + str.size(); }
 *
 * auto t = std::make_tuple(5, std::string("Hello!"));
 *
 * int v = lib::apply(&f, t);
 * // v is now 11
 * \endcode
 */
template<typename F, typename Tuple, typename Seq = typename make_index_sequence<std::tuple_size<typename std::remove_reference<Tuple>::type>::value>>
auto apply(F && f, Tuple&& args) -> decltype(apply_(std::forward<F>(f), std::forward<Tuple>(args), Seq()))
{
	return apply_(std::forward<F>(f), std::forward<Tuple>(args), Seq());
}

/** \private
 * Apply tuple to pointer to member
 */
template<typename Obj, typename F, typename Tuple, size_t... I>
auto apply_(Obj&& obj, F&& f, Tuple&& t, index_sequence<I...> const&) -> decltype((std::forward<Obj>(obj)->*std::forward<F>(f))(std::get<I>(std::forward<Tuple>(t))...))
{
	return (std::forward<Obj>(obj)->*std::forward<F>(f))(std::get<I>(std::forward<Tuple>(t))...);
}

/**
 * \brief Apply tuple to pointer to member
 *
 * Example:
 *
 * \code
 *
 * struct object {
 *	 int f(int value, std::string const& str) const { return value + str.size(); }
 * };
 *
 *
 * object o;
 * auto t = std::make_tuple(5, std::string("Hello!"));
 *
 * int v = lib::apply(&o, &object::f, t);
 * // v is now 11
 * \endcode
 */
template<typename Obj, typename F, typename Tuple, typename Seq = typename make_index_sequence<std::tuple_size<typename std::remove_reference<Tuple>::type>::value>>
auto apply(Obj&& obj, F && f, Tuple&& args) -> decltype(apply_(std::forward<Obj>(obj), std::forward<F>(f), std::forward<Tuple>(args), Seq()))
{
	return apply_(std::forward<Obj>(obj), std::forward<F>(f), std::forward<Tuple>(args), Seq());
}

}

#endif
