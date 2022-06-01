#ifndef __ATTACKINGTOWERSTATE_H__
#define __ATTACKINGTOWERSTATE_H__

#include "State.h"
#include "GameObject.h"

class AttackingTowerState : public State<GameObject>
{
private:

	AttackingTowerState() {}

	AttackingTowerState(const AttackingTowerState&);
	AttackingTowerState& operator=(const AttackingTowerState&);

public:

	void Enter(std::shared_ptr<GameObject>);
	void Execute(std::shared_ptr<GameObject>, float dt);
	void Exit(std::shared_ptr<GameObject>);
	~AttackingTowerState() {}

	static AttackingTowerState* Instance();
};

#endif