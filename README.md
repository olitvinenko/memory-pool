# memory-pool

objects pool for pooled allocations and pointer type for it

USAGE:

// your_type.hpp
class YOUR_TYPE
{
	DECLARE_POOLED_ALLOCATION(YOUR_TYPE);
	//...
};

// your_type.cpp
IMPLEMENT_POOLED_ALLOCATION(YOUR_TYPE);


some samples could be found in tests/main.cpp