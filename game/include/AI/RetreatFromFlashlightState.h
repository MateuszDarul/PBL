#ifndef __RETREATFROMFLASHLIGHTSTATE_H__
#define __RETREATFROMFLASHLIGHTSTATE_H__

#include "State.h"
#include "GameObject.h"

class RetreatFromFlashlightState : public State<GameObject>
{
private:

	RetreatFromFlashlightState() {}

	RetreatFromFlashlightState(const RetreatFromFlashlightState&);
	RetreatFromFlashlightState& operator=(const RetreatFromFlashlightState&);

public:

	void Enter(std::shared_ptr<GameObject>);
	void Execute(std::shared_ptr<GameObject>, float dt);
	void Exit(std::shared_ptr<GameObject>);
	~RetreatFromFlashlightState() {}

	static RetreatFromFlashlightState* Instance();
};

#endif