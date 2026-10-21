#ifndef _GAME_NAVIPARMS_H
#define _GAME_NAVIPARMS_H

#include "Game/FakePiki.h"

namespace Game {
struct NaviParms : public FakePikiParms {
	struct Parms : public Parameters {
		Parms()
		    : Parameters(nullptr, "Navi::Parms")
		    , mActionRadius(this, 'p000', 25.0f, 0.0f, 500.0f)             // 'action radius'
		    , mAutopluckDistance(this, 'p060', 200.0f, 0.0f, 1000.0f)      // 'continuous pluck distance'
		    , mPikiCallMaxRadius(this, 'p001', 90.0f, 0.0f, 500.0f)        // 'piki call max radius'
		    , mPikiCallMinRadius(this, 'p053', 5.0f, 0.0f, 500.0f)         // 'piki call min radius'
		    , mMaxCallTime(this, 'p002', 0.45f, 0.0f, 500.0f)              // 'max call time'
		    , mCircleDisappearTime(this, 'p003', 1.0f, 0.0f, 500.0f)       // 'circle disappearing time'
		    , mMoveSpeed(this, 'p004', 160.0f, 0.0f, 500.0f)               // 'movement speed'
		    , mRunSpeed(this, 'p056', 160.0f, 0.0f, 500.0f)                // 'running speed (in good condition)' (lol)
		    , mTimeLimitForThrowing(this, 'p009', 2.5f, 0.0f, 10.0f)       // 'time limit for throwing'
		    , mThrowDistanceMax(this, 'p010', 140.0f, 0.0f, 1000.0f)       // 'throw distance (Max)'
		    , mThrowDistanceMin(this, 'p011', 140.0f, 0.0f, 500.0f)        // 'throw distance (Min)'
		    , mThrowHeightMax(this, 'p024', 75.0f, 0.0f, 1000.0f)          // 'throw height (Max)'
		    , mThrowHeightMin(this, 'p025', 75.0f, 0.0f, 1000.0f)          // 'throw height (min)'
		    , mThrowHeightYellow(this, 'p054', 115.0f, 0.0f, 1000.0f)      // 'throw height (yellow)'
		    , mThrowBlackHeight(this, 'q000', 30.0f, 0.0f, 150.0f)         // 'black throw height'
		    , mThrowWhiteHeight(this, 'q001', 75.0f, 0.0f, 150.0f)         // 'white throw height'
		    , mLandingTime(this, 'p026', 1.0f, 0.0f, 100.0f)               // 'landing time'
		    , mGrabPikiRange(this, 'p037', 15.0f, 0.0f, 200.0f)            // 'effective range to grab piki'
		    , mLoopCountToPluck(this, 'p042', 0, 0, 10)                    // 'number of loops to pluck' (?)
		    , mPikiWaitRange(this, 'p039', 15.0f, 0.0f, 200.0f)            // 'piki wait range'
		    , mPikiChangeFormationRange(this, 'p040', 40.0f, 0.0f, 200.0f) // 'range where piki changes formation'
		    , mShakePreventionAngle(this, 'p035', 10.0f, 0.0f, 180.0f)     // 'shake prevention angle'
		    , mCursorLookTime(this, 'p048', 0.85f, 0.0f, 60.0f)            // 'cursor look time'
		    , mPikiLoseNumbnessTime(this, 'p049', 5.0f, 0.0f, 60.0f)       // 'time for piki to lose its numbness' (?)
		    , mNeutralStickThreshold(this, 'p043', 0.1f, 0.0f, 1.0f)       // 'neutral stick'
		    , mCursorMovementStick(this, 'p044', 0.65f, 0.0f, 1.0f)        // 'cursor movement stick'
		    , mClampStick(this, 'p048', 0.85f, 0.0f, 1.0f)                 // 'clamp stick'
		    , mMaxCursorMoveRadius(this, 'p046', 100.0f, 0.0f, 2000.0f)    // 'max cursor movement radius'
		    , mCursorMovementSpeed(this, 'p047', 300.0f, 0.0f, 1000.0f)    // 'cursor movement speed'
		    , mMaxHealth(this, 'p050', 100.0f, 0.0f, 1000.0f)              // 'life'
		    , mRushBootSpeed(this, 'q006', 240.0f, 0.0f, 500.0f)           // 'rush boot speed'
		    , mWideWhistleRadius(this, 'q007', 200.0f, 0.0f, 300.0f)       // 'wide whistle radius'
		    , mShieldDamageReductionRate(this, 'q008', 0.5f, 0.0f, 1.0f)   // 'shield damage reduction rate'
		    , mSeesawWeight(this, 'q009', 2, 0, 10)                        // 'weight for seesaw'
		    , mElectricGateDamage(this, 'q010', 10.0f, 0.0f, 200.0f)       // 'electric gate damage'
		{
		}

		// _27C-_288	= Parameters
		Parm<f32> mActionRadius;              // _288, p000
		Parm<f32> mAutopluckDistance;         // _2B0, p060
		Parm<f32> mPikiCallMaxRadius;         // _300, p001, max whistle radius (pre-upgrade)
		Parm<f32> mPikiCallMinRadius;         // _328, p053, initial whistle radius
		Parm<f32> mMaxCallTime;               // _350, p002
		Parm<f32> mCircleDisappearTime;       // _378, p003
		Parm<f32> mMoveSpeed;                 // _3A0, p004
		Parm<f32> mRunSpeed;                  // _3C8, p056
		Parm<f32> mTimeLimitForThrowing;      // _4E0, p009
		Parm<f32> mThrowDistanceMax;          // _508, p010
		Parm<f32> mThrowDistanceMin;          // _530, p011
		Parm<f32> mThrowHeightMax;            // _558, p024
		Parm<f32> mThrowHeightMin;            // _580, p025
		Parm<f32> mThrowHeightYellow;         // _5A8, p054
		Parm<f32> mThrowBlackHeight;          // _5D0, q000
		Parm<f32> mThrowWhiteHeight;          // _5F8, q001
		Parm<f32> mLandingTime;               // _620, p026
		Parm<f32> mGrabPikiRange;             // _648, p037
		Parm<int> mLoopCountToPluck;          // _670, p042
		Parm<f32> mPikiWaitRange;             // _698, p039
		Parm<f32> mPikiChangeFormationRange;  // _6C0, p040
		Parm<f32> mShakePreventionAngle;      // _828, p035
		Parm<f32> mCursorLookTime;            // _878, p048, yes these are both p048
		Parm<f32> mPikiLoseNumbnessTime;      // _8A0, p049
		Parm<f32> mNeutralStickThreshold;     // _8C8, p043
		Parm<f32> mCursorMovementStick;       // _8F0, p044
		Parm<f32> mClampStick;                // _918, p048, yes these are both p048
		Parm<f32> mMaxCursorMoveRadius;       // _968, p046
		Parm<f32> mCursorMovementSpeed;       // _990, p047
		Parm<f32> mMaxHealth;                 // _9B8, p050
		Parm<f32> mRushBootSpeed;             // _B98, q006
		Parm<f32> mWideWhistleRadius;         // _BC0, q007, max whistle radius (with upgrade)
		Parm<f32> mShieldDamageReductionRate; // _BE8, q008
		Parm<int> mSeesawWeight;              // _C10, q009
		Parm<f32> mElectricGateDamage;        // _C38, q010
		                                      // _C60 = IParameters ptr
	};

	NaviParms()
	    : mNaviParms()
	{
		mCreatureProps.mProps.mFriction.mValue      = 1.0f;
		mCreatureProps.mProps.mFaceDirAdjust.mValue = 0.5f;
		mCreatureProps.mProps.mAccel.mValue         = 0.1f;
	}

	virtual void read(Stream& stream) // _08 (weak)
	{
		CreatureParms::read(stream);
		mFakePikiParms.read(stream);
		mNaviParms.read(stream);
	}

	// _00-_D8  = CreatureParms
	// _D8 		  = VTBL
	// _DC-_27C	= FakePikiParms
	Parms mNaviParms; // _27C
};

} // namespace Game

#endif
