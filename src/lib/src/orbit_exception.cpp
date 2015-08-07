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

#include <cstdarg>
#include "../include/orbit.h"

namespace ORBIT {

	#define EXCEPTION_INVALID "Invalid exception"
	#define EXCEPTION_MALFORMED "Malformed format"

	void 
	_throw_exception(
		__in const std::string &message,
		__in const std::string &file,
		__in size_t line,
		__in_opt const char *format,
		...
		)
	{
		va_list list;
		size_t length;
		std::string buffer;
		std::stringstream stream;

		if(!message.empty()) {
			stream << message;
		}

		if(format) {
			va_start(list, format);

			length = std::vsnprintf(NULL, 0, format, list);
			if(length) {				
				va_end(list);
				va_start(list, format);
				buffer.resize(++length);

				if(std::vsnprintf((char *) &buffer[0], buffer.size(), 
						format, list) < 0) {
					buffer = EXCEPTION_MALFORMED;
				}			
			} else {
				buffer = EXCEPTION_INVALID;
			}

			va_end(list);
		}

		if(!buffer.empty()) {
			stream << ": "<< buffer;
		}
#ifndef NDEBUG
		stream << " (";

		if(!file.empty()) {
			stream << file << ":";
		}

		stream << line << ")";
#endif // NDEBUG

		throw orbit_exception(CHECK_STR(stream.str()));
	}

	_orbit_exception::_orbit_exception(
		__in_opt const std::string &what
		) :
			std::runtime_error(what)
	{
		return;
	}

	_orbit_exception::_orbit_exception(
		__in const _orbit_exception &other
		) :
			std::runtime_error(other)
	{
		return;
	}

	_orbit_exception::~_orbit_exception(void)
	{
		return;
	}

	_orbit_exception &
	_orbit_exception::operator=(
		__in const _orbit_exception &other
		)
	{

		if(this != &other) {
			std::runtime_error::operator=(other);
		}

		return *this;
	}
}
