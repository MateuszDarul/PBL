#include <State.h>

template <class entity_type>

State<entity_type>* State<entity_type>::Instance()
{
	static State instance;

	return &instance;
}