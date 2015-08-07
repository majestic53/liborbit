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

#ifndef ORBIT_TYPE_H_
#define ORBIT_TYPE_H_

namespace ORBIT {

	#define ORBIT_HEADER "(ORBIT)"

	#ifndef NDEBUG 
	#define ORBIT_EXCEPTION_HEADER ORBIT_HEADER
	#else
	#define ORBIT_EXCEPTION_HEADER EXCEPTION_HEADER
	#endif // NDEBUG

	enum {
		ORBIT_EXCEPTION_ALLOCATION = 0,
		ORBIT_EXCEPTION_INITIALIZE,
		ORBIT_EXCEPTION_UNINITIALIZE,
	};

	#define ORBIT_EXCEPTION_MAX ORBIT_EXCEPTION_UNINITIALIZE

	static const std::string ORBIT_EXCEPTION_STR[] = {
		ORBIT_EXCEPTION_HEADER " Failed to allocate library",
		ORBIT_EXCEPTION_HEADER " Library is initialized",
		ORBIT_EXCEPTION_HEADER " Library is uninitialized",
		};

	#define ORBIT_EXCEPTION_STRING(_TYPE_) \
		((_TYPE_) > ORBIT_EXCEPTION_MAX ? UNKNOWN_EXCEPTION : \
		CHECK_STR(ORBIT_EXCEPTION_STR[_TYPE_]))

	#define THROW_ORBIT_EXCEPTION(_EXCEPT_) \
		THROW_EXCEPTION(ORBIT_EXCEPTION_STRING(_EXCEPT_))
	#define THROW_ORBIT_EXCEPTION_MESSAGE(_EXCEPT_, _FORMAT_, ...) \
		THROW_EXCEPTION_MESSAGE(ORBIT_EXCEPTION_STRING(_EXCEPT_), _FORMAT_, __VA_ARGS__)

	class _orbit;
	typedef _orbit orbit, *orbit_ptr;
}

#endif // ORBIT_TYPE_H_
