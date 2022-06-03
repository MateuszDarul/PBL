#ifndef __IDLESTATE_H__
#define __IDLESTATE_H__

#include "State.h"
#include "GameObject.h"

class IdleState : public State<GameObject>
{
private:

	IdleState() {}

	IdleState(const IdleState&);
	IdleState& operator=(const IdleState&);

public:

	void Enter(std::shared_ptr<GameObject>);
	void Execute(std::shared_ptr<GameObject>, float dt);
	void Exit(std::shared_ptr<GameObject>);
	~IdleState() {}

	static IdleState* Instance();
};

#endif