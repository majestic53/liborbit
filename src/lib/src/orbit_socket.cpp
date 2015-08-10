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
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include "../include/orbit.h"
#include "../include/orbit_socket_type.h"

namespace ORBIT {

	namespace COMPONENT {

		#define SOCKET_ADDR_STR_MAX 80

		static const std::string ORBIT_SOCKET_TYPE_STR[] = {
			"NONE", "TCP", "UDP",
			};

		#define ORBIT_SOCKET_TYPE_STRING(_TYPE_) \
			((_TYPE_) > ORBIT_SOCKET_TYPE_MAX ? UNKNOWN : \
			CHECK_STR(ORBIT_SOCKET_TYPE_STR[_TYPE_]))

		static const std::string ORBIT_SOCKET_FAMILY_TYPE_STR[] = {
			"IPv4", "IPv6",
			};

		#define ORBIT_SOCKET_FAMILY_TYPE_STRING(_TYPE_) \
			((_TYPE_) > ORBIT_SOCKET_FAMILY_TYPE_MAX ? UNKNOWN : \
			CHECK_STR(ORBIT_SOCKET_FAMILY_TYPE_STR[_TYPE_]))

		_orbit_socket::_orbit_socket(
			__in_opt const std::string &host,
			__in_opt uint16_t port
			) :
				m_host(host),
				m_information(NULL),
				m_port(port),
				m_socket(0),
				m_type(ORBIT_SOCKET_TYPE_NONE)
		{
			memset(&m_address_4, 0, sizeof(sockaddr_in));
			memset(&m_address_6, 0, sizeof(sockaddr_in6));
		}

		_orbit_socket::_orbit_socket(
			__in const _orbit_socket &other
			) :
				m_host(other.m_host),
				m_information(NULL),
				m_port(other.m_port),
				m_socket(0),
				m_type(ORBIT_SOCKET_TYPE_NONE)
		{
			memset(&m_address_4, 0, sizeof(sockaddr_in));
			memset(&m_address_6, 0, sizeof(sockaddr_in6));
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

				if(m_socket) {

					if(::close(m_socket) < 0) {
						THROW_ORBIT_SOCKET_EXCEPTION_MESSAGE(ORBIT_SOCKET_EXCEPTION_INTERNAL,
							"[%s] %s", CONCAT_STR(::close), strerror(errno));
					}

					m_socket = 0;
				}

				if(m_information) {
					freeaddrinfo(m_information);
					m_information = NULL;
				}

				memset(&m_address_4, 0, sizeof(sockaddr_in));
				memset(&m_address_6, 0, sizeof(sockaddr_in6));
				m_host = other.m_host;
				m_port = other.m_port;
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

			if(m_socket) {

				if(::close(m_socket) < 0) {
					THROW_ORBIT_SOCKET_EXCEPTION_MESSAGE(ORBIT_SOCKET_EXCEPTION_INTERNAL,
						"[%s] %s", CONCAT_STR(::close), strerror(errno));
				}

				m_socket = 0;
			}

			if(m_information) {
				freeaddrinfo(m_information);
				m_information = NULL;
			}

			memset(&m_address_4, 0, sizeof(sockaddr_in));
			memset(&m_address_6, 0, sizeof(sockaddr_in6));
			m_host.clear();
			m_port = 0;
			m_type = ORBIT_SOCKET_TYPE_NONE;
		}

		orbit_socket_family_t 
		_orbit_socket::family(void)
		{
			orbit_socket_family_t result = INVALID_TYPE(orbit_socket_family_t);

			SERIALIZE_CALL_RECUR(m_lock);

			if(!m_socket) {
				THROW_ORBIT_SOCKET_EXCEPTION(ORBIT_SOCKET_EXCEPTION_CLOSE);
			}

			switch(m_information->ai_family) {
				case AF_INET:
					result = ORBIT_SOCKET_FAMILY_TYPE_4;
					break;
				case AF_INET6:
					result = ORBIT_SOCKET_FAMILY_TYPE_6;
					break;
				default:
					THROW_ORBIT_SOCKET_EXCEPTION_MESSAGE(ORBIT_SOCKET_EXCEPTION_TYPE_INET,
						"%i", m_information->ai_family);
			}

			return result;
		}

		bool 
		_orbit_socket::is_open(void)
		{
			SERIALIZE_CALL_RECUR(m_lock);
			return (m_socket != 0);
		}

		void 
		_orbit_socket::open_tcp(void)
		{
			SERIALIZE_CALL_RECUR(m_lock);

			if(m_socket) {
				THROW_ORBIT_SOCKET_EXCEPTION(ORBIT_SOCKET_EXCEPTION_OPEN);
			}

			open_tcp(m_host, m_port);
		}

		void 
		_orbit_socket::open_tcp(
			__in const std::string &host,
			__in uint16_t port
			)
		{
			int result;

			SERIALIZE_CALL_RECUR(m_lock);

			if(m_socket) {
				THROW_ORBIT_SOCKET_EXCEPTION(ORBIT_SOCKET_EXCEPTION_OPEN);
			}

			m_host = host;
			m_port = port;
			m_type = ORBIT_SOCKET_TYPE_TCP;

			m_socket = ::socket(AF_INET, SOCK_STREAM, 0);
			if(m_socket) {
				THROW_ORBIT_SOCKET_EXCEPTION_MESSAGE(ORBIT_SOCKET_EXCEPTION_INTERNAL,
					"[%s] %s", CONCAT_STR(::socket), strerror(errno));
			}

			result = getaddrinfo(CHECK_STR(host), NULL, NULL, &m_information);
			if(result) {
				THROW_ORBIT_SOCKET_EXCEPTION_MESSAGE(ORBIT_SOCKET_EXCEPTION_INTERNAL,
					"[%s] %s", CONCAT_STR(getaddrinfo), gai_strerror(result));
			}

			memset(&m_address_4, 0, sizeof(sockaddr_in));
			memset(&m_address_6, 0, sizeof(sockaddr_in6));

			switch(m_information->ai_family) {
				case AF_INET:
					memcpy(&m_address_4.sin_addr, &((sockaddr_in *) m_information->ai_addr)->sin_addr, 
						sizeof(m_address_4.sin_addr));
					m_address_4.sin_family = m_information->ai_family;
					m_address_4.sin_port = htons(m_port);

					result = ::connect(m_socket, (sockaddr *) &m_address_4, sizeof(m_address_4));
					if(result < 0) {
						THROW_ORBIT_SOCKET_EXCEPTION_MESSAGE(ORBIT_SOCKET_EXCEPTION_INTERNAL,
							"[%s] %s", CONCAT_STR(::connect), strerror(errno));
					}
					break;
				case AF_INET6:
					memcpy(&m_address_6.sin6_addr, &((sockaddr_in6 *) m_information->ai_addr)->sin6_addr, 
						sizeof(m_address_6.sin6_addr));
					m_address_6.sin6_family = m_information->ai_family;
					m_address_6.sin6_port = htons(m_port);

					result = ::connect(m_socket, (sockaddr *) &m_address_6, sizeof(m_address_6));
					if(result < 0) {
						THROW_ORBIT_SOCKET_EXCEPTION_MESSAGE(ORBIT_SOCKET_EXCEPTION_INTERNAL,
							"[%s] %s", CONCAT_STR(::connect), strerror(errno));
					}
					break;
				default:
					THROW_ORBIT_SOCKET_EXCEPTION_MESSAGE(ORBIT_SOCKET_EXCEPTION_TYPE_INET,
						"%i", m_information->ai_family);
			}
		}

		int 
		_orbit_socket::read(
			__in orbit_socket_buf_t &buffer,
			__in size_t size
			)
		{
			int result;

			SERIALIZE_CALL_RECUR(m_lock);

			if(!m_socket) {
				THROW_ORBIT_SOCKET_EXCEPTION(ORBIT_SOCKET_EXCEPTION_CLOSE);
			}

			buffer.resize(size, 0);

			result = ::read(m_socket, (uint8_t *) &buffer[0], size);
			if(result < 0) {
				THROW_ORBIT_SOCKET_EXCEPTION_MESSAGE(ORBIT_SOCKET_EXCEPTION_INTERNAL,
					"[%s] %s", CONCAT_STR(::read), strerror(errno));
			}

			return result;
		}

		std::string 
		_orbit_socket::to_string(
			__in_opt bool verbose
			)
		{
			void *addr = NULL;
			std::string addr_str;
			std::stringstream result;

			SERIALIZE_CALL_RECUR(m_lock);
			UNREFERENCE_PARAM(verbose);

			result << CHECK_STR(orbit_uid::to_string()) << " [" << ORBIT_SOCKET_TYPE_STRING(m_type) 
				<< ", " << (m_socket ? "CONN" : "DISC");

			if(m_socket) {
				result << ", " << ORBIT_SOCKET_FAMILY_TYPE_STRING(family());
			}

			result << "]";

			if(m_socket) {
				result << " " << CHECK_STR(m_host);

				if(m_information) {

					switch(m_information->ai_family) {
						case AF_INET:
							addr = &((sockaddr_in *) m_information->ai_addr)->sin_addr;
							break;
						case AF_INET6:
							addr = &((sockaddr_in6 *) m_information->ai_addr)->sin6_addr;
							break;
						default:
							THROW_ORBIT_SOCKET_EXCEPTION_MESSAGE(ORBIT_SOCKET_EXCEPTION_TYPE_INET,
								"%i", m_information->ai_family);
					}

					addr_str.resize(SOCKET_ADDR_STR_MAX);
					if(!inet_ntop(m_information->ai_family, addr, (char *) &addr_str[0], 
							SOCKET_ADDR_STR_MAX)) {
						THROW_ORBIT_SOCKET_EXCEPTION_MESSAGE(ORBIT_SOCKET_EXCEPTION_INTERNAL,
							"[%s] %s", CONCAT_STR(inet_ntop), strerror(errno));
					}

					result << " (" << CHECK_STR(addr_str) << ")";
				}

				result << ":" << m_port;
			}

			return CHECK_STR(result.str());
		}

		orbit_socket_t 
		_orbit_socket::type(void)
		{
			SERIALIZE_CALL_RECUR(m_lock);
			return m_type;
		}

		int 
		_orbit_socket::write(
			__in const orbit_socket_buf_t &buffer
			)
		{
			int result;

			SERIALIZE_CALL_RECUR(m_lock);

			if(!m_socket) {
				THROW_ORBIT_SOCKET_EXCEPTION(ORBIT_SOCKET_EXCEPTION_CLOSE);
			}

			result = ::write(m_socket, (uint8_t *) &buffer[0], buffer.size());
			if(result < 0) {
				THROW_ORBIT_SOCKET_EXCEPTION_MESSAGE(ORBIT_SOCKET_EXCEPTION_INTERNAL,
					"[%s] %s", CONCAT_STR(::write), strerror(errno));
			}

			return result;
		}
	}
}
