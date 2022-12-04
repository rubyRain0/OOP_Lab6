#include "List.h"


template<typename TInfo>
class NODE
{
public:
	TInfo info;
	NODE* next;
	NODE(TInfo info, NODE* ptr = nullptr) :info(info), next(ptr) {}
	~NODE()
	{
		next = nullptr;
	}
};