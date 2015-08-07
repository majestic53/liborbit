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

#ifndef ORBIT_DEFINES_H_
#define ORBIT_DEFINES_H_

#include <cstdint>
#include <iomanip>
#include <iostream>
#include <mutex>
#include <sstream>
#include <string>

namespace ORBIT {

	#ifndef __in
	#define __in
	#endif // __in
	#ifndef __in_opt
	#define __in_opt
	#endif // __in_opt
	#ifndef __out
	#define __out
	#endif // __out
	#ifndef __out_opt
	#define __out_opt
	#endif // __out_opt
	#ifndef __inout
	#define __inout
	#endif // __inout
	#ifndef __inout_opt
	#define __inout_opt
	#endif // __inout_opt

	#define EMPTY "EMPTY"
	#define UNKNOWN "UNKNOWN"

	#define VERSION_MAJ 0
	#define VERSION_MIN 1
	#define VERSION_REV 1
	#define VERSION_WEEK 1532

	#define CHECK_STR(_STRING_) \
		(_STRING_.empty() ? EMPTY : _STRING_.c_str())

	#define _CONCAT_STR(_STRING_) # _STRING_
	#define CONCAT_STR(_STRING_) _CONCAT_STR(_STRING_)

	#define INVALID_TYPE(_TYPE_) ((_TYPE_) -1)

	#define _SERIALIZE_CALL(_TYPE_, _MUTEX_) \
		std::lock_guard<_TYPE_> __lock ## _MUTEX_(_MUTEX_)
	#define SERIALIZE_CALL(_MUTEX_) \
		_SERIALIZE_CALL(std::mutex, _MUTEX_)
	#define SERIALIZE_CALL_RECUR(_MUTEX_) \
		_SERIALIZE_CALL(std::recursive_mutex, _MUTEX_)

	#define UNREFERENCE_PARAM(_PARAM_) (void) _PARAM_

	#define VALUE_AS_HEX(_TYPE_, _VALUE_) \
		std::setw(sizeof(_TYPE_) * 2) << std::setfill('0') << std::hex \
		<< (uintmax_t) ((_TYPE_) (_VALUE_)) << std::dec

	#define VERSION_STR CONCAT_STR(VERSION_MAJ) "." CONCAT_STR(VERSION_MIN) \
		"." CONCAT_STR(VERSION_WEEK) "." CONCAT_STR(VERSION_REV)
}

#endif // ORBIT_DEFINES_H_
