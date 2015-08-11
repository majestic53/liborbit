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

#include <map>
#include <netdb.h>

namespace ORBIT {

	namespace COMPONENT {

		typedef enum {
			ORBIT_SOCKET_TYPE_NONE = 0,
			ORBIT_SOCKET_TYPE_TCP,
			ORBIT_SOCKET_TYPE_UDP,
		} orbit_socket_t;

		#define ORBIT_SOCKET_TYPE_MAX ORBIT_SOCKET_TYPE_UDP

		typedef enum {
			ORBIT_SOCKET_FAMILY_TYPE_NONE = 0,
			ORBIT_SOCKET_FAMILY_TYPE_IPV4,
			ORBIT_SOCKET_FAMILY_TYPE_IPV6,
		} orbit_socket_family_t;

		#define ORBIT_SOCKET_FAMILY_TYPE_MAX ORBIT_SOCKET_FAMILY_TYPE_IPV6

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

				std::string address(void);

				void close(void);

				orbit_socket_family_t family(void);

				bool is_open(void);

				void open_tcp(void);

				void open_tcp(
					__in const std::string &host,
					__in uint16_t port
					);

				uint16_t port(void);

				int read(
					__in orbit_buf_t &output
					);

				int read(
					__in std::string &output
					);

				virtual std::string to_string(
					__in_opt bool verbose = false
					);

				orbit_socket_t type(void);

				int write(
					__in const orbit_buf_t &input
					);

				int write(
					__in const std::string &input
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

		typedef class _orbit_socket_factory {

			public:

				~_orbit_socket_factory(void);

				static _orbit_socket_factory *acquire(void);

				orbit_socket &at(
					__in const orbit_uid &uid
					);

				bool contains(
					__in const orbit_uid &uid
					);

				size_t decrement_reference(
					__in const orbit_uid &uid
					);

				orbit_uid generate(
					__in_opt const std::string &host = std::string(),
					__in_opt uint16_t port = 0
					);

				size_t increment_reference(
					__in const orbit_uid &uid
					);

				void initialize(void);

				static bool is_allocated(void);

				bool is_initialized(void);

				size_t reference_count(
					__in const orbit_uid &uid
					);

				size_t size(void);

				std::string to_string(
					__in_opt bool verbose = false
					);

				void uninitialize(void);

			protected:

				_orbit_socket_factory(void);

				_orbit_socket_factory(
					__in const _orbit_socket_factory &other
					);

				_orbit_socket_factory &operator=(
					__in const _orbit_socket_factory &other
					);

				static void _delete(void);

				std::map<orbit_uid, std::pair<orbit_socket, size_t>>::iterator find(
					__in const orbit_uid &uid
					);

				bool m_initialized;

				static _orbit_socket_factory *m_instance;

				std::map<orbit_uid, std::pair<orbit_socket, size_t>> m_socket_map;

			private:

				std::recursive_mutex m_lock;

		} orbit_socket_factory, *orbit_socket_factory_ptr;
	}
}

#endif // ORBIT_SOCKET_H_
