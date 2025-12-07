#pragma once
#ifndef CLAW_STATE_H
#define CLAW_STATE_H

enum class ClawState {
	Idle,
	Dropping,
	Retracting,
	CarryingToy,
	Off
};

#endif // !CLAW_STATE_H
