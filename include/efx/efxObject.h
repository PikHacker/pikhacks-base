#ifndef _EFX_OBJECT_H
#define _EFX_OBJECT_H

#include "efx/TForever.h"
#include "efx/TSimple.h"
#include "efx/TSimpleMtx.h"
#include "efx/TChaseMtx.h"
#include "efx/TChasePos.h"
#include "efx/TChasePosYRot.h"
#include "ModelEffect.h"
#include "sys/MatBaseAnimator.h"
#include "sys/MatBaseAnimation.h"

namespace efx {

///////////////////////
//// GATE EFFECTS /////
///////////////////////

struct TEgateBC : public TForever2 {
	inline TEgateBC()
	    : TForever2(PID_EgateBC1, PID_EgateBC2)
	{
	}
	virtual bool create(Arg*); // _08

	// _00      = VTBL
	// _00-_24  = TForever2
};

////////////////////////////
//// CAVE HOLE EFFECTS /////
////////////////////////////

struct WarpZone : public TForever4 {
	inline WarpZone()
	    : TForever4(PID_WarpZone_1, PID_WarpZone_2, PID_WarpZone_3, PID_WarpZone_4)
	{
	}

	void setRateLOD(int, bool);

	// _00		= VTBL
	// _00-_44	= TForever4
};

////////////////////////
//// PELLET EFFECTS ////
////////////////////////

struct TPelkira_ver01 : public TChasePos {
	TPelkira_ver01(Vector3f* pos)
	    : TChasePos(PID_SyncDefault, pos)
	{
	}

	// vtable 1 (TBase)
	virtual bool create(Arg*);    // _08
	virtual ~TPelkira_ver01() { } // _48 (weak)
};

struct TOtakaraApL : public TSimple3 {
	inline TOtakaraApL()
	    : TSimple3(PID_OtakaraApL_1, PID_OtakaraApL_2, PID_OtakaraApL_3)
	{
	}

	virtual bool create(Arg*); // _08

	// _00     = VTBL
	// _00-_18 = TSimple3
};

struct TOtakaraApS : public TSimple2 {
	inline TOtakaraApS()
	    : TSimple2(PID_OtakaraApS_1, PID_OtakaraApS_2)
	{
	}

	virtual bool create(Arg*); // _08

	// _00     = VTBL
	// _00-_10 = TSimple2
};

struct TOtakaraAp : public TBase {
	TOtakaraAp()
	    : TBase()
	{
	}
	virtual bool create(Arg*);   // _08
	virtual void forceKill() { } // _0C (weak)
	virtual void fade() { }      // _10 (weak)

	// _00 VTBL
};

struct TOtakaraDive : public TSimple3 {
	inline TOtakaraDive()
	    : TSimple3(PID_OtakaraDive_1, PID_OtakaraDive_2, PID_OtakaraDive_3)
	{
	}

	virtual bool create(Arg*); // _08

	// _00     = VTBL
	// _00-_18 = TSimple3
};

///////////////////////////////////////////
//// PLANT/FRUIT (SPIDERWORTS) EFFECTS ////
///////////////////////////////////////////

struct TTsuyuGrow0 : public TSimple1 {
	inline TTsuyuGrow0()
	    : TSimple1(PID_TsuyuGrow0)
	{
	}

	virtual bool create(Arg*); // _08

	// _00     = VTBL
	// _00-_0C = TSimple1
};

//////////////////////////////////////////
//// KOUHAI (SPIDERWORT MOLD) EFFECTS ////
//////////////////////////////////////////

struct TKouhaiDamage : public TSimple1 {
	inline TKouhaiDamage()
	    : TSimple1(PID_KouhaiDamage)
	{
	}

	virtual bool create(Arg*); // _08

	// _00     = VTBL
	// _00-_0C = TSimple1
};

////////////////////////
//// ONION EFFECTS /////
////////////////////////

struct Container : public TForever {
	inline Container()
	    : TForever(PID_Container_Blue)
	{
	}
	virtual bool create(Arg*); // _08
	virtual ~Container() { }   // _48 (weak)

	// _00      = VTBL
	// _00-_10  = TForever
};

struct ContainerAct : public TForever2 {
	inline ContainerAct()
	    : TForever2(PID_ContainerAct_Blue_1, PID_ContainerAct_Blue_2)
	{
	}
	virtual bool create(Arg*); // _08

	// _00      = VTBL
	// _00-_24  = TForever2
};

struct OnyonSpot;

struct OnyonSpotArg : public ModelEffectCreateArg {
	inline OnyonSpotArg(Vector3f& orig, u32 onyonType)
	    : mOnyonType(onyonType)
	{
		mOrig = orig;
	}

	virtual u64 getID() { return 'ONY_SPOT'; } // _08 (weak)

	// _00 		= VTBL
	Vector3f mOrig; // _04
	u32 mOnyonType; // _10
};

struct OnyonSpotData : public ModelEffectData {
	virtual ~OnyonSpotData() { }                              // _08 (weak)
	virtual void loadResources();                             // _10
	virtual u64 getID() { return 'ONY_SPOT'; }                // _14 (weak)
	virtual ModelEffect* onCreate(ModelEffectCreateArg* arg); // _18

	int mTexAnimCount;                  // _20
	Sys::MatTexAnimation* mTexAnims;    // _24
	int mTevAnimCount;                  // _28
	Sys::MatTevRegAnimation* mTevAnims; // _2C
};

struct OnyonSpot : public ModelEffect {
	virtual void changeMaterial();                     // _1C
	virtual void getLODSphere(Sys::Sphere& lodSphere); // _20
	virtual bool useCylinderLOD() { return true; }     // _24 (weak)
	virtual void getLODCylinder(Sys::Cylinder&);       // _28

	// unused/inlined
	void initAnimators(Sys::MatTexAnimation*, Sys::MatTevRegAnimation*);

	Sys::MatLoopAnimator mAnim1; // _3C
	Sys::MatLoopAnimator mAnim2; // _48
};

} // namespace efx

#endif
