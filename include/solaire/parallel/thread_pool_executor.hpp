#ifndef SOLAIRE_PARALEL_THREAD_POOL_EXECUTOR_HPP
#define SOLAIRE_PARALEL_THREAD_POOL_EXECUTOR_HPP

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

#include <vector>
#include "solaire/parallel/task_executor.hpp"

namespace solaire {

	class thread_pool_executor : public task_executor {
	private:
		std::thread* const mWorkerThreads;
		std::vector<std::function<void()>> mTasks;
		std::mutex mTasksLock;
		std::condition_variable mTaskAdded;
		const uint32_t mThreadCount;
		bool mExit;
	protected:
		// inherited from task_executor

		void _schedule(std::function<void()> aTask) override {
			mTasksLock.lock();
			mTasks.push_back(aTask);
			mTasksLock.unlock();
			mTaskAdded.notify_one();
		}
	public:
		thread_pool_executor(uint32_t aThreadCount) :
			mWorkerThreads(new std::thread[aThreadCount]),
			mThreadCount(aThreadCount),
			mExit(false)
		{
			const auto threadFn = [this]() {
				while(! mExit) {
					std::unique_lock<std::mutex> lock(mTasksLock);
					mTaskAdded.wait(lock);
					if(! mTasks.empty()) {
						std::function<void()> task = mTasks.back();
						mTasks.pop_back();
						lock.unlock();
						task();
					}
				}
			};

			for(uint32_t i = 0; i < aThreadCount; ++i) mWorkerThreads[i] = std::thread(threadFn);

		}

		~thread_pool_executor() {
			mExit = true;
			mTaskAdded.notify_all();
			for (uint32_t i = 0; i < mThreadCount; ++i) mWorkerThreads[i].join();
			delete[] mWorkerThreads;
		}
	};
}

#endif
