#include "../include/event.hpp"

namespace lib {

/// \private
/// This instantiation must be a public symbol
template class simple_event<timer_event_type, timer_id>;

}

