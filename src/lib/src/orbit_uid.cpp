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

#include "../include/orbit.h"
#include "../include/orbit_uid_type.h"

namespace ORBIT {

	namespace COMPONENT {

		_orbit_uid::_orbit_uid(
			__in_opt orbit_uid_t uid
			) :
				m_uid(uid)
		{
			return;
		}

		_orbit_uid::_orbit_uid(
			__in const _orbit_uid &other
			) :
				m_uid(other.m_uid)
		{
			return;
		}

		_orbit_uid::~_orbit_uid(void)
		{
			return;
		}

		_orbit_uid &
		_orbit_uid::operator=(
			__in const _orbit_uid &other
			)
		{
			SERIALIZE_CALL_RECUR(m_lock);

			if(this != &other) {
				m_uid = other.m_uid;
			}

			return *this;
		}

		bool 
		_orbit_uid::operator==(
			__in const _orbit_uid &other
			)
		{
			SERIALIZE_CALL_RECUR(m_lock);
			return (m_uid == other.m_uid);
		}

		bool 
		_orbit_uid::operator!=(
			__in const _orbit_uid &other
			)
		{
			SERIALIZE_CALL_RECUR(m_lock);
			return !(*this == other);
		}

		std::string 
		_orbit_uid::as_string(
			__in const _orbit_uid &uid,
			__in_opt bool verbose
			)
		{
			std::stringstream result;

			UNREFERENCE_PARAM(verbose);

			result << "{" << VALUE_AS_HEX(orbit_uid_t, uid.m_uid) << "}";

			return CHECK_STR(result.str());
		}

		std::string 
		_orbit_uid::to_string(
			__in_opt bool verbose
			)
		{
			SERIALIZE_CALL_RECUR(m_lock);
			return CHECK_STR(as_string(*this, verbose));
		}

		orbit_uid_t &
		_orbit_uid::uid(void)
		{
			SERIALIZE_CALL_RECUR(m_lock);
			return m_uid;
		}

		bool 
		operator<(
			__in const _orbit_uid &left,
			__in const _orbit_uid &right
			)
		{
			return (left.m_uid < right.m_uid);
		}

		_orbit_uid_class::_orbit_uid_class(void) :
			orbit_uid(orbit::acquire()->acquire_uid_factory()->generate())
		{
			return;
		}

		_orbit_uid_class::_orbit_uid_class(
			__in const _orbit_uid_class &other
			) :
				orbit_uid(other)
		{
			orbit_uid_factory_ptr fact = orbit::acquire()->acquire_uid_factory();

			if(fact->contains(*this)) {
				fact->increment_reference(*this);
			}
		}

		_orbit_uid_class::~_orbit_uid_class(void)
		{
			orbit_uid_factory_ptr fact = orbit::acquire()->acquire_uid_factory();

			if(fact->is_initialized() 
					&& fact->contains(*this)) {
				fact->decrement_reference(*this);
			}
		}

		_orbit_uid_class &
		_orbit_uid_class::operator=(
			__in const _orbit_uid_class &other
			)
		{
			orbit_uid_factory_ptr fact = orbit::acquire()->acquire_uid_factory();

			SERIALIZE_CALL_RECUR(m_lock);

			if(this != &other) {

				if(fact->contains(*this)) {
					fact->decrement_reference(*this);
				}

				orbit_uid::operator=(other);

				if(fact->contains(*this)) {
					fact->increment_reference(*this);
				}
			}

			return *this;
		}

		bool 
		_orbit_uid_class::contains(void)
		{
			SERIALIZE_CALL_RECUR(m_lock);
			return orbit::acquire()->acquire_uid_factory()->contains(*this);
		}

		size_t 
		_orbit_uid_class::decrement_reference(void)
		{
			SERIALIZE_CALL_RECUR(m_lock);
			return orbit::acquire()->acquire_uid_factory()->decrement_reference(*this);
		}

		size_t 
		_orbit_uid_class::increment_reference(void)
		{
			SERIALIZE_CALL_RECUR(m_lock);
			return orbit::acquire()->acquire_uid_factory()->increment_reference(*this);
		}

		size_t 
		_orbit_uid_class::reference_count(void)
		{
			SERIALIZE_CALL_RECUR(m_lock);
			return orbit::acquire()->acquire_uid_factory()->reference_count(*this);
		}

		orbit_uid_factory_ptr orbit_uid_factory::m_instance = NULL;

		_orbit_uid_factory::_orbit_uid_factory(void) :
			m_initialized(false)
		{
			std::atexit(orbit_uid_factory::_delete);
		}

		_orbit_uid_factory::~_orbit_uid_factory(void)
		{

			if(m_initialized) {
				uninitialize();
			}
		}

		void 
		_orbit_uid_factory::_delete(void)
		{

			if(orbit_uid_factory::m_instance) {
				delete orbit_uid_factory::m_instance;
				orbit_uid_factory::m_instance = NULL;
			}
		}				

		orbit_uid_factory_ptr 
		_orbit_uid_factory::acquire(void)
		{

			if(!orbit_uid_factory::m_instance) {

				orbit_uid_factory::m_instance = new orbit_uid_factory;
				if(!orbit_uid_factory::m_instance) {
					THROW_ORBIT_UID_EXCEPTION(ORBIT_UID_EXCEPTION_ALLOCATION);
				}
			}

			return orbit_uid_factory::m_instance;
		}

		bool 
		_orbit_uid_factory::contains(
			__in const orbit_uid &uid
			)
		{
			SERIALIZE_CALL_RECUR(m_lock);

			if(!m_initialized) {
				THROW_ORBIT_UID_EXCEPTION(ORBIT_UID_EXCEPTION_UNINITIALIZE);
			}

			return (m_map_uid.find(uid) != m_map_uid.end());
		}

		size_t 
		_orbit_uid_factory::decrement_reference(
			__in const orbit_uid &uid
			)
		{
			size_t result;
			std::map<orbit_uid, size_t>::iterator iter;

			SERIALIZE_CALL_RECUR(m_lock);

			if(!m_initialized) {
				THROW_ORBIT_UID_EXCEPTION(ORBIT_UID_EXCEPTION_UNINITIALIZE);
			}

			iter = find(uid);
			result = --iter->second;
			if(result < REFERENCE_INIT) {
				m_set_uid.insert(iter->first.m_uid);
				m_map_uid.erase(iter);
			}

			return result;
		}

		std::map<orbit_uid, size_t>::iterator 
		_orbit_uid_factory::find(
			__in const orbit_uid &uid
			)
		{
			std::map<orbit_uid, size_t>::iterator result;

			SERIALIZE_CALL_RECUR(m_lock);

			if(!m_initialized) {
				THROW_ORBIT_UID_EXCEPTION(ORBIT_UID_EXCEPTION_UNINITIALIZE);
			}

			result = m_map_uid.find(uid);
			if(result == m_map_uid.end()) {
				THROW_ORBIT_UID_EXCEPTION_MESSAGE(ORBIT_UID_EXCEPTION_NOT_FOUND, 
					"{%x}", uid.m_uid);
			}

			return result;
		}

		orbit_uid 
		_orbit_uid_factory::generate(void)
		{
			orbit_uid result;

			SERIALIZE_CALL_RECUR(m_lock);

			if(!m_initialized) {
				THROW_ORBIT_UID_EXCEPTION(ORBIT_UID_EXCEPTION_UNINITIALIZE);
			}

			if(!m_set_uid.empty()) {
				result.uid() = *m_set_uid.begin();
				m_set_uid.erase(result.uid());
			} else if(m_next_uid != UID_INVALID) {
				result.uid() = m_next_uid++;
			} else {
				THROW_ORBIT_UID_EXCEPTION(ORBIT_UID_EXCEPTION_INSUFFICENT);
			}

			m_map_uid.insert(std::pair<orbit_uid, size_t>(result, REFERENCE_INIT));

			return result;
		}

		size_t 
		_orbit_uid_factory::increment_reference(
			__in const orbit_uid &uid
			)
		{
			SERIALIZE_CALL_RECUR(m_lock);

			if(!m_initialized) {
				THROW_ORBIT_UID_EXCEPTION(ORBIT_UID_EXCEPTION_UNINITIALIZE);
			}

			return ++find(uid)->second;
		}

		void 
		_orbit_uid_factory::initialize(void)
		{
			SERIALIZE_CALL_RECUR(m_lock);

			if(m_initialized) {
				THROW_ORBIT_UID_EXCEPTION(ORBIT_UID_EXCEPTION_INITIALIZE);
			}

			m_initialized = true;
			m_map_uid.clear();
			m_next_uid = 0;
			m_set_uid.clear();
		}

		bool 
		_orbit_uid_factory::is_allocated(void)
		{
			return (orbit_uid_factory::m_instance != NULL);
		}

		bool 
		_orbit_uid_factory::is_initialized(void)
		{
			SERIALIZE_CALL_RECUR(m_lock);
			return m_initialized;
		}

		size_t 
		_orbit_uid_factory::reference_count(
			__in const orbit_uid &uid
			)
		{
			SERIALIZE_CALL_RECUR(m_lock);

			if(!m_initialized) {
				THROW_ORBIT_UID_EXCEPTION(ORBIT_UID_EXCEPTION_UNINITIALIZE);
			}

			return find(uid)->second;
		}

		size_t 
		_orbit_uid_factory::size(void)
		{
			SERIALIZE_CALL_RECUR(m_lock);

			if(!m_initialized) {
				THROW_ORBIT_UID_EXCEPTION(ORBIT_UID_EXCEPTION_UNINITIALIZE);
			}

			return m_map_uid.size();
		}

		std::string 
		_orbit_uid_factory::to_string(
			__in_opt bool verbose
			)
		{
			size_t index = 1;
			std::stringstream result;
			std::map<orbit_uid, size_t>::iterator iter;

			SERIALIZE_CALL_RECUR(m_lock);

			result << "[" << (m_initialized ? "INIT" : "UNINIT") << "] " 
				<< ORBIT_UID_HEADER;

			if(verbose) {
				result << " (" << VALUE_AS_HEX(uintptr_t, this) << ")";
			}

			for(iter = m_map_uid.begin(); iter != m_map_uid.end(); ++index, ++iter) {
				result << std::endl << "--- [" << index << "/" << m_map_uid.size() << "] {"
					<< VALUE_AS_HEX(orbit_uid_t, iter->first.m_uid) << "}, ref: "
					<< iter->second;
			}

			return CHECK_STR(result.str());
		}

		void 
		_orbit_uid_factory::uninitialize(void)
		{
			SERIALIZE_CALL_RECUR(m_lock);

			if(!m_initialized) {
				THROW_ORBIT_UID_EXCEPTION(ORBIT_UID_EXCEPTION_UNINITIALIZE);
			}

			m_map_uid.clear();
			m_next_uid = 0;
			m_set_uid.clear();
			m_initialized = false;
		}
	}
}
