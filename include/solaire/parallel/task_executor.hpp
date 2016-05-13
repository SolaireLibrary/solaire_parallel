#ifndef SOLAIRE_PARALEL_TASK_EXECUTOR_HPP
#define SOLAIRE_PARALEL_TASK_EXECUTOR_HPP

#include <functional>
#include <memory>
#include <future>
#include <chrono>

namespace solaire {

	class task_executor;

	template<class R>
	class task_future {
	public:
		friend task_executor;
	private:
		std::shared_ptr<std::promise<R>> mPromise;

		public:
			task_future(std::shared_ptr<std::promise<R>> aPromise) :
				mPromise(aPromise)
			{}
	public:
		inline R get() {
			return mPromise->get_future().get();
		}

		inline void wait() const {
			mPromise->get_future().get();
		}

		inline bool valid() const {
			return mPromise->get_future().valid();
		}

		template<class REP, class PERIOD>
		std::future_status wait_for(const std::chrono::duration<REP, PERIOD>& aDuration) const {
			return mPromise->get_future().wait_for(aDuration);
		}

		template<class REP, class PERIOD>
		std::future_status wait_until(const std::chrono::duration<REP, PERIOD>& aDuration) const {
			return mPromise->get_future().wait_until(aDuration);
		}
	};

	class task_executor {
	protected:
		virtual void _schedule(std::function<void()>) = 0;
	public:
		virtual ~task_executor() {

		}

		template<class R, class ...PARAMS>
		task_future<void> schedule(std::function<void(PARAMS...)> aTask, PARAMS... aParams) {
			const std::shared_ptr<std::promise<void>> promise(new std::promise<void>());
			const std::function<void()> task = [=]() {
				try {
					aTask(aParams...);
					promise->set_value();
				}
				catch (std::exception& e) {
					promise->set_exception(std::current_exception());
				}
			};

			_schedule(task);

			return task_future<R>(promise);
		}

		template<class R, class ...PARAMS>
		task_future<R> schedule(std::function<R(PARAMS...)> aTask, PARAMS... aParams) {
			const std::shared_ptr<std::promise<R>> promise(new std::promise<R>());
			const std::function<void()> task = [=]() {
				try {
					promise->set_value(aTask(aParams...));
				} catch (std::exception& e) {
					promise->set_exception(std::current_exception());
				}
			};

			_schedule(task);

			return task_future<R>(promise);
		}
	};
}

#endif#pragma once
