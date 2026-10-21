#ifndef _GAME_PIKIPARMS_H
#define _GAME_PIKIPARMS_H

#include "Game/FakePiki.h"

namespace Game {
struct PikiParms : public FakePikiParms {
	struct Parms : public Parameters {
		Parms()
		    : Parameters(nullptr, "Piki::Parms")
		    , mWalkSpeed(this, 'p000', 84.0f, 0.0f, 500.0f)                  // 'walking speed'
		    , mRunSpeed(this, 'p001', 120.0f, 0.0f, 500.0f)                  // 'running speed'
		    , mFlowerRunSpeed(this, 'p054', 170.0f, 0.0f, 500.0f)            // 'flower pikmin speed'
		    , mBudRunSpeed(this, 'p065', 140.0f, 0.0f, 500.0f)               // 'bud speed'
		    , mPressedScale(this, 'p038', 1.0f, 0.1f, 10.0f)                 // 'piki display scale'
		    , mBlueAttackDamage(this, 'p004', 10.0f, 0.0f, 1000.0f)          // 'blue attack power'
		    , mRedAttackDamage(this, 'p012', 15.0f, 0.0f, 1000.0f)           // 'red attack power'
		    , mYellowAttackDamage(this, 'p013', 10.0f, 0.0f, 1000.0f)        // 'yellow attack power'
		    , mPurpleAttackDamage(this, 'P003', 10.0f, 1.0f, 30.0f)          // 'black attack power'
		    , mWhiteAttackDamage(this, 'P004', 1.0f, 1.0f, 30.0f)            // 'white attack power'
		    , mScale(this, 'p007', 0.8f, 0.0f, 2.0f)                         // 'normal piki scale'
		    , mWhiteScale(this, 'p008', 1.5f, 0.0f, 2.0f)                    // 'tall piki scale'
		    , mPurpleScale(this, 'p009', 1.2f, 0.0f, 2.0f)                   // 'fatty piki scale'
		    , mWalkAnimSpeed(this, 'p018', 35.0f, 0.0f, 500.0f)              // 'WALK start speed'
		    , mWhiteDistance(this, 'p029', 600.0f, 0.0f, 10000.0f)           // 'white distance'
		    , mGrayDistance(this, 'p030', 1000.0f, 0.0f, 10000.0f)           // 'gray distance'
		    , mLostChildTime(this, 'p031', 10.0f, 0.0f, 1000.0f)             // 'lost child time'
		    , mEnemySearchRange(this, 'p041', 60.0f, 1.0f, 1000.0f)          // 'enemy search area'
		    , mPelletSearchRange(this, 'p042', 30.0f, 1.0f, 1000.0f)         // 'pellet search area'
		    , mNectarRockSearchRange(this, 'P010', 30.0f, 1.0f, 1000.0f)     // 'agricultural rock search area' (lmao)
		    , mGateSearchRange(this, 'P011', 40.0f, 1.0f, 1000.0f)           // 'agricultural rock search area' (should refer to gates)
		    , mBridgeSearchRange(this, 'P012', 100.0f, 1.0f, 1000.0f)        // 'bridge search area'
		    , mClogSearchRange(this, 'P013', 30.0f, 1.0f, 1000.0f)           // 'breaking rock search area'
		    , mBuriedTreasureSearchRange(this, 'P014', 30.0f, 1.0f, 1000.0f) // 'bargain search area' (lmao??)
		    , mSpiderWortSearchRange(this, 'P015', 30.0f, 1.0f, 1000.0f)     // 'agricultural plant search area'
		    , mFlowerPikiGravity(this, 'p048', 0.01f, 0.0f, 1.0f)            // 'flower piki gravity rate'
		    , mBuriedSeedWaitTime(this, 'p049', 1.0f, 0.0f, 300.0f)          // 'taneupTime (seconds)' (seed?)
		    , mUnpluckableTime(this, 'p050', 1.0f, 0.0f, 300.0f)             // 'hatugaTime (seconds)' (bud?)
		    , mGrowUpToFlowerTime(this, 'p051', 4.0f, 0.0f, 300.0f)          // 'growupTime (seconds)'
		    , mWitherFromFlowerTime(this, 'p052', 4.0f, 0.0f, 300.0f)        // 'kareTime (seconds)' (he? boyfriend?)
		    , mWitheredHideTime(this, 'p053', 1.0f, 0.0f, 300.0f)            // 'rebirthTime (seconds)'
		    , mPanicMaxTime(this, 'p055', 3.0f, 0.0f, 30.0f)                 // 'time until death in fire'
		    , mWhiteRunSpeedMultiplier(this, 'P001', 2.0f, 1.0f, 3.0f)       // 'white speed multiplier'
		    , mPurpleRunSpeedMultiplier(this, 'P002', 0.6f, 0.1f, 3.0f)      // 'black speed multiplier'
		    , mPurpleCarryPower(this, 'P005', 0.5f, 0.0f, 5.0f)              // 'black carrying capacity'
		    , mWhiteCarryPower(this, 'P006', 1.5f, 0.0f, 5.0f)               // 'white carrying capacity'
		    , mBudCarrySpeedBonus(this, 'P020', 0.5f, 0.0f, 2.0f)            // 'bud | carrying bonus'
		    , mFlowerCarrySpeedBonus(this, 'P021', 1.0f, 0.0f, 2.0f)         // 'flower | carrying bonus'
		    , mCarryMaxFactor(this, 'P018', 0.7f, 0.0f, 2.0f)                // 'carrying MAX FACTOR'
		    , mCarryMinFactor(this, 'P019', 0.35f, 0.0f, 2.0f)               // 'carrying MAX FACTOR' (mistake?)
		    , mDopeMaxDuration(this, 'P007', 30.0f, 0.0f, 200.0f)            // 'doping duration'
		    , mDopeAttackDamage(this, 'P008', 25.0f, 0.0f, 100.0f)           // 'doping piki attack'
		    , mDopeRunSpeed(this, 'P009', 190.0f, 0.0f, 220.0f)              // 'doping piki speed'
		    , mKurageKillTime(this, 'P016', 16.0f, 0.0f, 120.0f)             // 'jellyfish digestion time'
		    , mPoundAOERange(this, 'P017', 60.0f, 0.0f, 1000.0f)             // 'doshin radius' (pound radius?)
		    , mPoundDamage(this, 'P022', 100.0f, 0.0f, 1000.0f)              // 'black hip attack damage'
		{
		}

		// _27C-_288	= Parameters
		Parm<f32> mWalkSpeed;                 // _288, p000, 'initial walk speed'; affects things like dismiss speed and suffering run speed
		Parm<f32> mRunSpeed;                  // _2B0, p001, normal movement speed
		Parm<f32> mFlowerRunSpeed;            // _2D8, p054
		Parm<f32> mBudRunSpeed;               // _300, p065
		Parm<f32> mPressedScale;              // _328, p038, scale for when crushed
		Parm<f32> mBlueAttackDamage;          // _3C8, p004, blue pikmin damage (also default damage)
		Parm<f32> mRedAttackDamage;           // _3F0, p012, red pikmin damage + vs mode damage
		Parm<f32> mYellowAttackDamage;        // _418, p013, yellow pikmin damage
		Parm<f32> mPurpleAttackDamage;        // _440, P003, purple pikmin damage
		Parm<f32> mWhiteAttackDamage;         // _468, P004, white pikmin damage
		Parm<f32> mScale;                     // _4E0, p007, default pikmin model scale
		Parm<f32> mWhiteScale;                // _508, p008, white pikmin model scale
		Parm<f32> mPurpleScale;               // _530, p009, purple pikmin model scale
		Parm<f32> mWalkAnimSpeed;             // _620, p018
		Parm<f32> mWhiteDistance;             // _828, p029
		Parm<f32> mGrayDistance;              // _850, p030
		Parm<f32> mLostChildTime;             // _878, p031
		Parm<f32> mEnemySearchRange;          // _968, p041, search ranges when idle
		Parm<f32> mPelletSearchRange;         // _990, p042
		Parm<f32> mNectarRockSearchRange;     // _9B8, P010,
		Parm<f32> mGateSearchRange;           // _9E0, P011, duplicate of above?
		Parm<f32> mBridgeSearchRange;         // _A08, P012
		Parm<f32> mClogSearchRange;           // _A30, P013
		Parm<f32> mBuriedTreasureSearchRange; // _A58, P014
		Parm<f32> mSpiderWortSearchRange;     // _A80, P015
		Parm<f32> mFlowerPikiGravity;         // _B98, p048, gravity for flower pikmin when thrown
		Parm<f32> mBuriedSeedWaitTime;        // _BC0, p049, time seed is hidden on hitting the ground (pikihead burystate)
		Parm<f32> mUnpluckableTime;           // _BE8, p050, time withered seed is small/unpluckable (piki tanestate)
		Parm<f32> mGrowUpToFlowerTime;        // _C10, p051, time seed takes to upgrade a stage (pikihead waitstate)
		Parm<f32> mWitherFromFlowerTime;      // _C38, p052, time seed takes to wither/downgrade to leaf (pikihead waitstate)
		Parm<f32> mWitheredHideTime;          // _C60, p053, time seed hides underground after withering (pikihead SioreState)
		Parm<f32> mPanicMaxTime;              // _C88, p055
		Parm<f32> mWhiteRunSpeedMultiplier;   // _FF8, P001, multiplier to speed values for white pikmin
		Parm<f32> mPurpleRunSpeedMultiplier;  // _1020, P002, multiplier to speed values for purple pikmin
		Parm<f32> mPurpleCarryPower;          // _1048, P005, carry strength for purples
		Parm<f32> mWhiteCarryPower;           // _1070, P006, carry strength for whites
		Parm<f32> mBudCarrySpeedBonus;        // _1098, P020, speed bonus for bud pikmin carrying stuff
		Parm<f32> mFlowerCarrySpeedBonus;     // _10C0, P021, speed bonus for flower pikmin carrying stuff
		Parm<f32> mCarryMaxFactor;            // _10E8, P018, "(transport) MAX FACTOR" unknown
		Parm<f32> mCarryMinFactor;            // _1110, P019, "(transport) MAX FACTOR" unknown
		Parm<f32> mDopeMaxDuration;           // _1138, P007, max duration of spicy spray
		Parm<f32> mDopeAttackDamage;          // _1160, P008, attack damage for spicied pikmin
		Parm<f32> mDopeRunSpeed;              // _1188, P009, run speed for spicied pikmin
		Parm<f32> mKurageKillTime;            // _11B0, P016, time for jellyfloats to kill pikmin (idk why this is here)
		Parm<f32> mPoundAOERange;             // _11D8, P017, radius for purple pikmin ground pound AOE
		Parm<f32> mPoundDamage;               // _1200, P022
		                                      // _1228 = IParameters ptr
	};

	PikiParms()
	    : mPikiParms()
	{
		mCreatureProps.mProps.mFriction.mValue      = 0.1f;
		mCreatureProps.mProps.mFaceDirAdjust.mValue = 0.8f;
		mCreatureProps.mProps.mAccel.mValue         = 0.14f;
	}

	virtual void read(Stream& stream) // _08 (weak)
	{
		CreatureParms::read(stream);
		mFakePikiParms.read(stream);
		mPikiParms.read(stream);
	}

	// _00-_D8  = CreatureParms
	// _D8 		  = VTBL
	// _DC-_27C	= FakePikiParms
	Parms mPikiParms; // _27C
};

} // namespace Game

#endif
