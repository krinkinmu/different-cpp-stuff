#ifndef __THREAD_GROUP_HPP__
#define __THREAD_GROUP_HPP__

#include <map>
#include <thread>

class ThreadGroup
{
	using container = std::map<std::thread::id, std::thread *>;
public:
	ThreadGroup(ThreadGroup const &) = delete;
	ThreadGroup &operator=(ThreadGroup const &) = delete;

	~ThreadGroup();

	void addThread(std::thread *th);
	void removeThread(std::thread *th);

	bool isThreadIn(std::thread *th) const;
	bool isThisThreadIn() const;

	void joinAll();
	void detachAll();

	size_t size() const;

	template <typename F, typename ... Args>
	std::thread *createThread(F && f, Args && ... args)
	{
		std::thread *new_one = new std::thread(std::forward<F>(f),
					std::forward<Args>(args)...);
		addThread(new_one);
		return new_one;
	}

private:
	container m_threads;
};

#endif /*__THREAD_GROUP_HPP__*/
