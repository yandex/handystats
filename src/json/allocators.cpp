#include <handystats/json/allocators.hpp>

namespace handystats { namespace json {

rapidjson::CrtAllocator crtAllocator;
rapidjson::MemoryPoolAllocator<> memoryPoolAllocator;

}} // namespace handystats::json

