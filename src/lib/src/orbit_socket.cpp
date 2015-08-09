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

#include <string.h>
#include "../include/orbit.h"
#include "../include/orbit_socket_type.h"

namespace ORBIT {

	namespace COMPONENT {

		static const std::string ORBIT_SOCKET_TYPE_STR[] = {
			"NONE", "TCP", "UDP",
			};

		#define ORBIT_SOCKET_TYPE_STRING(_TYPE_) \
			((_TYPE_) > ORBIT_SOCKET_TYPE_MAX ? UNKNOWN : \
			CHECK_STR(ORBIT_SOCKET_TYPE_STR[_TYPE_]))

		_orbit_socket::_orbit_socket(void) :
			m_information(NULL),
			m_port(0),
			m_socket(0),
			m_type(ORBIT_SOCKET_TYPE_NONE)
		{
			memset(&m_address, 0, sizeof(sockaddr_in));
		}

		_orbit_socket::_orbit_socket(
			__in const _orbit_socket &other
			) :
				m_information(NULL),
				m_port(0),
				m_socket(0),
				m_type(ORBIT_SOCKET_TYPE_NONE)
		{
			memset(&m_address, 0, sizeof(sockaddr_in));
		}

		_orbit_socket::~_orbit_socket(void)
		{

			if(is_open()) {
				close();
			}
		}

		_orbit_socket &
		_orbit_socket::operator=(
			__in const _orbit_socket &other
			)
		{
			SERIALIZE_CALL_RECUR(m_lock);

			if(this != &other) {
				memset(&m_address, 0, sizeof(sockaddr_in));
				m_host.clear();
				m_information = NULL;
				m_port = 0;
				m_socket = 0;
				m_type = ORBIT_SOCKET_TYPE_NONE;
			}

			return *this;
		}

		void 
		_orbit_socket::close(void)
		{
			SERIALIZE_CALL_RECUR(m_lock);

			if(!m_socket) {
				THROW_ORBIT_SOCKET_EXCEPTION(ORBIT_SOCKET_EXCEPTION_CLOSE);
			}

			// TODO

			memset(&m_address, 0, sizeof(sockaddr_in));
			m_host.clear();
			m_information = NULL;
			m_port = 0;
			m_socket = 0;
			m_type = ORBIT_SOCKET_TYPE_NONE;
		}

		bool 
		_orbit_socket::is_open(void)
		{
			SERIALIZE_CALL_RECUR(m_lock);
			return (m_socket != 0);
		}

		void 
		_orbit_socket::open(
			__in const std::string &host,
			__in uint16_t port
			)
		{
			SERIALIZE_CALL_RECUR(m_lock);

			if(m_socket) {
				THROW_ORBIT_SOCKET_EXCEPTION(ORBIT_SOCKET_EXCEPTION_OPEN);
			}

			// TODO
		}

		size_t 
		_orbit_socket::read(
			__in orbit_socket_buf_t &buffer
			)
		{
			size_t result;

			SERIALIZE_CALL_RECUR(m_lock);

			if(!m_socket) {
				THROW_ORBIT_SOCKET_EXCEPTION(ORBIT_SOCKET_EXCEPTION_CLOSE);
			}

			// TODO
			result = 0;
			// ---

			return result;
		}

		std::string 
		_orbit_socket::to_string(
			__in_opt bool verbose
			)
		{
			std::stringstream result;

			SERIALIZE_CALL_RECUR(m_lock);

			// TODO

			return CHECK_STR(result.str());
		}

		orbit_socket_t 
		_orbit_socket::type(void)
		{
			SERIALIZE_CALL_RECUR(m_lock);
			return m_type;
		}

		size_t 
		_orbit_socket::write(
			__in const orbit_socket_buf_t &buffer
			)
		{
			size_t result;

			SERIALIZE_CALL_RECUR(m_lock);

			if(!m_socket) {
				THROW_ORBIT_SOCKET_EXCEPTION(ORBIT_SOCKET_EXCEPTION_CLOSE);
			}

			// TODO
			result = 0;
			// ---

			return result;
		}
	}
}
