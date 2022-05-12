#ifndef __FOLLOWPATHSTATE_H__
#define __FOLLOWPATHSTATE_H__

#include "State.h"
#include "GameObject.h"

class FollowPathState : public State<GameObject>
{
private:

	FollowPathState() {}

	FollowPathState(const FollowPathState&);
	FollowPathState& operator=(const FollowPathState&);

public:

	void Enter(std::shared_ptr<GameObject>);
	void Execute(std::shared_ptr<GameObject>, float dt);
	void Exit(std::shared_ptr<GameObject>);
	~FollowPathState() {}

	static FollowPathState* Instance();
};

#endif