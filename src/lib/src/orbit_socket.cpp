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

		#define SOCKET_BLOCK_READ_LEN 0x2000
		#define SOCKET_ADDR_STR_MAX 80

		static const std::string ORBIT_SOCKET_TYPE_STR[] = {
			"NONE", "TCP", "UDP",
			};

		#define ORBIT_SOCKET_TYPE_STRING(_TYPE_) \
			((_TYPE_) > ORBIT_SOCKET_TYPE_MAX ? UNKNOWN : \
			CHECK_STR(ORBIT_SOCKET_TYPE_STR[_TYPE_]))

		static const std::string ORBIT_SOCKET_FAMILY_TYPE_STR[] = {
			"NONE", "IPV4", "IPV6",
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

		std::string 
		_orbit_socket::address(void)
		{
			void *addr = NULL;
			std::string result;

			SERIALIZE_CALL_RECUR(m_lock);

			if(!m_socket) {
				THROW_ORBIT_SOCKET_EXCEPTION(ORBIT_SOCKET_EXCEPTION_CLOSE);
			}

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

				result.resize(SOCKET_ADDR_STR_MAX);

				if(!inet_ntop(m_information->ai_family, addr, (char *) &result[0], 
						SOCKET_ADDR_STR_MAX)) {
					THROW_ORBIT_SOCKET_EXCEPTION_MESSAGE(ORBIT_SOCKET_EXCEPTION_INTERNAL,
						"[%s] %s", CONCAT_STR(inet_ntop), strerror(errno));
				}
			}

			return CHECK_STR(result);
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
			orbit_socket_family_t result = ORBIT_SOCKET_FAMILY_TYPE_NONE;

			SERIALIZE_CALL_RECUR(m_lock);

			if(!m_socket) {
				THROW_ORBIT_SOCKET_EXCEPTION(ORBIT_SOCKET_EXCEPTION_CLOSE);
			}

			if(m_information) {

				switch(m_information->ai_family) {
					case AF_INET:
						result = ORBIT_SOCKET_FAMILY_TYPE_IPV4;
						break;
					case AF_INET6:
						result = ORBIT_SOCKET_FAMILY_TYPE_IPV6;
						break;
					default:
						THROW_ORBIT_SOCKET_EXCEPTION_MESSAGE(ORBIT_SOCKET_EXCEPTION_TYPE_INET,
							"%i", m_information->ai_family);
				}
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

			memset(&m_address_4, 0, sizeof(sockaddr_in));
			memset(&m_address_6, 0, sizeof(sockaddr_in6));
			m_host = host;
			m_port = port;
			m_type = ORBIT_SOCKET_TYPE_TCP;

			result = getaddrinfo(CHECK_STR(host), NULL, NULL, &m_information);
			if(result) {
				THROW_ORBIT_SOCKET_EXCEPTION_MESSAGE(ORBIT_SOCKET_EXCEPTION_INTERNAL,
					"[%s] %s", CONCAT_STR(getaddrinfo), gai_strerror(result));
			}

			m_socket = ::socket(m_information->ai_family, SOCK_STREAM, 0);
			if(!m_socket) {
				THROW_ORBIT_SOCKET_EXCEPTION_MESSAGE(ORBIT_SOCKET_EXCEPTION_INTERNAL,
					"[%s] %s", CONCAT_STR(::socket), strerror(errno));
			}

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

		uint16_t 
		_orbit_socket::port(void)
		{
			SERIALIZE_CALL_RECUR(m_lock);

			if(!m_socket) {
				THROW_ORBIT_SOCKET_EXCEPTION(ORBIT_SOCKET_EXCEPTION_CLOSE);
			}

			return m_port;	
		}

		int 
		_orbit_socket::read(
			__in orbit_buf_t &output
			)
		{
			int result = 0, len;
			bool receiving = true;
			orbit_buf_t buffer;

			SERIALIZE_CALL_RECUR(m_lock);

			if(!m_socket) {
				THROW_ORBIT_SOCKET_EXCEPTION(ORBIT_SOCKET_EXCEPTION_CLOSE);
			}

			output.clear();

			while(receiving) {
				buffer.resize(SOCKET_BLOCK_READ_LEN, 0);

				len = ::recv(m_socket, (uint8_t *) &buffer[0], SOCKET_BLOCK_READ_LEN, 0);
				if(len < 0) {
					THROW_ORBIT_SOCKET_EXCEPTION_MESSAGE(ORBIT_SOCKET_EXCEPTION_INTERNAL,
						"[%s] %s", CONCAT_STR(::recv), strerror(errno));
				} else if(len) {					
					output.insert(output.end(), buffer.begin(), buffer.begin() + len);
					result += len;
				} else {
					receiving = false;
				}
			}

			return result;
		}

		int 
		_orbit_socket::read(
			__in std::string &output
			)
		{
			int result;
			orbit_buf_t buffer;

			SERIALIZE_CALL_RECUR(m_lock);

			if(!m_socket) {
				THROW_ORBIT_SOCKET_EXCEPTION(ORBIT_SOCKET_EXCEPTION_CLOSE);
			}

			result = read(buffer);
			output = std::string(buffer.begin(), buffer.end());

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

			result << CHECK_STR(orbit_uid::to_string()) << " [" << ORBIT_SOCKET_TYPE_STRING(m_type) 
				<< ", " << (m_socket ? "CONN" : "DISC");

			if(m_socket) {
				result << ", " << ORBIT_SOCKET_FAMILY_TYPE_STRING(family());
			}

			result << "]";

			if(verbose && m_socket) {
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

					result << " " << CHECK_STR(addr_str);
				}

				result << ":" << m_port;
			}

			return CHECK_STR(result.str());
		}

		orbit_socket_t 
		_orbit_socket::type(void)
		{
			SERIALIZE_CALL_RECUR(m_lock);

			if(!m_socket) {
				THROW_ORBIT_SOCKET_EXCEPTION(ORBIT_SOCKET_EXCEPTION_CLOSE);
			}

			return m_type;
		}

		int 
		_orbit_socket::write(
			__in const orbit_buf_t &input
			)
		{
			int result;

			SERIALIZE_CALL_RECUR(m_lock);

			if(!m_socket) {
				THROW_ORBIT_SOCKET_EXCEPTION(ORBIT_SOCKET_EXCEPTION_CLOSE);
			}

			result = ::write(m_socket, (uint8_t *) &input[0], input.size());
			if(result < 0) {
				THROW_ORBIT_SOCKET_EXCEPTION_MESSAGE(ORBIT_SOCKET_EXCEPTION_INTERNAL,
					"[%s] %s", CONCAT_STR(::write), strerror(errno));
			}

			return result;
		}

		int 
		_orbit_socket::write(
			__in const std::string &input
			)
		{
			orbit_buf_t buffer;

			SERIALIZE_CALL_RECUR(m_lock);

			if(!m_socket) {
				THROW_ORBIT_SOCKET_EXCEPTION(ORBIT_SOCKET_EXCEPTION_CLOSE);
			}

			buffer = orbit_buf_t(input.begin(), input.end());

			return write(buffer);
		}

		orbit_socket_factory_ptr orbit_socket_factory::m_instance = NULL;

		_orbit_socket_factory::_orbit_socket_factory(void) :
			m_initialized(false)
		{
			std::atexit(orbit_socket_factory::_delete);
		}

		_orbit_socket_factory::~_orbit_socket_factory(void)
		{

			if(m_initialized) {
				uninitialize();
			}
		}

		void 
		_orbit_socket_factory::_delete(void)
		{

			if(orbit_socket_factory::m_instance) {
				delete orbit_socket_factory::m_instance;
				orbit_socket_factory::m_instance = NULL;
			}
		}

		orbit_socket_factory_ptr 
		_orbit_socket_factory::acquire(void)
		{

			if(!orbit_socket_factory::m_instance) {

				orbit_socket_factory::m_instance = new orbit_socket_factory;
				if(!orbit_socket_factory::m_instance) {
					THROW_ORBIT_SOCKET_EXCEPTION(ORBIT_SOCKET_EXCEPTION_ALLOCATION);
				}
			}

			return orbit_socket_factory::m_instance;
		}

		orbit_socket &
		_orbit_socket_factory::at(
			__in const orbit_uid &uid
			)
		{
			SERIALIZE_CALL_RECUR(m_lock);

			if(!m_initialized) {
				THROW_ORBIT_SOCKET_EXCEPTION(ORBIT_SOCKET_EXCEPTION_UNINITIALIZE);
			}

			return find(uid)->second.first;
		}

		bool 
		_orbit_socket_factory::contains(
			__in const orbit_uid &uid
			)
		{
			SERIALIZE_CALL_RECUR(m_lock);

			if(!m_initialized) {
				THROW_ORBIT_SOCKET_EXCEPTION(ORBIT_SOCKET_EXCEPTION_UNINITIALIZE);
			}

			return (m_map_socket.find(uid) != m_map_socket.end());
		}

		size_t 
		_orbit_socket_factory::decrement_reference(
			__in const orbit_uid &uid
			)
		{
			size_t result;
			std::map<orbit_uid, std::pair<orbit_socket, size_t>>::iterator iter;

			SERIALIZE_CALL_RECUR(m_lock);

			if(!m_initialized) {
				THROW_ORBIT_SOCKET_EXCEPTION(ORBIT_SOCKET_EXCEPTION_UNINITIALIZE);
			}

			iter = find(uid);

			result = --iter->second.second;
			if(result < REFERENCE_INIT) {
				m_map_socket.erase(iter);
			}

			return result;
		}

		std::map<orbit_uid, std::pair<orbit_socket, size_t>>::iterator 
		_orbit_socket_factory::find(
			__in const orbit_uid &uid
			)
		{
			std::map<orbit_uid, std::pair<orbit_socket, size_t>>::iterator result;

			SERIALIZE_CALL_RECUR(m_lock);

			if(!m_initialized) {
				THROW_ORBIT_SOCKET_EXCEPTION(ORBIT_SOCKET_EXCEPTION_UNINITIALIZE);
			}

			result = m_map_socket.find(uid);
			if(result == m_map_socket.end()) {
				THROW_ORBIT_SOCKET_EXCEPTION_MESSAGE(ORBIT_SOCKET_EXCEPTION_NOT_FOUND,
					"%s", CHECK_STR(orbit_uid::as_string(uid)));
			}

			return result;
		}

		orbit_uid 
		_orbit_socket_factory::generate_tcp(
			__in_opt const std::string &host,
			__in_opt uint16_t port
			)
		{
			orbit_uid result;

			SERIALIZE_CALL_RECUR(m_lock);

			if(!m_initialized) {
				THROW_ORBIT_SOCKET_EXCEPTION(ORBIT_SOCKET_EXCEPTION_UNINITIALIZE);
			}

			orbit_socket sock(host, port);
			result = sock.uid();
			m_map_socket.insert(std::pair<orbit_uid, std::pair<orbit_socket, size_t>>(
				result, std::pair<orbit_socket, size_t>(sock, REFERENCE_INIT)));

			return result;
		}

		size_t 
		_orbit_socket_factory::increment_reference(
			__in const orbit_uid &uid
			)
		{
			SERIALIZE_CALL_RECUR(m_lock);

			if(!m_initialized) {
				THROW_ORBIT_SOCKET_EXCEPTION(ORBIT_SOCKET_EXCEPTION_UNINITIALIZE);
			}

			return ++find(uid)->second.second;
		}

		void 
		_orbit_socket_factory::initialize(void)
		{
			SERIALIZE_CALL_RECUR(m_lock);

			if(m_initialized) {
				THROW_ORBIT_SOCKET_EXCEPTION(ORBIT_SOCKET_EXCEPTION_INITIALIZE);
			}

			m_initialized = true;
			m_map_socket.clear();
		}

		bool 
		_orbit_socket_factory::is_allocated(void)
		{
			return (orbit_socket_factory::m_instance != NULL);
		}

		bool 
		_orbit_socket_factory::is_initialized(void)
		{
			SERIALIZE_CALL_RECUR(m_lock);
			return m_initialized;
		}

		size_t 
		_orbit_socket_factory::reference_count(
			__in const orbit_uid &uid
			)
		{
			SERIALIZE_CALL_RECUR(m_lock);

			if(!m_initialized) {
				THROW_ORBIT_SOCKET_EXCEPTION(ORBIT_SOCKET_EXCEPTION_UNINITIALIZE);
			}

			return find(uid)->second.second;
		}

		size_t 
		_orbit_socket_factory::size(void)
		{
			SERIALIZE_CALL_RECUR(m_lock);

			if(!m_initialized) {
				THROW_ORBIT_SOCKET_EXCEPTION(ORBIT_SOCKET_EXCEPTION_UNINITIALIZE);
			}

			return m_map_socket.size();
		}

		std::string 
		_orbit_socket_factory::to_string(
			__in_opt bool verbose
			)
		{
			size_t index = 1;
			std::stringstream result;
			std::map<orbit_uid, std::pair<orbit_socket, size_t>>::iterator iter;

			SERIALIZE_CALL_RECUR(m_lock);

			result << "[" << (m_initialized ? "INIT" : "UNINIT") << "] " 
				<< ORBIT_SOCKET_HEADER;

			if(verbose) {
				result << " (" << VALUE_AS_HEX(uintptr_t, this) << ")";
			}

			for(iter = m_map_socket.begin(); iter != m_map_socket.end(); ++index, ++iter) {
				result << std::endl << "--- [" << index << "/" << m_map_socket.size() << "] "
					<< iter->second.first.to_string(verbose) << ", ref: "
					<< iter->second.second;
			}

			return CHECK_STR(result.str());
		}

		void 
		_orbit_socket_factory::uninitialize(void)
		{
			SERIALIZE_CALL_RECUR(m_lock);

			if(!m_initialized) {
				THROW_ORBIT_SOCKET_EXCEPTION(ORBIT_SOCKET_EXCEPTION_UNINITIALIZE);
			}

			m_map_socket.clear();
			m_initialized =false;
		}
	}
}
