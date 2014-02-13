#ifndef HANDYSTATS_JSON_ALLOCATORS_H_
#define HANDYSTATS_JSON_ALLOCATORS_H_

#include <handystats/rapidjson/allocators.h>

namespace handystats { namespace json {

extern rapidjson::CrtAllocator crtAllocator;
extern rapidjson::MemoryPoolAllocator<> memoryPoolAllocator;

}} // namespace handystats::json

#endif // HANDYSTATS_JSON_ALLOCATORS_H_
