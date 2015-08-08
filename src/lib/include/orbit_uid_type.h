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

#ifndef ORBIT_UID_TYPE_H_
#define ORBIT_UID_TYPE_H_

namespace ORBIT {

	namespace COMPONENT {

		#define ORBIT_UID_HEADER "(UID)"

		#ifndef NDEBUG
		#define ORBIT_UID_EXCEPTION_HEADER ORBIT_UID_HEADER
		#else
		#define ORBIT_UID_EXCEPTION_HEADER EXCEPTION_HEADER
		#endif // NDEBUG

		enum {
			ORBIT_UID_EXCEPTION_ALLOCATION = 0,
			ORBIT_UID_EXCEPTION_INITIALIZE,
			ORBIT_UID_EXCEPTION_INSUFFICENT,
			ORBIT_UID_EXCEPTION_NOT_FOUND,
			ORBIT_UID_EXCEPTION_UNINITIALIZE,
		};

		#define ORBIT_UID_EXCEPTION_MAX ORBIT_UID_EXCEPTION_UNINITIALIZE

		static const std::string ORBIT_UID_EXCEPTION_STR[] = {
			ORBIT_UID_EXCEPTION_HEADER " Failed to allocate Uid component",
			ORBIT_UID_EXCEPTION_HEADER " Uid component is initialized",
			ORBIT_UID_EXCEPTION_HEADER " Uid component entry space saturated",
			ORBIT_UID_EXCEPTION_HEADER " Uid component entry does not exist",
			ORBIT_UID_EXCEPTION_HEADER " Uid component is uninitialized",
			};

		#define ORBIT_UID_EXCEPTION_STRING(_TYPE_) \
			((_TYPE_) > ORBIT_UID_EXCEPTION_MAX ? UNKNOWN_EXCEPTION : \
			CHECK_STR(ORBIT_UID_EXCEPTION_STR[_TYPE_]))

		#define THROW_ORBIT_UID_EXCEPTION(_EXCEPT_) \
			THROW_EXCEPTION(ORBIT_UID_EXCEPTION_STRING(_EXCEPT_))
		#define THROW_ORBIT_UID_EXCEPTION_MESSAGE(_EXCEPT_, _FORMAT_, ...) \
			THROW_EXCEPTION_MESSAGE(ORBIT_UID_EXCEPTION_STRING(_EXCEPT_), _FORMAT_, __VA_ARGS__)

		class _orbit_uid;
		typedef _orbit_uid orbit_uid, *orbit_uid_ptr;

		class _orbit_uid_class;
		typedef _orbit_uid_class orbit_uid_class, *orbit_uid_class_ptr;

		class _orbit_uid_factory;
		typedef _orbit_uid_factory orbit_uid_factory, *orbit_uid_factory_ptr;
	}
}

#endif // ORBIT_UID_TYPE_H_
