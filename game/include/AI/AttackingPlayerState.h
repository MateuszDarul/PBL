#ifndef __ATTACKINGPLAYERSTATE_H__
#define __ATTACKINGPLAYERSTATE_H__

#include "State.h"
#include "GameObject.h"

class AttackingPlayerState : public State<GameObject>
{
private:

	AttackingPlayerState() {}

	AttackingPlayerState(const AttackingPlayerState&);
	AttackingPlayerState& operator=(const AttackingPlayerState&);

public:

	void Enter(std::shared_ptr<GameObject>);
	void Execute(std::shared_ptr<GameObject>, float dt);
	void Exit(std::shared_ptr<GameObject>);
	~AttackingPlayerState() {}

	static AttackingPlayerState* Instance();
};

#endif