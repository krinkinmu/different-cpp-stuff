#include <algorithm>
#include <iostream>
#include <iomanip>
#include <thread>
#include <chrono>

namespace stuff
{

	template <typename Iter, typename Rt, typename Acc>
	void accumulate_block(Iter first, Iter last, Rt & res, Acc op)
	{
		res = std::accumulate(first, last, res, op);
	}

	template <typename Iter, typename Rt, typename Acc, size_t MIN = 25>
	Rt parallel_accumulate(Iter first, Iter last, Rt init, Acc op)
	{
		size_t const length = std::distance(first, last);
		if (!length) return init;
		size_t const max_threads = (length + MIN - 1) / MIN;
		size_t const hardware_threads = std::thread::hardware_concurrency();
		size_t const num_threads = std::min(hardware_threads != 0 ? hardware_threads : 2u, max_threads);

		size_t const block_size = length / num_threads;

		std::vector<Rt> results(num_threads);
		std::vector<std::thread> threads(num_threads - 1);

		Iter block_first = first;
		for (size_t it = 0; it != num_threads - 1; ++it)
		{
			Iter block_last = block_first;
			std::advance(block_last, block_size);
			threads[it] = std::thread(
					&accumulate_block<Iter, Rt, Acc>,
					block_first,
					block_last,
					std::ref(results[it]),
					op
				);
			block_first = block_last;
		}
		accumulate_block(block_first, last, results.back(), op);
		std::for_each(threads.begin(), threads.end(), std::mem_fn(&std::thread::join));
		return std::accumulate(results.begin(), results.end(), init);
	}

}

int main()
{
	unsigned long long const last = 100000;
	std::vector<size_t> values;

	for (size_t it = 0; it != last; ++it)
		values.push_back(it);

	unsigned long long const expected = last * (last - 1) / 2;

	
	std::chrono::high_resolution_clock::time_point const simple_begin = std::chrono::high_resolution_clock::now();
	unsigned long long const sum = std::accumulate(values.begin(), values.end(), 0ull, std::plus<unsigned long long>());
	std::chrono::high_resolution_clock::time_point const simple_end = std::chrono::high_resolution_clock::now();
	unsigned long long const parallel_sum = stuff::parallel_accumulate(values.begin(), values.end(), 0ull, std::plus<unsigned long long>());
	std::chrono::high_resolution_clock::time_point const parallel_end = std::chrono::high_resolution_clock::now();

	std::cout << std::boolalpha;
	std::cout << (expected == parallel_sum) << std::endl;
	std::cout << (sum == parallel_sum) << std::endl;

	std::cout << "accumulate time: " << (simple_end - simple_begin).count() << std::endl;
	std::cout << "parallel time: " << (parallel_end - simple_end).count() << std::endl;

	return 0;
}
