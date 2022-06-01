#ifndef __TAKEDAMAGESTATE_H__
#define __TAKEDAMAGESTATE_H__

#include "State.h"
#include "GameObject.h"

class TakeDamageState : public State<GameObject>
{
private:

	TakeDamageState() {}

	TakeDamageState(const TakeDamageState&);
	TakeDamageState& operator=(const TakeDamageState&);

public:

	void Enter(std::shared_ptr<GameObject>);
	void Execute(std::shared_ptr<GameObject>, float dt);
	void Exit(std::shared_ptr<GameObject>);
	~TakeDamageState() {}

	static TakeDamageState* Instance();
};

#endif