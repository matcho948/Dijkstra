#include "mem-leak-config.h"

#ifdef TURN_ON_MEM_LEAK_DETECTION

#include <unordered_map>
#include <string>

namespace
{	
	//Co by zmusic kontenery uzywane do kontrolowania wyciewko powodowanych przez new do uzywania malloc/free
	template <typename T>
	class MallocAllocator {
	public:
		using value_type = T;
		using size_type = size_t;
		using difference_type = std::ptrdiff_t;
		using propagate_on_container_move_assignment = std::true_type;
		using is_always_equal = std::true_type;
	
		constexpr MallocAllocator() noexcept {}

		constexpr MallocAllocator(const MallocAllocator&) noexcept = default;
		
		template <class U>
		constexpr MallocAllocator(const MallocAllocator<U>&) noexcept { }

		void deallocate(T* const ptr, const size_t count)
		{			
			free(ptr);
		}

		T* allocate(const size_t count)
		{
			auto mem = malloc(count * sizeof(T));
			if (!mem)
				throw std::bad_alloc{};
			return static_cast<T*>(mem);
		}
	};

	struct MemDescItem
	{		
		size_t size;
		using MallocString = std::basic_string<char, std::char_traits<char>, MallocAllocator<char>>;
		MallocString file;
		uint32_t line;

		MemDescItem(size_t size, const char* file, uint32_t line) : size(size), file(file), line(line) {}
	};

	class MemManager		
	{	
		std::unordered_map<void*, MemDescItem, std::hash<void*>, std::equal_to<void*>, MallocAllocator<std::pair<void*, MemDescItem>>> data;
	public:
		static MemManager& GetInst()
		{
			static MemManager inst;
			return inst;
		}

		void LogAloc(void* ptr, const MemDescItem& item)
		{						
			data.emplace(ptr, item);
		}

		void LogDealloc(void* ptr)
		{			
			auto iter = data.find(ptr);
			if (iter == data.end())
				printf("Wrong deallocation!\n");
			else
				data.erase(iter);
		}

		compl MemManager() //destructor (compl = ~) 
		{
			if (data.size())
			{
				printf("A co to za wycieki, nieladnie tak!\n");
				for (const auto& e : data)									
					printf("adres: %p, rozmiar: %luB, zaalokowano w pliku(linii): %s(%u)\n", e.first, e.second.size,  e.second.file.c_str(), e.second.line);
				
			}
		}
	};	
}

void* operator new(size_t size, const char* file, uint32_t line)
{
	auto mem = malloc(size);
	if (!mem)
		throw std::bad_alloc{};
	MemManager::GetInst().LogAloc(mem, { size, file, line });	
	return mem;
}


void* malloc(size_t size, const char* file, uint32_t line)
{
	auto mem = malloc(size);
	if (mem)
		MemManager::GetInst().LogAloc(mem, { size, file, line });
	return mem;
}

void* calloc(size_t num, size_t size, const char* file, uint32_t line)
{
	auto mem = calloc(num, size);
	if (mem)
		MemManager::GetInst().LogAloc(mem, { size, file, line });
	return mem;
}

void* realloc(void* ptr, size_t size, const char* file, uint32_t line)
{
	if (!ptr)
		return malloc(size, file, line);
	auto mem = realloc(ptr, size);
	if (mem != ptr) // deallocation occured
	{
		MemManager::GetInst().LogDealloc(ptr);
		if(mem)
			MemManager::GetInst().LogAloc(mem, { size, file, line });
	}
	return mem;
}

void free(void* ptr, int /*just to be able to overload function*/)
{
	if (!ptr)
		return;
	MemManager::GetInst().LogDealloc({ ptr });
	free(ptr);
}

void* operator new[](size_t size)
{
	return operator new(size);
}


void* operator new[](size_t size, const char* file, uint32_t line) 
{
	return operator new(size, file, line);
}

void operator delete(void* ptr)
{
	if (!ptr)
		return;
	MemManager::GetInst().LogDealloc({ ptr });
	free(ptr);
}

void* operator new(size_t size)
{
	return operator new(size, "<nieznany plik>", 0);
}

void operator delete[](void* ptr)
{
	operator delete(ptr);
}

#endif