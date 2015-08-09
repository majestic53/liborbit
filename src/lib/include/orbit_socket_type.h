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

#ifndef ORBIT_SOCKET_TYPE_H_
#define ORBIT_SOCKET_TYPE_H_

namespace ORBIT {

	namespace COMPONENT {

		#define ORBIT_SOCKET_HEADER "(SOCKET)"

		#ifndef NDEBUG
		#define ORBIT_SOCKET_EXCEPTION_HEADER ORBIT_SOCKET_HEADER
		#else
		#define ORBIT_SOCKET_EXCEPTION_HEADER EXCEPTION_HEADER
		#endif // NDEBUG

		enum {
			ORBIT_SOCKET_EXCEPTION_ALLOCATION = 0,
			ORBIT_SOCKET_EXCEPTION_CLOSE,
			ORBIT_SOCKET_EXCEPTION_INITIALIZE,
			ORBIT_SOCKET_EXCEPTION_OPEN,
			ORBIT_SOCKET_EXCEPTION_UNINITIALIZE,
		};

		#define ORBIT_SOCKET_EXCEPTION_MAX ORBIT_SOCKET_EXCEPTION_UNINITIALIZE

		static const std::string ORBIT_SOCKET_EXCEPTION_STR[] = {
			ORBIT_SOCKET_EXCEPTION_HEADER " Failed to allocate socket component",
			ORBIT_SOCKET_EXCEPTION_HEADER " Socket component is closed",
			ORBIT_SOCKET_EXCEPTION_HEADER " Socket component is initialized",
			ORBIT_SOCKET_EXCEPTION_HEADER " Socket component is open",
			ORBIT_SOCKET_EXCEPTION_HEADER " Socket component is uninitialized",
			};

		#define ORBIT_SOCKET_EXCEPTION_STRING(_TYPE_) \
			((_TYPE_) > ORBIT_SOCKET_EXCEPTION_MAX ? UNKNOWN_EXCEPTION : \
			CHECK_STR(ORBIT_SOCKET_EXCEPTION_STR[_TYPE_]))

		#define THROW_ORBIT_SOCKET_EXCEPTION(_EXCEPT_) \
			THROW_EXCEPTION(ORBIT_SOCKET_EXCEPTION_STRING(_EXCEPT_))
		#define THROW_ORBIT_SOCKET_EXCEPTION_MESSAGE(_EXCEPT_, _FORMAT_, ...) \
			THROW_EXCEPTION_MESSAGE(ORBIT_SOCKET_EXCEPTION_STRING(_EXCEPT_), _FORMAT_, __VA_ARGS__)

		class _orbit_socket;
		typedef _orbit_socket orbit_socket, *orbit_socket_ptr;

		class _orbit_socket_factory;
		typedef _orbit_socket_factory orbit_socket_factory, *orbit_socket_factory_ptr;
	}
}

#endif // ORBIT_SOCKET_TYPE_H_
