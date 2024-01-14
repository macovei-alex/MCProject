#pragma once

#include <string>
#include <vector>
#include <chrono>
#include <algorithm>
#include <mutex>

#define LOCK(mutex) std::lock_guard<std::mutex> lock(mutex)
#define LOCK_2(mutex1, mutex2) \
	std::lock_guard<std::mutex> lock1(mutex1); \
	std::lock_guard<std::mutex> lock2(mutex2)

namespace utils
{
	__declspec(dllexport) std::string GetHashSHA256(const std::string& input);
	__declspec(dllexport) std::vector<std::string> SplitToVector(const std::string& str, const std::string& delim);
	__declspec(dllexport) std::pair<std::string, std::string> SplitToPair(const std::string& str, const std::string& delim);
	__declspec(dllexport) uint64_t MillisFromDateTime(std::chrono::system_clock::time_point dateTime = std::chrono::system_clock::now());
	__declspec(dllexport) std::chrono::system_clock::time_point DateTimeFromMillis(uint64_t millis);

	template<class T> class __declspec(dllexport) ThreadSafe
	{
	public:
		ThreadSafe() noexcept = default;
		ThreadSafe(const T& obj) noexcept : obj(obj) {}
		ThreadSafe(T&& obj) noexcept : obj(std::move(obj)) {}

		ThreadSafe(const ThreadSafe&) = delete;
		ThreadSafe& operator=(const ThreadSafe&) = delete;

		ThreadSafe(ThreadSafe&& other) noexcept
		{
			if (&other == this)
				return;

			LOCK_2(other.mutex, mutex);
			obj = std::move(other.obj);
		}

		ThreadSafe& operator=(ThreadSafe&& other) noexcept
		{
			if (&other == this)
				return *this;

			LOCK_2(other.mutex, mutex);
			obj = std::move(other.obj);
			return *this;
		}

		~ThreadSafe() noexcept = default;

	public:
		T Get() const noexcept
		{
			LOCK(mutex);
			return obj;
		}

		/*
		This function lets the user manage the object more freely, but at the cost of compromising thread safety.
		If you want to use this function, make sure there are no concurrent threads accessing the object, or that
		you lock the object manually after receiving it
		*/
		T& GetRef() noexcept
		{
			return obj;
		}

		void Set(const ThreadSafe& other) noexcept
		{
			LOCK(mutex);
			obj = other.Get();
		}

		void Set(ThreadSafe&& other) noexcept
		{
			LOCK(mutex);
			obj = std::move(other.Get());
		}

		void Set(const T& obj) noexcept
		{
			LOCK(mutex);
			this->obj = obj;
		}

		void Set(T&& obj) noexcept
		{
			LOCK(mutex);
			this->obj = std::move(obj);
		}

		/*
		Function made to ensure better control over the stored value in cases of high efficiency needs
		(multiple uses of the value in a short amount of time)
		*/
		std::mutex& GetMutex() const noexcept
		{
			return mutex;
		}

	private:
		T obj;
		mutable std::mutex mutex;
	};
}

#undef LOCK
