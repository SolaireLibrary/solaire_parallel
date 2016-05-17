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

#include "solaire/parallel/spin_lock.hpp"

namespace solaire {
	// spin_lock
	spin_lock::spin_lock() :
		mFlag()
	{
		mFlag.clear();
	}

	spin_lock::~spin_lock() {
		runtime_assert(! mFlag.test_and_set(std::memory_order_acquire), "solaire::spin_lock : Lock must be released before it is destroyed");
	}

	void spin_lock::lock() {
		while(mFlag.test_and_set(std::memory_order_acquire)){;}
	}

	void spin_lock::unlock() {
		mFlag.clear(std::memory_order_release);
	}

	bool spin_lock::try_lock() {
		return ! mFlag.test_and_set(std::memory_order_acquire);
	}
}