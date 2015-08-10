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

#include "../lib/include/orbit.h"

int 
main(void)
{
	int result = 0;
	orbit_ptr inst = NULL;

	std::cout << "ORBIT " << orbit::version() << std::endl 
		<< "Copyright (C) 2015 David Jolly" << std::endl << std::endl;

	try {
		inst = orbit::acquire();
		//std::cout << inst->to_string(true) << std::endl;
		inst->initialize();
		
		// TODO
		orbit_socket sock;
		std::cout << sock.to_string(true) << std::endl;
		// ---

		//std::cout << inst->to_string(true) << std::endl;
		inst->uninitialize();
		//std::cout << inst->to_string(true) << std::endl;
	} catch(orbit_exception &exc) {
		std::cerr << exc.what() << std::endl;
		result = -1;
	}

	return result;
}
