#ifndef __STATE_H__
#define __STATE_H__

template <class entity_type>

class State
{
private:

	State() {}

	State(const State&);
	State& operator=(const State &);

public:

	virtual void Enter(entity_type*) = 0;
	virtual void Execute(entity_type*) = 0;
	virtual void Exit(entity_type*) = 0;
	virtual ~State() {}

	static State* Instance();
};

#endif