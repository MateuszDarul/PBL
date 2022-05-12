#ifndef __STATE_H__
#define __STATE_H__

#include <memory>

template <class entity_type>
class State
{
protected:

	State() {}

private:

	State(const State&);
	State& operator=(const State &);

public:

	virtual void Enter(std::shared_ptr<entity_type>) = 0;
	virtual void Execute(std::shared_ptr<entity_type>, float dt) = 0;
	virtual void Exit(std::shared_ptr<entity_type>) = 0;
	virtual ~State() {}

	static State* Instance();
};

#endif