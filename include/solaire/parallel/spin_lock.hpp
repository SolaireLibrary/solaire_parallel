#ifndef SOLAIRE_PARALEL_SPIN_LOCK_HPP
#define SOLAIRE_PARALEL_SPIN_LOCK_HPP

//Copyright 2016 Adam G. Smith
//
//Licensed under the Apache License, Version 2.0 (the "License");
//you may not use this file except in compliance with the License.
//You may obtain a copy of the License at
//
//http ://www.apache.org/licenses/LICENSE-2.0
//
//Unless required by applicable law or agreed to in writing, software
//distributed under the License is distributed on an "AS IS" BASIS,
//WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
//See the License for the specific language governing permissions and
//limitations under the License.

#include <atomic>
#include "solaire/core/core.hpp"

namespace solaire {
	class spin_lock {
		std::atomic_flag mFlag;
	public:
		spin_lock();
		~spin_lock();

		void lock();
		void unlock();
		bool try_lock();
	};
}

#endif
