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

#ifndef ORBIT_SOCKET_H_
#define ORBIT_SOCKET_H_

#include <netdb.h>
#include <vector>

namespace ORBIT {

	namespace COMPONENT {

		typedef enum {
			ORBIT_SOCKET_TYPE_NONE = 0,
			ORBIT_SOCKET_TYPE_TCP,
			ORBIT_SOCKET_TYPE_UDP,
		} orbit_socket_t;

		#define ORBIT_SOCKET_TYPE_MAX ORBIT_SOCKET_TYPE_UDP

		typedef enum {
			ORBIT_SOCKET_FAMILY_TYPE_4 = 0,
			ORBIT_SOCKET_FAMILY_TYPE_6,
		} orbit_socket_family_t;

		#define ORBIT_SOCKET_FAMILY_TYPE_MAX ORBIT_SOCKET_FAMILY_TYPE_6

		typedef std::vector<uint8_t> orbit_socket_buf_t;

		typedef class _orbit_socket :
				public orbit_uid_class {

			public:

				_orbit_socket(
					__in_opt const std::string &host = std::string(),
					__in_opt uint16_t port = 0
					);

				_orbit_socket(
					__in const _orbit_socket &other
					);

				virtual ~_orbit_socket(void);

				_orbit_socket &operator=(
					__in const _orbit_socket &other
					);

				void close(void);

				orbit_socket_family_t family(void);

				bool is_open(void);

				void open_tcp(void);

				void open_tcp(
					__in const std::string &host,
					__in uint16_t port
					);

				int read(
					__in orbit_socket_buf_t &buffer,
					__in size_t size
					);

				virtual std::string to_string(
					__in_opt bool verbose = false
					);

				orbit_socket_t type(void);

				int write(
					__in const orbit_socket_buf_t &buffer
					);

			protected:

				sockaddr_in m_address_4;

				sockaddr_in6 m_address_6;

				std::string m_host;

				addrinfo *m_information;

				uint16_t m_port;

				int m_socket;

				orbit_socket_t m_type;

			private:

				std::recursive_mutex m_lock;

		} orbit_socket, *orbit_socket_ptr;
	}
}

#endif // ORBIT_SOCKET_H_
