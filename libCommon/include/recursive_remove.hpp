#ifndef LIB_RECURSIVE_REMOVE_HEADER
#define LIB_RECURSIVE_REMOVE_HEADER

#include "libcommon.hpp"

#ifdef LIB_WINDOWS
#include "private/windows.hpp"
#endif

#include <list>

/// \file
/// \brief Class to recursively delete directories

namespace lib {

/** \brief Recursively deletes directories.
 *
 * Behavior varies by platform. On Windows, SHFileOperation is used,
 * whereas we manually traverse the directory tree on other platforms.
 */
class LIB_PUBLIC_SYMBOL recursive_remove
{
public:
	recursive_remove() = default;
	virtual ~recursive_remove() = default;

	recursive_remove(recursive_remove const&) = delete;
	recursive_remove& operator=(recursive_remove const&) = delete;

	/// \brief Removes given directory
	bool remove(native_string const& path);

	/// \brief Removes given directories
	bool remove(std::list<native_string> dirsToVisit);

protected:
	/// \brief Can be overriden to ask the user for a confirmation.
	///
	/// On Windows this isn't called, there SHFileOperation itself can ask for confirmation.
	/// See \ref adjust_shfileop
	virtual bool confirm() const { return true; }

#ifdef LIB_WINDOWS
	/// \brief Windows only: Allows customization of the SHFILEOPSTRUCT passed to SHFileOperation.
	///
	/// The default implementation allows undo and supresses any GUI output.
	virtual void adjust_shfileop(SHFILEOPSTRUCT & op);
#endif
};

}

#endif
