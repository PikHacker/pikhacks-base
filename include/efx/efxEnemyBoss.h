#ifndef _EFX_ENEMYBOSS_H
#define _EFX_ENEMYBOSS_H

#include "efx/TSimple.h"
#include "efx/TForever.h"
#include "efx/TOneEmitter.h"
#include "efx/TSimpleMtx.h"
#include "efx/TChaseMtx.h"
#include "efx/TChasePos.h"
#include "efx/TChasePosPos.h"
#include "efx/TChasePosYRot.h"

namespace efx {

///////////////////////////////////////////////
//// SNAKECROW (BURROWING SNAGRET) EFFECTS ////
///////////////////////////////////////////////

struct THebiAphd_base : public TSimple4 {
	inline THebiAphd_base(u16 effectID1, u16 effectID2, u16 effectID3, u16 effectID4, u32 duration)
	    : TSimple4(effectID1, effectID2, effectID3, effectID4)
	    , mMaxDuration(duration) // this is from the THebiAphd_dive ctor
	{
	}

	virtual bool create(Arg*); // _08

	// _00      = VTBL
	// _00-_1C  = TSimple4
	u32 mMaxDuration; // _1C
};

//////////////////////////////////////////////
//// KINGCHAPPY (EMPEROR BULBLAX) EFFECTS ////
//////////////////////////////////////////////

struct TKchApSand : public TSimple3 {
	inline TKchApSand()
	    : TSimple3(PID_KchApSand_1, PID_KchApSand_2, PID_KchApSand_3)
	{
	}

	virtual bool create(Arg*); // _08

	// _00      = VTBL
	// _00-_18  = TSimple3
};

struct TKchApWat : public TSimple5 {
	inline TKchApWat()
	    : TSimple5(PID_KchApWat_1, PID_KchApWat_2, PID_KchApWat_3, PID_KchApWat_4, PID_KchApWat_5)
	{
	}

	virtual bool create(Arg*); // _08

	// _00      = VTBL
	// _00-_24  = TSimple5
};

struct TKchCryAB : public TChasePos2 {
	inline TKchCryAB(Vector3f* position)
	    : TChasePos2(position, PID_KchCryAB_1, PID_KchCryAB_2)
	{
	}

	void setGlobalScale(f32);

	// _00      = VTBL
	// _00-_2C  = TChasePos2
};

struct TKchCryInd : public TChaseMtxT {
	inline TKchCryInd()
	    : TChaseMtxT(PID_KchCryInd)
	{
	}

	virtual ~TKchCryInd() { } // _48 (weak)

	void setGlobalScale(f32);

	// _00      = VTBL
	// _00-_14  = TChaseMtxT
};

struct TKchDamage : public TSimpleMtx4 {
	inline TKchDamage(Matrixf* mtx)
	    : TSimpleMtx4(mtx, PID_KchDamage_1, PID_KchDamage_2, PID_KchDamage_3, PID_KchDamage_4)
	{
	}

	virtual bool create(Arg*); // _08

	// _00      = VTBL
	// _00-_1C  = TSimple4
};

struct TKchDeadHana : public TChaseMtx2 {
	inline TKchDeadHana()
	    : TChaseMtx2(nullptr, PID_KchDeadHana_1, PID_KchDeadHana_2)
	{
	}

	void setGlobalScale(f32);

	// _00      = VTBL
	// _00-_2C  = TChaseMtx2
};

struct TKchDiveSand : public TChasePosYRot3 {
	inline TKchDiveSand(Vector3f* position, f32* rotation)
	    : TChasePosYRot3(position, rotation, PID_KchDiveSand_1, PID_KchDiveSand_2, PID_KchDiveSand_3)
	{
	}

	void setGlobalScale(f32);

	// _00      = VTBL
	// _00-_4C  = TChasePosYRot3
};

struct TKchDiveWat : public TChasePosYRot3 {
	inline TKchDiveWat(Vector3f* position, f32* rotation)
	    : TChasePosYRot3(position, rotation, PID_KchDiveWat_1, PID_KchDiveWat_2, PID_KchDiveWat_3)
	{
	}

	void setGlobalScale(f32);

	// _00      = VTBL
	// _00-_4C  = TChasePosYRot3
};

struct TKchDownsmoke : public TSimple1 {
	inline TKchDownsmoke()
	    : TSimple1(PID_KchDownSmoke)
	{
	}

	virtual bool create(Arg*); // _08

	// _00      = VTBL
	// _00-_0C  = TSimple1
};

struct TKchFlickSand : public TSimple2 {
	inline TKchFlickSand()
	    : TSimple2(PID_KchFlickSand_1, PID_KchFlickSand_2)
	{
	}

	virtual bool create(Arg*); // _08

	// _00      = VTBL
	// _00-_10  = TSimple2
};

struct TKchSmokeHana : public TChaseMtx {
	inline TKchSmokeHana()
	    : TChaseMtx(PID_KchSmokeHana, nullptr)
	{
	}

	virtual ~TKchSmokeHana() { } // _48 (weak)

	void setGlobalScale(f32);

	// _00      = VTBL
	// _00-_14  = TChaseMtx
};

struct TParticleCallBack_KchYodare : public JPAParticleCallBack {
	inline TParticleCallBack_KchYodare()
	    : mHitGround(PID_KchYodareHitGr)
	    , mHitWater(PID_KchYodareHitWat)
	{
	}

	virtual ~TParticleCallBack_KchYodare() { }               // _08 (weak)
	virtual void execute(JPABaseEmitter*, JPABaseParticle*); // _0C
	virtual void init(JPABaseEmitter*, JPABaseParticle*);    // _14

	// _00 VTBL
	TOneEmitterSimple mHitGround; // _04 (size 0x1c)
	TOneEmitterSimple mHitWater;  // _20
	f32 mGroundYPos;              // _3C
};

struct TKchYodareBaseChaseMtx : public TChaseMtx {
	inline TKchYodareBaseChaseMtx(Mtx mtx, u16 effectID)
	    : TChaseMtx(effectID, (Matrixf*)mtx)
	{
		mParticleCallBack.mGroundYPos = 0.0f;
	}

	virtual bool create(Arg*); // _08
	virtual void forceKill()   // _0C (weak)
	{
		TChaseMtx::forceKill();
		mParticleCallBack.mHitGround.forceKill();
		mParticleCallBack.mHitWater.forceKill();
	}
	virtual void fade() // _10 (weak)
	{
		TChaseMtx::fade();
		mParticleCallBack.mHitGround.fade();
		mParticleCallBack.mHitWater.fade();
	}
	virtual void startDemoDrawOff() // _40 (weak)
	{
		mFlags |= 1;
		mParticleCallBack.mHitGround.startDemoDrawOff();
		mParticleCallBack.mHitWater.startDemoDrawOff();
	}
	virtual void endDemoDrawOn() // _44 (weak)
	{
		mFlags &= ~1;
		mParticleCallBack.mHitGround.endDemoDrawOn();
		mParticleCallBack.mHitWater.endDemoDrawOn();
	}
	virtual ~TKchYodareBaseChaseMtx() { } // _48 (weak)

	void setGlobalScale(f32);

	// _00      = VTBL
	// _00-_14  = TChaseMtx
	TParticleCallBack_KchYodare mParticleCallBack; // _14
};

//////////////////////////////////////
//// HOUDAI (MAN AT LEGS) EFFECTS ////
//////////////////////////////////////

struct THdamaDeadHahen1 : public TSimple1 {
	inline THdamaDeadHahen1()
	    : TSimple1(PID_HDamaDeadHahen1)
	{
	}

	virtual bool create(Arg*); // _08

	// _00		= VTBL
	// _00-_0C	= TSimple1
};

struct THdamaDeadHahen2 : public TSimple2 {
	inline THdamaDeadHahen2()
	    : TSimple2(PID_HDamaDeadHahen2_1, PID_HDamaDeadHahen2_2)
	{
	}

	virtual bool create(Arg*); // _08

	// _00		= VTBL
	// _00-_10	= TSimple2
};

struct THdamaHit2W : public TSimple3 {
	inline THdamaHit2W()
	    : TSimple3(PID_HDamaHit2W_1, PID_HDamaHit2W_2, PID_HDamaHit2W_3)
	{
	}

	virtual bool create(Arg*); // _08

	// _00		= VTBL
	// _00-_18	= TSimple3
};

struct THdamaShell : public TChasePos {
	inline THdamaShell()
	    : TChasePos(PID_HDamaShell)
	{
	}

	virtual bool create(Arg*); // _08

	// _00		= VTBL
	// _00-_14	= TChasePos
};

struct THdamaSight : public TForever {
	inline THdamaSight()
	    : TForever(PID_HDamaSight)
	{
	}

	virtual ~THdamaSight() { } // _48 (weak)

	void setPosNrm(Vector3f&, Vector3f&);

	// _00		= VTBL
	// _00-_10	= TForever
};

/////////////////////////////////////////////
//// UMIMUSHI (RANGING BLOYSTER) EFFECTS ////
/////////////////////////////////////////////

struct TUmiAttack : public TSimpleMtx1 {
	inline TUmiAttack(Matrixf* mtx)
	    : TSimpleMtx1(mtx, PID_UmiAttack)
	{
	}

	virtual bool create(Arg*); // _08

	// _00     = VTBL
	// _00-_0C = TSimple1
};

struct TUmiDeadawa : public TChaseMtx {
	inline TUmiDeadawa()
	    : TChaseMtx(PID_UmiDeadAwa, nullptr)
	{
	}

	virtual ~TUmiDeadawa() { } // _48 (weak)

	void setGlobalScale(f32);

	// _00     = VTBL
	// _00-_14 = TChaseMtx
};

struct TUmiDeadmelt : public TSimple1 {
	inline TUmiDeadmelt()
	    : TSimple1(PID_UmiDeadMelt)
	{
	}

	virtual bool create(Arg*); // _08

	// _00     = VTBL
	// _00-_0C = TSimple1
};

struct TUmiEat : public TChaseMtx {
	inline TUmiEat()
	    : TChaseMtx(PID_UmiEat, nullptr)
	{
	}

	virtual ~TUmiEat() { } // _48 (weak)

	void setGlobalScale(f32);

	// _00     = VTBL
	// _00-_14 = TChaseMtx
};

struct TUmiFlick : public TSimple3 {
	inline TUmiFlick()
	    : TSimple3(PID_UmiFlick_1, PID_UmiFlick_2, PID_UmiFlick_3)
	{
	}

	virtual bool create(Arg*); // _08

	// _00     = VTBL
	// _00-_18 = TSimple3
};

struct TUmiHamon : public TChasePos2 {
	inline TUmiHamon(Vector3f* pos)
	    : TChasePos2(pos, PID_UmiHamon_1, PID_UmiHamon_2)
	{
	}

	void setGlobalScale(f32);

	// _00     = VTBL
	// _00-_2C = TChasePos2
};

////////////////////////////////////////
//// BLACKMAN (WATERWRAITH) EFFECTS ////
////////////////////////////////////////

struct TKageDead1 : public TChaseMtx {
	inline TKageDead1()
	    : TChaseMtx(PID_KageDead1, nullptr)
	{
	}

	virtual ~TKageDead1() { } // _48 (weak)

	void setGlobalPrmColor(Color4&);

	// _00      = VTBL
	// _00-_14  = TChaseMtx
};

struct TKageDead2 : public TSimple1 {
	inline TKageDead2()
	    : TSimple1(PID_KageDead2)
	{
	}

	virtual bool create(Arg*); // _08

	// _00      = VTBL
	// _00-_0C  = TSimple1
};

struct TKageMove : public TChasePosYRot {

	inline TKageMove(Vector3f* pos, f32* rot)
	    : TChasePosYRot(pos, rot, PID_KageMove)
	{
	}
	virtual ~TKageMove() { } // _48 (weak)

	void setGlobalPrmColor(Color4&);

	// _00      = VTBL
	// _00-_18  = TChasePosYRot
};

struct TKageRun : public TChasePosYRot {
	inline TKageRun(Vector3f* pos, f32* rot)
	    : TChasePosYRot(pos, rot, PID_KageRun)
	{
	}
	virtual ~TKageRun() { } // _48 (weak)

	void setGlobalPrmColor(Color4&);

	// _00      = VTBL
	// _00-_18  = TChasePosYRot
};

//////////////////////////////////////////////////
//// DANGOMUSHI (SEGMENTED CRAWBSTER) EFFECTS ////
//////////////////////////////////////////////////

struct TDangoCrash : public TSimple2 {
	inline TDangoCrash()
	    : TSimple2(PID_DangoCrash_1, PID_DangoCrash_2)
	{
	}

	virtual bool create(Arg*); // _08

	// _00      = VTBL
	// _00-_10  = TSimple2
};

struct TDangoTurn : public TSimple2 {
	TDangoTurn()
	    : TSimple2(PID_DangoTurn_1, PID_DangoTurn_2)
	{
	}

	virtual bool create(Arg*); // _08

	// _00      = VTBL
	// _00-_10  = TSimple2
};

//////////////////////////////////////////
//// ODAMA (RAGING LONG LEGS) EFFECTS ////
//////////////////////////////////////////

struct TOdamaFur1 : public TChaseMtx {
	inline TOdamaFur1()
	    : TChaseMtx(PID_ODamaFur1, nullptr)
	{
	}

	virtual ~TOdamaFur1() { } // _48 (weak)

	void setGlobalScale(f32);

	// _00		  = VTBL
	// _00-_14	= TChaseMtx
};

struct TOdamaFur2 : public TChaseMtx {
	inline TOdamaFur2()
	    : TChaseMtx(PID_ODamaFur2, nullptr)
	{
	}

	virtual ~TOdamaFur2() { } // _48 (weak)

	void setGlobalScale(f32);

	// _00		  = VTBL
	// _00-_14	= TChaseMtx
};

///////////////////////////////////////////
//// OOOTAKARA (TITAN DWEEVIL) EFFECTS ////
///////////////////////////////////////////

struct TOootaBombLeg : public TSimple1 {
	inline TOootaBombLeg()
	    : TSimple1(PID_OoOtaBombLeg)
	{
	}

	virtual bool create(Arg*); // _08

	// _00     = VTBL
	// _00-_0C = TSimple1
};

struct TOootaFire : public TChaseMtx6 {
	inline TOootaFire()
	    : TChaseMtx6(nullptr, PID_OoOtaFire_1, PID_OoOtaFire_2, PID_OoOtaFire_3, PID_OoOtaFire_4, PID_OoOtaFire_6, PID_OoOtaFire_5)
	{
	}

	virtual bool create(Arg*); // _08

	// _00     = VTBL
	// _00-_7C = TChaseMtx6
};

struct TOootaParticle : public TChasePos {
	inline TOootaParticle()
	    : TChasePos(PID_OoOtaParticle, nullptr)
	{
	}

	virtual ~TOootaParticle() { } // _48 (weak)

	void setGlobalDynamicsScale(f32);

	// _00     = VTBL
	// _00-_14 = TChasePos
};

}; // namespace efx

#endif
