#include "thread_group.hpp"

ThreadGroup::~ThreadGroup()
{
	container::const_iterator const from = std::begin(m_threads);
	container::const_iterator const to = std::end(m_threads);
	for (container::const_iterator it = from; it != to; ++it)
		delete it->second;
}

void ThreadGroup::addThread(std::thread *th)
{
	m_threads.emplace(th->get_id(), th);
}

void ThreadGroup::removeThread(std::thread *th)
{
	container::iterator it = m_threads.find(th->get_id());
	if (it != std::end(m_threads))
		m_threads.erase(it);
}

bool ThreadGroup::isThreadIn(std::thread *th) const
{
	return m_threads.find(th->get_id()) != std::end(m_threads);
}

bool ThreadGroup::isThisThreadIn() const
{
	return m_threads.find(std::this_thread::get_id()) !=
				std::end(m_threads);
}

void ThreadGroup::joinAll()
{
	container::const_iterator const from = std::begin(m_threads);
	container::const_iterator const to = std::end(m_threads);
	for (container::const_iterator it = from; it != to; ++it)
		it->second->join();	
}

void ThreadGroup::detachAll()
{
	container::const_iterator const from = std::begin(m_threads);
	container::const_iterator const to = std::end(m_threads);
	for (container::const_iterator it = from; it != to; ++it)
		it->second->detach();	
}

size_t ThreadGroup::size() const
{
	return m_threads.size();
}
