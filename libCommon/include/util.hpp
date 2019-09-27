#ifndef LIB_UTIL_HEADER
#define LIB_UTIL_HEADER

#include "time.hpp"

#include <cstdint>

/** \file
 * \brief Various utility functions
 */

namespace lib {

/** \brief Sleep current thread for the specified \ref duration.
 *
 * Alternative to \c std::this_thread::sleep_for which unfortunately isn't implemented on
 * MinGW.
 *
 * \note May wake up early, e.g. due to a signal. You can use \ref monotonic_clock
 * to check elapsed time and sleep again if needed.
 */
void LIB_PUBLIC_SYMBOL sleep(duration const& d);

/** \brief Get a secure random integer uniformly distributed in the closed interval [min, max]
 */
int64_t LIB_PUBLIC_SYMBOL random_number(int64_t min, int64_t max);

}

#endif
