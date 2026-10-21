#ifndef _GAME_CAMERAMGR_H
#define _GAME_CAMERAMGR_H

#include "CNode.h"
#include "BaseParm.h"
#include "Parameters.h"
#include "IDelegate.h"
#include "Vector3.h"
#include "Camera.h"

struct Viewport;

namespace Game {
struct Navi;

enum DemoCameraType {
	CAMDEMO_NearLow = 0,
	CAMDEMO_Test    = 1, // never used but has code
};

enum CamNaviID {
	CAMNAVI_Olimar = 0,
	CAMNAVI_Louie  = 1,
	CAMNAVI_Both   = 2,
};

enum CamSelAngle {
	CAMANGLE_Behind   = 0, // low angle (behind navi)
	CAMANGLE_Overhead = 1, // high angle (overhead)
};

enum CamZoomLevel {
	CAMZOOM_Near = 0, // fully zoomed in
	CAMZOOM_Mid  = 1, // middle zoom
	CAMZOOM_Far  = 2, // fully zoomed out
};

enum CameraFlags {
	CAMFLAGS_ChangeZoomLevel = 0x1,  // i.e. R has been pressed
	CAMFLAGS_ChangeSelAngle  = 0x2,  // i.e. Z has been pressed
	CAMFLAGS_CenterBehind    = 0x4,  // i.e. L has been pressed
	CAMFLAGS_SmoothFollow    = 0x8,  // i.e. L is being held
	CAMFLAGS_StartZoomCam    = 0x10, // start zoom cam (held R/starting demo)
	CAMFLAGS_InZoomCam       = 0x20, // continue zoom cam (holding R/in demo)
	CAMFLAGS_EndZoomCam      = 0x40, // end zoom cam (stopped holding R/ended demo)
};

enum CamChangePlayer {
	CAMCHANGE_None       = 0,
	CAMCHANGE_IsChanging = 1,
};

// StrengthSpeedDuration
enum VibrationType {
	//////// Strength LIGHT ////////
	// Speed SLOW
	VIBTYPE_LightSlowShort = 0, // duration short (light, slow)
	VIBTYPE_LightSlowMid   = 1, // duration mid   (light, slow)
	VIBTYPE_LightSlowLong  = 2, // duration long  (light, slow)

	// Speed MID
	VIBTYPE_LightMidShort = 3, // duration short (light, mid)
	VIBTYPE_LightMidMid   = 4, // duration mid   (light, mid)
	VIBTYPE_LightMidLong  = 5, // duration long  (light, mid)

	// Speed FAST
	VIBTYPE_LightFastShort = 6,                     // duration short (light, fast)
	VIBTYPE_LightFastMid   = 7,                     // duration mid   (light, fast)
	VIBTYPE_LightFastLong  = 8,                     // duration long  (light, fast)
	VIBTYPE_LIGHT          = VIBTYPE_LightFastLong, // cutoff for light vibration

	//////// Strength MID ////////
	// Speed SLOW
	VIBTYPE_MidSlowShort = 9,  // duration short (mid, slow)
	VIBTYPE_MidSlowMid   = 10, // duration mid   (mid, slow)
	VIBTYPE_MidSlowLong  = 11, // duration long  (mid, slow)

	// Speed MID
	VIBTYPE_MidMidShort = 12, // duration short (mid, mid)
	VIBTYPE_MidMidMid   = 13, // duration mid   (mid, mid)
	VIBTYPE_MidMidLong  = 14, // duration long  (mid, mid)

	// Speed FAST
	VIBTYPE_MidFastShort = 15,                  // duration short (mid, fast)
	VIBTYPE_MidFastMid   = 16,                  // duration mid   (mid, fast)
	VIBTYPE_MidFastLong  = 17,                  // duration long  (mid, fast)
	VIBTYPE_MID          = VIBTYPE_MidFastLong, // cutoff for light vibration

	//////// Strength HARD ////////
	// Speed SLOW
	VIBTYPE_HardSlowShort = 18, // duration short (hard, slow)
	VIBTYPE_HardSlowMid   = 19, // duration mid   (hard, slow)
	VIBTYPE_HardSlowLong  = 20, // duration long  (hard, slow)

	// Speed MID
	VIBTYPE_HardMidShort = 21, // duration short (hard, mid)
	VIBTYPE_HardMidMid   = 22, // duration mid   (hard, mid)
	VIBTYPE_HardMidLong  = 23, // duration long  (hard, mid)

	// Speed FAST
	VIBTYPE_HardFastShort = 24,                   // duration short (hard, fast)
	VIBTYPE_HardFastMid   = 25,                   // duration mid   (hard, fast)
	VIBTYPE_HardFastLong  = 26,                   // duration long  (hard, fast)
	VIBTYPE_HARD          = VIBTYPE_HardFastLong, // cutoff for light vibration

	// special set vibration parameters
	VIBTYPE_Crash      = 27, // 'crash' effects (rocks hitting the ground, crawbster falling/crashing, empress crashing)
	VIBTYPE_Boom       = 28, // for mum flicks and groink deaths?
	VIBTYPE_NaviDamage = 29, // just for navis taking damage
};

struct CameraArg {
	u32 mState; // _00
};

struct CameraData {
	f32 mTargetDistance;      // _00
	f32 mCameraAngle;         // _04
	f32 mVerticalAngle;       // _08
	f32 mFieldofView;         // _0C
	f32 mNearZ;               // _10
	f32 mFarZ;                // _14
	Vector3f mLookAtPosition; // _18
	int mZoomLevel;           // _24
	int mSelAngle;            // _28
};

/**
 * @size{0x998}
 */
struct CameraParms : public Parameters {
	CameraParms()
	    : Parameters(nullptr, "CameraParms")
	    , mNearLowDist(this, 'cnld', 600.0f, 10.0f, 5000.0f)     // 'near(low) distance'
	    , mNearLowAngle(this, 'cnla', 20.0f, 0.0f, 90.0f)        // 'near(low) angle'
	    , mNearLowFOV(this, 'cnlf', 10.0f, 1.0f, 60.0f)          // 'near(low) fov'
	    , mNearLowOffset(this, 'cnlo', 25.0f, 10.0f, 500.0f)     // 'near(low) offset'
	    , mNearLowWeight(this, 'cnlw', 1000.0f, 10.0f, 5000.0f)  // 'near(low) weight'
	    , mNearLowDetached(this, 'nldt', 17.5f, 0.0f, 1000.0f)   // 'near(low) detached'
	    , mNearLowNear(this, 'nlnc', 1.0f, 1.0f, 12800.0f)       // 'near(low) near'
	    , mNearLowFar(this, 'nlfc', 12800.0f, 1.0f, 12800.0f)    // 'near(low) far'
	    , mMidLowDist(this, 'cmld', 600.0f, 10.0f, 5000.0f)      // 'mid(low) distance'
	    , mMidLowAngle(this, 'cmla', 21.0f, 0.0f, 90.0f)         // 'mid(low) angle'
	    , mMidLowFOV(this, 'cmlf', 23.0f, 1.0f, 60.0f)           // 'mid(low) fov'
	    , mMidLowOffset(this, 'cmlo', 38.0f, 10.0f, 500.0f)      // 'mid(low) offset'
	    , mMidLowWeight(this, 'cmlw', 500.0f, 10.0f, 5000.0f)    // 'mid(low) weight'
	    , mMidLowDetached(this, 'mldt', 55.0f, 0.0f, 1000.0f)    // 'mid(low) detached'
	    , mMidLowNear(this, 'mlnc', 1.0f, 1.0f, 12800.0f)        // 'mid(low) near'
	    , mMidLowFar(this, 'mlfc', 12800.0f, 1.0f, 12800.0f)     // 'mid(low) far'
	    , mFarLowDist(this, 'cfld', 900.0f, 10.0f, 5000.0f)      // 'far(low) distance'
	    , mFarLowAngle(this, 'cfla', 25.0f, 0.0f, 90.0f)         // 'far(low) angle'
	    , mFarLowFOV(this, 'cflf', 26.0f, 1.0f, 60.0f)           // 'far(low) fov'
	    , mFarLowOffset(this, 'cflo', 33.0f, 10.0f, 500.0f)      // 'far(low) offset'
	    , mFarLowWeight(this, 'cflw', 400.0f, 10.0f, 5000.0f)    // 'far(low) weight'
	    , mFarLowDetached(this, 'fldt', 130.0f, 0.0f, 1000.0f)   // 'far(low) detached'
	    , mFarLowNear(this, 'flnc', 1.0f, 1.0f, 12800.0f)        // 'far(low) near'
	    , mFarLowFar(this, 'flfc', 12800.0f, 1.0f, 12800.0f)     // 'far(low) far'
	    , mNearHighDist(this, 'cnhd', 900.0f, 10.0f, 5000.0f)    // 'near(high) distance'
	    , mNearHighAngle(this, 'cnha', 60.0f, 0.0f, 90.0f)       // 'near(high) angle'
	    , mNearHighFOV(this, 'cnhf', 10.0f, 1.0f, 60.0f)         // 'near(high) fov'
	    , mNearHighOffset(this, 'cnho', 40.0f, 10.0f, 500.0f)    // 'near(high) offset'
	    , mNearHighWeight(this, 'cnhw', 1000.0f, 10.0f, 5000.0f) // 'near(high) weight'
	    , mNearHighDetached(this, 'nhdt', 27.5f, 0.0f, 1000.0f)  // 'near(high) detached'
	    , mNearHighNear(this, 'nhnc', 1.0f, 1.0f, 12800.0f)      // 'near(high) near'
	    , mNearHighFar(this, 'nhfc', 12800.0f, 1.0f, 12800.0f)   // 'near(high) far'
	    , mMidHighDist(this, 'cmhd', 900.0f, 10.0f, 5000.0f)     // 'mid(high) distance'
	    , mMidHighAngle(this, 'cmha', 60.0f, 0.0f, 90.0f)        // 'mid(high) angle'
	    , mMidHighFOV(this, 'cmhf', 20.0f, 1.0f, 60.0f)          // 'mid(high) fov'
	    , mMidHighOffset(this, 'cmho', 70.0f, 10.0f, 500.0f)     // 'mid(high) offset'
	    , mMidHighWeight(this, 'cmhw', 500.0f, 10.0f, 5000.0f)   // 'mid(high) weight'
	    , mMidHighDetached(this, 'mhdt', 75.0f, 0.0f, 1000.0f)   // 'mid(high) detached'
	    , mMidHighNear(this, 'mhnc', 1.0f, 1.0f, 12800.0f)       // 'mid(high) near'
	    , mMidHighFar(this, 'mhfc', 12800.0f, 1.0f, 12800.0f)    // 'mid(high) far'
	    , mFarHighDist(this, 'cfhd', 1200.0f, 10.0f, 5000.0f)    // 'far(high) distance'
	    , mFarHighAngle(this, 'cfha', 60.0f, 0.0f, 90.0f)        // 'far(high) angle'
	    , mFarHighFOV(this, 'cfhf', 30.0f, 1.0f, 60.0f)          // 'far(high) fov'
	    , mFarHighOffset(this, 'cfho', 100.0f, 10.0f, 500.0f)    // 'far(high) offset'
	    , mFarHighWeight(this, 'cfhw', 200.0f, 10.0f, 5000.0f)   // 'far(high) weight'
	    , mFarHighDetached(this, 'fhdt', 165.0f, 0.0f, 1000.0f)  // 'far(high) detached'
	    , mFarHighNear(this, 'fhnc', 1.0f, 1.0f, 12800.0f)       // 'far(high) near'
	    , mFarHighFar(this, 'fhfc', 12800.0f, 1.0f, 12800.0f)    // 'far(high) far'
	    , mZoomDist(this, 'zmdt', 250.0f, 10.0f, 5000.0f)        // 'ZOOM distance'
	    , mZoomAngle(this, 'zman', 12.0f, 0.0f, 90.0f)           // 'ZOOM angle'
	    , mZoomFOV(this, 'zmfv', 35.0f, 1.0f, 60.0f)             // 'ZOOM fov'
	    , mCollRadius(this, 'clcr', 300.0f, 0.0f, 500.0f)        // 'collision radius'
	    , mCollInterpSpeed(this, 'clms', 0.005f, 0.001f, 1.0f)   // 'collision interpolation speed'
	    , mCollCorrHeight(this, 'clmh', 5.0f, 0.0f, 100.0f)      // 'collision correction height'
	    , mNoCollHeight(this, 'clnh', 25.0f, 0.0f, 500.0f)       // 'no collision height'
	    , mSettingChangeSpeed(this, 'cpmd', 0.1f, 0.0f, 1.0f)    // 'setting change speed'
	    , mRotSpeed(this, 'cmmt', 0.2f, 0.0f, 1.0f)              // 'rotation speed'
	    , mRotFollowTime(this, 'cmft', 0.6f, 0.0f, 3.0f)         // 'rotation follow time'
	    , mRotAccel(this, 'cmta', 0.005f, 0.0f, 1.0f)            // 'rotation acceleration'
	    , mMaxRotSpeed(this, 'cmtm', 0.9f, 0.0f, 5.0f)           // 'max rotation speed'
	    , mRotDampRate(this, 'cmtb', 0.85f, 0.0f, 1.0f)          // 'rotation damping rate'
	{
	}

	Parm<f32> mNearLowDist;        // _00C, cnld
	Parm<f32> mNearLowAngle;       // _034, cnla
	Parm<f32> mNearLowFOV;         // _05C, cnlf
	Parm<f32> mNearLowOffset;      // _084, cnlo
	Parm<f32> mNearLowWeight;      // _0AC, cnlw
	Parm<f32> mNearLowDetached;    // _0D4, nldt
	Parm<f32> mNearLowNear;        // _0FC, nlnc
	Parm<f32> mNearLowFar;         // _124, nlfc
	Parm<f32> mMidLowDist;         // _14C, cmld
	Parm<f32> mMidLowAngle;        // _174, cmla
	Parm<f32> mMidLowFOV;          // _19C, cmlf
	Parm<f32> mMidLowOffset;       // _1C4, cmlo
	Parm<f32> mMidLowWeight;       // _1EC, cmlw
	Parm<f32> mMidLowDetached;     // _214, mldt
	Parm<f32> mMidLowNear;         // _23C, mlnc
	Parm<f32> mMidLowFar;          // _264, mlfc
	Parm<f32> mFarLowDist;         // _28C, cfld
	Parm<f32> mFarLowAngle;        // _2B4, cfla
	Parm<f32> mFarLowFOV;          // _2DC, cflf
	Parm<f32> mFarLowOffset;       // _304, cflo
	Parm<f32> mFarLowWeight;       // _32C, clfw
	Parm<f32> mFarLowDetached;     // _354, fldt
	Parm<f32> mFarLowNear;         // _37C, flnc
	Parm<f32> mFarLowFar;          // _3A4, flfc
	Parm<f32> mNearHighDist;       // _3CC, cnhd
	Parm<f32> mNearHighAngle;      // _3F4, cnha
	Parm<f32> mNearHighFOV;        // _41C, cnhf
	Parm<f32> mNearHighOffset;     // _444, cnho
	Parm<f32> mNearHighWeight;     // _46C, cnhw
	Parm<f32> mNearHighDetached;   // _494, nhdt
	Parm<f32> mNearHighNear;       // _4BC, nhnc
	Parm<f32> mNearHighFar;        // _4E4, nhfc
	Parm<f32> mMidHighDist;        // _50C, cmhd
	Parm<f32> mMidHighAngle;       // _534, cmha
	Parm<f32> mMidHighFOV;         // _55C, cmhf
	Parm<f32> mMidHighOffset;      // _584, cmho
	Parm<f32> mMidHighWeight;      // _5AC, cmhw
	Parm<f32> mMidHighDetached;    // _5D4, mhdt
	Parm<f32> mMidHighNear;        // _5FC, mhnc
	Parm<f32> mMidHighFar;         // _624, mhfc
	Parm<f32> mFarHighDist;        // _64C, cfhd
	Parm<f32> mFarHighAngle;       // _674, cfha
	Parm<f32> mFarHighFOV;         // _69C, cfhf
	Parm<f32> mFarHighOffset;      // _6C4, cfho
	Parm<f32> mFarHighWeight;      // _6EC, cfhw
	Parm<f32> mFarHighDetached;    // _714, fhdt
	Parm<f32> mFarHighNear;        // _73C, fhnc
	Parm<f32> mFarHighFar;         // _764, fhfc
	Parm<f32> mZoomDist;           // _78C, zmdt
	Parm<f32> mZoomAngle;          // _7B4, zman
	Parm<f32> mZoomFOV;            // _7DC, zmfv
	Parm<f32> mCollRadius;         // _804, clcr
	Parm<f32> mCollInterpSpeed;    // _82C, clms
	Parm<f32> mCollCorrHeight;     // _854, clmh
	Parm<f32> mNoCollHeight;       // _87C, clnh
	Parm<f32> mSettingChangeSpeed; // _8A4, cpmd
	Parm<f32> mRotSpeed;           // _8CC, cmmt
	Parm<f32> mRotFollowTime;      // _8F4, cmft
	Parm<f32> mRotAccel;           // _91C, cmta
	Parm<f32> mMaxRotSpeed;        // _944, cmtm
	Parm<f32> mRotDampRate;        // _96C, cmtb
};

/**
 * @size{0x308}
 */
struct VibrationParms : public Parameters {
	VibrationParms()
	    : Parameters(nullptr, "VibrationParms")
	    , mElevationLightVib(this, 'celv', 1.0f, 0.0f, 10.0f)
	    , mElevationMiddleVib(this, 'cemv', 3.0f, 0.0f, 10.0f)
	    , mElevationHardVib(this, 'cehv', 5.0f, 0.0f, 10.0f)
	    , mElevationSlowSpeed(this, 'cess', 15.0f, 0.0f, 100.0f)
	    , mElevationMiddleSpeed(this, 'cems', 25.0f, 0.0f, 100.0f)
	    , mElevationFastSpeed(this, 'cefs', 35.0f, 0.0f, 100.0f)
	    , mElevationShortTime(this, 'cest', 0.3f, 0.0f, 5.0f)
	    , mElevationMiddleTime(this, 'cemt', 0.75f, 0.0f, 5.0f)
	    , mElevationLongTime(this, 'celt', 1.5f, 0.0f, 5.0f)
	    , mElevationHardVib2(this, 'ceqv', 20.0f, 0.0f, 50.0f)
	    , mElevationHardSpeed(this, 'ceqs', 35.0f, 0.0f, 100.0f)
	    , mElevationHardTime(this, 'ceqt', 0.25f, 0.0f, 5.0f)
	    , mZoomShortVib(this, 'czsv', 25.0f, 0.0f, 100.0f)
	    , mZoomShortSpeed(this, 'czss', 35.0f, 0.0f, 50.0f)
	    , mZoomShortTime(this, 'czst', 0.75f, 0.0f, 5.0f)
	    , mAzimuthShortVib(this, 'casv', 0.06f, 0.0f, 1.0f)
	    , mAzimuthShortSpeed(this, 'cass', 40.0f, 0.0f, 50.0f)
	    , mAzimuthShortTime(this, 'cast', 0.75f, 0.0f, 5.0f)
	    , mVibMaxDistance(this, 'cmdm', 750.0f, 0.0f, 1000.0f)
	{
	}

	Parm<f32> mElevationLightVib;    // _00C, celv
	Parm<f32> mElevationMiddleVib;   // _034, cemv
	Parm<f32> mElevationHardVib;     // _05C, cehv
	Parm<f32> mElevationSlowSpeed;   // _084, cess
	Parm<f32> mElevationMiddleSpeed; // _0AC, cems
	Parm<f32> mElevationFastSpeed;   // _0D4, cefs
	Parm<f32> mElevationShortTime;   // _0FC, cest
	Parm<f32> mElevationMiddleTime;  // _124, cemt
	Parm<f32> mElevationLongTime;    // _14C, celt
	Parm<f32> mElevationHardVib2;    // _174, ceqv
	Parm<f32> mElevationHardSpeed;   // _19C, ceqs
	Parm<f32> mElevationHardTime;    // _1C4, ceqt
	Parm<f32> mZoomShortVib;         // _1EC, czsv
	Parm<f32> mZoomShortSpeed;       // _214, czss
	Parm<f32> mZoomShortTime;        // _23C, czst
	Parm<f32> mAzimuthShortVib;      // _264, casv
	Parm<f32> mAzimuthShortSpeed;    // _28C, cass
	Parm<f32> mAzimuthShortTime;     // _2B4, cast
	Parm<f32> mVibMaxDistance;       // _2DC, cmdm
};

struct PlayCamera : public LookAtCamera {
	PlayCamera(Navi* target);

	virtual ~PlayCamera() { }                                         // _08 (weak)
	virtual bool isSpecialCamera();                                   // _70
	virtual void updateMatrix();                                      // _74
	virtual void doUpdate();                                          // _78
	virtual void init();                                              // _80
	virtual void startVibration(int) { }                              // _7C (weak)
	virtual Vector3f getLookAtPosition_() { return mLookAtPosition; } // _58 (weak)

	void setCameraParms(CameraParms* parms);
	void setVibrationParms(VibrationParms* parms);
	void setCameraAngle(f32 angle);
	void getCameraData(CameraData& data);
	void setCameraData(CameraData& data);
	void changePlayerMode(bool doCenterCameraBehind);
	void noUpdate();
	bool isVibration();
	void startVibration(int type, f32 strength);
	void startDemoCamera(int type);
	void finishDemoCamera();
	u32 updateCameraMode();
	void startZoomCamera();
	void startGameCamera(int);
	void setTargetParms();
	void setTargetThetaToWhistle();
	void setFollowTime();
	void setSmoothThetaSpeed();
	void changeTargetTheta();
	void changeTargetAtPosition();
	void updateParms(int flags);
	void updateVibration(int id);
	void otherVibFinished(int id);
	bool isModCameraFinished();
	void setCollisionCameraTargetPhi(int flags);
	f32 getCollisionCameraTargetPhi(f32 angle, f32 dist);

	// _00 		= VTBL
	// _00-_198	= LookAtCamera
	Navi* mTargetObj;                // _198
	int mChangePlayerState;          // _19C (is 1 during a player transition, 0 otherwise)
	int mCameraZoomLevel;            // _1A0 (what's toggled between when you press R)
	int mCameraSelAngle;             // _1A4 (what's toggled between when you press Z)
	f32 mCurrTargetDistance;         // _1A8
	f32 mGoalTargetDistance;         // _1AC
	f32 mCameraAngleCurrent;         // _1B0
	f32 mCameraAngleTarget;          // _1B4
	f32 mCurrVerticalAngle;          // _1B8
	f32 mGoalVerticalAngle;          // _1BC
	f32 mGoalFOV;                    // _1C0
	f32 mNearZPlane;                 // _1C4
	f32 mFarZPlane;                  // _1C8
	f32 mYOffset;                    // _1CC
	f32 mDetachedWeight;             // _1D0
	f32 mDetachedParm;               // _1D4 (used to offset camera position to put pikmin squad in frame)
	f32 mSmoothMoveSpeed;            // _1D8 (used in holding L mode)
	f32 mFollowTime;                 // _1DC
	f32 mHoldRTimer;                 // _1E0
	Vector3f mGoalPosition;          // _1E4
	bool mVibrateEnabled[3];         // _1F0
	f32 mVibrateSpeed[3];            // _1F4
	f32 mVibrateRollAngle[3];        // _200
	f32 mVibrateAngle[3];            // _20C
	f32 mVibrateTimer[3];            // _218
	f32 mVibrateDuration[3];         // _224
	f32 mVibrateScale[3];            // _230
	f32 mVibrateStrength[3];         // _23C
	bool mCanInput;                  // _248
	bool mIsCollisionCamActive;      // _249
	CameraParms* mCameraParms;       // _24C
	VibrationParms* mVibrationParms; // _250
};

/**
 * @size{0x3C}
 */
struct CameraMgr : public CNode {
	CameraMgr();

	virtual ~CameraMgr() { } // _08

	void loadResource();
	void setViewport(Viewport* vp, int id);
	void init(int state);
	void setCameraAngle(f32 angle, int id);
	void update();
	void startVibration(int type, Vector3f& pos, int camID);
	void startVibration(int type, int camID);
	void controllerLock(int camID);
	void controllerUnLock(int camID);
	void startDemoCamera(int camID, int type);
	void finishDemoCamera(int camID);
	void changePlayerMode(int naviID, IDelegate1<CameraArg*>* delegate);
	bool isChangePlayer();
	void setZukanCamera(LookAtCamera* cam);
	bool isCameraUpdateOn();
	bool isStartAndEnd(int* data, int camID);
	bool isVibrationStart(int type, int camID);
	void readCameraParms(char* path);
	void readParameter(Stream& stream);

	// _00 		= VTBL
	// _00-_18	= CNode
	int mCurrPlayerMode;               // _18 (0 = olimar, 1 = louie, 2 = multiplayer)
	int* mCameraVibrateState;          // _1C
	Viewport** mViewportList;          // _20
	PlayCamera** mCameraObjList;       // _24
	CameraParms* mCameraParms[2];      // _28
	VibrationParms* mVibrationParms;   // _30
	IDelegate1<CameraArg*>* mDelegate; // _34
	LookAtCamera* mZukanCamera;        // _38
};

extern CameraMgr* cameraMgr;
} // namespace Game

#endif
