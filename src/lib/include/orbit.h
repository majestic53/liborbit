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

#ifndef ORBIT_H_
#define ORBIT_H_

//#define NDEBUG

#ifndef ORBIT
#define ORBIT orbit_ns
#endif // ORBIT

#include "orbit_defines.h"
#include "orbit_exception.h"

using namespace ORBIT;

#ifndef COMPONENT
#define COMPONENT comp_ns
#endif // COMPONENT

#include "orbit_uid.h"
#include "orbit_socket.h"

using namespace ORBIT::COMPONENT;

namespace ORBIT {

	typedef class _orbit {

		public:

			~_orbit(void);

			static _orbit *acquire(void);

			void initialize(void);

			static bool is_allocated(void);

			bool is_initialized(void);

			std::string to_string(
				__in_opt bool verbose = false
				);

			void uninitialize(void);

			static std::string version(void);

		protected:

			_orbit(void);

			_orbit(
				__in const _orbit &other
				);

			_orbit &operator=(
				__in const _orbit &other
				);

			static void _delete(void);

			bool m_initialized;

			static _orbit *m_instance;

		private:

			std::recursive_mutex m_lock;

	} orbit, *orbit_ptr;
}

#endif // ORBIT_H_
