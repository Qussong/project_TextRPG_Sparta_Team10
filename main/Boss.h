#pragma once
#include "Monster.h"

enum EBossCondition
{
	INTRO,
	HALF,
	ALMOST,
	DEATH,
};

class Boss : public Monster {
public:
	Boss(int level);

	inline std::string mGetName() const override { return name; }
	inline int mGetMaxHealth() const override { return maxHealth; }
	inline int mGetHealth() const override { return health; }
	inline int mGetAttack() const override { return attack; }
	inline int mGetExp() const override { return exp; }
	inline int mGetLevel() const override { return level; }

	void mTakeDamage(int damage);
	void SoundEffect(EBossCondition condition);

	bool GetHalfAlarm() { return bHalfAlarm; }
	bool GetAlmostAlarm() { return bAlmostAlarm; }

private:
	bool bHalfAlarm = true;
	bool bAlmostAlarm = true;
};