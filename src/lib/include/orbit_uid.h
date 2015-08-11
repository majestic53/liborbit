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

#ifndef ORBIT_UID_H_
#define ORBIT_UID_H_

#include <map>
#include <set>

namespace ORBIT {

	namespace COMPONENT {

		typedef uint32_t orbit_uid_t;

		#define UID_INVALID INVALID_TYPE(orbit_uid_t)

		typedef class _orbit_uid {

			public:

				_orbit_uid(
					__in_opt orbit_uid_t uid = UID_INVALID
					);

				_orbit_uid(
					__in const _orbit_uid &other
					);

				virtual ~_orbit_uid(void);

				_orbit_uid &operator=(
					__in const _orbit_uid &other
					);

				bool operator==(
					__in const _orbit_uid &other
					);

				bool operator!=(
					__in const _orbit_uid &other
					);

				static std::string as_string(
					__in const _orbit_uid &uid,
					__in_opt bool verbose = false
					);

				virtual std::string to_string(
					__in_opt bool verbose = false
					);

				orbit_uid_t &uid(void);

			protected:

				friend bool operator<(
					__in const _orbit_uid &left,
					__in const _orbit_uid &right
					);

				friend class _orbit_uid_factory;

				orbit_uid_t m_uid;

			private:

				std::recursive_mutex m_lock;

		} orbit_uid, *orbit_uid_ptr;

		extern bool operator<(
			__in const _orbit_uid &left,
			__in const _orbit_uid &right
			);

		typedef class _orbit_uid_class :
				public orbit_uid {

			public:

				_orbit_uid_class(void);

				_orbit_uid_class(
					__in const _orbit_uid_class &other
					);

				virtual ~_orbit_uid_class(void);

				_orbit_uid_class &operator=(
					__in const _orbit_uid_class &other
					);

				bool contains(void);

				size_t decrement_reference(void);

				size_t increment_reference(void);

				size_t reference_count(void);

			private:

				std::recursive_mutex m_lock;

		} orbit_uid_class, *orbit_uid_class_ptr;

		typedef class _orbit_uid_factory {

			public:

				~_orbit_uid_factory(void);

				static _orbit_uid_factory *acquire(void);

				bool contains(
					__in const orbit_uid &uid
					);

				size_t decrement_reference(
					__in const orbit_uid &uid
					);

				orbit_uid generate(void);

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

				_orbit_uid_factory(void);

				_orbit_uid_factory(
					__in const _orbit_uid_factory &other
					);

				_orbit_uid_factory &operator=(
					__in const _orbit_uid_factory &other
					);

				static void _delete(void);

				std::map<orbit_uid, size_t>::iterator find(
					__in const orbit_uid &uid
					);

				bool m_initialized;

				static _orbit_uid_factory *m_instance;

				std::map<orbit_uid, size_t> m_uid_map;

				orbit_uid_t m_uid_next;

				std::set<orbit_uid_t> m_uid_set;

			private:

				std::recursive_mutex m_lock;

		} orbit_uid_factory, *orbit_uid_factory_ptr;
	}
}

#endif // ORBIT_UID_H_
