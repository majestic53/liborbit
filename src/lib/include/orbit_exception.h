/**
 * liborbit
 * Copyright (C) 2015 David Jolly
 * ----------------------
 *
 * liborbit is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * liborbit is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef ORBIT_EXCEPTION_H_
#define ORBIT_EXCEPTION_H_

#include <stdexcept>

namespace ORBIT {

	#define EXCEPTION_HEADER "(EXCEPTION)"
	#define UNKNOWN_EXCEPTION "Unknown exception"

	#define THROW_EXCEPTION(_EXCEPT_) \
		_throw_exception(_EXCEPT_, __FILE__, __LINE__, NULL);
	#define THROW_EXCEPTION_MESSAGE(_EXCEPT_, _FORMAT_, ...) \
		_throw_exception(_EXCEPT_, __FILE__, __LINE__, _FORMAT_, __VA_ARGS__)

	extern void _throw_exception(
		__in const std::string &message,
		__in const std::string &file,
		__in size_t line,
		__in_opt const char *format = NULL,
		...
		);

	typedef class _orbit_exception :
			public std::runtime_error {

		public:

			_orbit_exception(
				__in_opt const std::string &what = std::string()
				);

			_orbit_exception(
				__in const _orbit_exception &other
				);

			virtual ~_orbit_exception(void);

			_orbit_exception &operator=(
				__in const _orbit_exception &other
				);

	} orbit_exception, *orbit_exception_ptr;
}

#endif // ORBIT_EXCEPTION_H_
