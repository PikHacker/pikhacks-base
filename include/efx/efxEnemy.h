#ifndef _EFX_ENEMY_H
#define _EFX_ENEMY_H

#include "efx/TChasePos.h"
#include "efx/TSimple.h"
#include "efx/TSimpleMtx.h"
#include "efx/TChaseMtx.h"
#include "efx/TForever.h"
#include "efx/TOneEmitter.h"
#include "efx/TChasePosYRot.h"

namespace efx {

//////////////////////////////////
//// FART (DOODLEBUG) EFFECTS ////
//////////////////////////////////

struct TBabaFly_ver01 : public TChasePos {
	TBabaFly_ver01(Vector3f* position)
	    : TChasePos(PID_BabaFly, position)
	{
	}

	virtual bool create(Arg*);    // _08
	virtual ~TBabaFly_ver01() { } // _48 (weak)

	// _00      = VTBL
	// _00-_14  = TChasePos
};

struct TBabaHe : public TSimple2 {
	TBabaHe()
	    : TSimple2(PID_BabaHe_1, PID_BabaHe_2)
	{
	}

	virtual bool create(Arg*); // _08

	// _00      = VTBL
	// _00-_0C  = TSimple1
};

///////////////////////////
//// BOMB ROCK EFFECTS ////
///////////////////////////
struct TBombrock : public TBase {
	inline TBombrock()
	    : mEfxBombABCD(PID_BombrockABCD_1, PID_BombrockABCD_2, PID_BombrockABCD_3, PID_BombrockABCD_4)
	    , mEfxBombEFGH(PID_BombrockEFGH_1, PID_BombrockEFGH_2, PID_BombrockEFGH_3, PID_BombrockEFGH_4)
	{
	}

	virtual bool create(Arg* arg) // _08 (weak)
	{
		bool result = false;
		if ((&mEfxBombABCD)->create(arg) && (&mEfxBombEFGH)->create(arg)) {
			result = true;
		}
		return result;
	}
	virtual void forceKill() { } // _0C (weak)
	virtual void fade() { }      // _10 (weak)

	// _00      = VTBL
	TSimple4 mEfxBombABCD; // _04
	TSimple4 mEfxBombEFGH; // _20
};

////////////////////////////////////////
//// CHIBI (GATLING GROINK) EFFECTS ////
////////////////////////////////////////

struct TChibiHit : public TSimple4 {
	inline TChibiHit()
	    : TSimple4(PID_ChibiHit_1, PID_ChibiHit_2, PID_ChibiHit_3, PID_ChibiHit_4)
	{
	}

	virtual bool create(Arg*); // _08

	// _00      = VTBL
	// _00-_1C  = TSimple4
};

///////////////////////////////////
//// CHOU (SPECTRALID) EFFECTS ////
///////////////////////////////////

struct TChouDown : public TChasePos {
	inline TChouDown(Vector3f* pos)
	    : TChasePos(pos, PID_SyncDefault)
	{
	}

	virtual bool create(Arg*); // _08
	virtual ~TChouDown() { }   // _48 (weak)

	// _00      = VTBL
	// _00-_14  = TChasePos
};

///////////////////////////////////////////
//// DENKIHIBA (ELECTRIC WIRE) EFFECTS ////
///////////////////////////////////////////

struct TDenkiHiba : public TForever3 {
	inline TDenkiHiba()
	    : TForever3(PID_DenkiHiba_1, PID_DenkiHiba_2, PID_DenkiHiba_3)
	{
	}

	virtual bool create(Arg*); // _08

	// unused/inlined:
	void setRateLOD(int);

	// _00      = VTBL
	// _00-_34  = TForever3
};

struct TDenkiPole : public TForever2 {
	TDenkiPole()
	    : TForever2(PID_DenkiPole_1, PID_DenkiPole_2)
	{
	}

	~TDenkiPole() { }

	// _00      = VTBL
	// _00-_24  = TForever2
};

struct TDenkipoleSign : public TForever {
	TDenkipoleSign()
	    : TForever(PID_DenkiPoleSign)
	{
	}

	virtual ~TDenkipoleSign() { } // _48 (weak)

	// _00      = VTBL
	// _00-_10  = TForever
};

struct TDenkiHibaMgr : public TBase {
	inline TDenkiHibaMgr() { }

	virtual bool create(Arg*); // _08
	virtual void forceKill();  // _0C
	virtual void fade();       // _10

	bool createHiba(int);
	void setRateLOD(int);

	// unused/inlined:
	bool createHiba();

	// _00  = VTBL
	TDenkiHiba mHiba;                 // _04
	TDenkiPole mPoles[2];             // _38
	TDenkipoleSign mPolesigns[2];     // _80
	Vector3f mOwnerPosition;          // _A0
	Vector3f mTargetCreaturePosition; // _AC
};

////////////////////////////////////////
//// ELECBUG (ANODE BEETLE) EFFECTS ////
////////////////////////////////////////

struct TDnkmsThunderA : public TSync {
	inline TDnkmsThunderA()
	    : TSync(PID_DnkmsThunderA)
	{
	}

	virtual void doExecuteEmitterOperation(JPABaseEmitter*); // _38
	virtual ~TDnkmsThunderA() { }                            // _48 (weak)

	inline void setupEffect(Vector3f* pos, Vector3f* partnerPos)
	{
		mPosition        = pos;
		mPartnerPosition = partnerPos;
	}

	// _00      = VTBL
	// _00-_10  = TSync
	Vector3f* mPosition;        // _10
	Vector3f* mPartnerPosition; // _14
};

struct TDnkmsThunderB : public TSync {
	inline TDnkmsThunderB()
	    : TSync(PID_DnkmsThunderB)
	{
		// mEffectID = PID_DnkmsThunderB;
	}

	virtual void doExecuteEmitterOperation(JPABaseEmitter*); // _38
	virtual ~TDnkmsThunderB() { }                            // _48 (weak)

	inline void setupEffect(Vector3f* pos, Vector3f* partnerPos)
	{
		mPosition        = pos;
		mPartnerPosition = partnerPos;
	}

	// _00      = VTBL
	// _00-_10  = TSync
	Vector3f* mPosition;        // _10
	Vector3f* mPartnerPosition; // _14
};

struct TDnkmsEffect {
	TDnkmsEffect()
	    : mHoudenA1(PID_DnkmsHoudenA)
	    , mHoudenA2(PID_DnkmsHoudenA)
	    , mHoudenB(PID_DnkmsHoudenB)
	    , mThunderA()
	    , mThunderB()
	{
	}

	inline void startCharge()
	{
		mHoudenB.mPosition = mPosition;
		mHoudenB.create(nullptr);
	}

	inline void startDischarge(Vector3f* partnerPos)
	{
		mPartnerPosition = partnerPos;

		mHoudenA1.mPosition = mPosition;
		mHoudenA1.create(nullptr);
		mHoudenA2.mPosition = mPartnerPosition;
		mHoudenA2.create(nullptr);

		mThunderA.setupEffect(mPosition, mPartnerPosition);
		mThunderB.setupEffect(mPosition, mPartnerPosition);

		mThunderA.create(nullptr);
		mThunderB.create(nullptr);
	}

	inline void fade()
	{
		mHoudenA1.fade();
		mHoudenA2.fade();
		mHoudenB.fade();
		mThunderA.fade();
		mThunderB.fade();
	}

	inline void effectDrawOn()
	{
		mHoudenA1.endDemoDrawOn();
		mHoudenA2.endDemoDrawOn();
		mHoudenB.endDemoDrawOn();
		mThunderA.endDemoDrawOn();
		mThunderB.endDemoDrawOn();
	}

	inline void effectDrawOff()
	{
		mHoudenA1.startDemoDrawOff();
		mHoudenA2.startDemoDrawOff();
		mHoudenB.startDemoDrawOff();
		mThunderA.startDemoDrawOff();
		mThunderB.startDemoDrawOff();
	}

	Vector3f* mPosition;        // _00
	Vector3f* mPartnerPosition; // _04
	TChasePos mHoudenA1;        // _08, discharge effect (self)
	TChasePos mHoudenA2;        // _1C, discharge effect (partner)
	TChasePos mHoudenB;         // _30, charge effect
	TDnkmsThunderA mThunderA;   // _44
	TDnkmsThunderB mThunderB;   // _5C
};

///////////////////////////////////////////
//// GASHIBA (POISON GAS PIPE) EFFECTS ////
///////////////////////////////////////////

struct TGasuHiba : public TForever2 {
	inline TGasuHiba()
	    : TForever2(PID_GasuHiba_1, PID_GasuHiba_2)
	{
	}
	virtual bool create(Arg*); // _08

	void setRateLOD(int);

	// _00      = VTBL
	// _00-_24  = TForever2
};

////////////////////////////////////////
//// FIREHIBA (FIRE GEYSER) EFFECTS ////
////////////////////////////////////////

struct THibaFire : public TForever4 {
	inline THibaFire()
	    : TForever4(PID_HibaFire_1, PID_HibaFire_2, PID_HibaFire_3, PID_HibaFire_4)
	{
	}

	void setRateLOD(int);

	// _00      = VTBL
	// _00-_44  = TForever4
};

///////////////////////////////////////
//// CHAPPY/HANA (BULBORB) EFFECTS ////
///////////////////////////////////////

struct TKechappyTest : public TChaseMtx3 {
	TKechappyTest()
	    : TChaseMtx3(0, PID_KechappyTest_1, PID_KechappyTest_2, PID_KechappyTest_3)
	{
	}

	void setGlobalAlpha(u8 alpha);
	void setGlobalParticleScale(f32 scale);
	void setAwayFromCenterSpeed(f32 speed);
	void setSpread(f32 spread);
	void setGlobalDynamicsScale(Vector3f& scale);

	// _00      = VTBL
	// _00-_40  = TChaseMtx3
};

//////////////////////////////////////////
//// IMOMUSHI (WHISKERPILLAR) EFFECTS ////
//////////////////////////////////////////

struct TImoEat : public TChaseMtx {
	inline TImoEat()
	    : TChaseMtx(PID_ImoEat_1, nullptr)
	{
	}

	virtual bool create(Arg*); // _08
	virtual ~TImoEat() { }     // _48 (weak)

	// _00      = VTBL
	// _00-_14  = TChaseMtx
};

struct TImoSmoke : public TChasePos {
	inline TImoSmoke(Vector3f* pos)
	    : TChasePos(pos, PID_ImoSmoke)
	{
	}

	virtual bool create(Arg*); // _08
	virtual ~TImoSmoke() { }   // _48 (weak)

	// _00      = VTBL
	// _00-_14  = TChasePos
};

/////////////////////////////////////////
//// JIGUMO (HERMIT CRAWMAD) EFFECTS ////
/////////////////////////////////////////

struct TJgmAttack : public TChasePos2 {
	inline TJgmAttack(Vector3f* pos)
	    : TChasePos2(pos, PID_JgmAttack_1, PID_JgmAttack_2)
	{
	}

	virtual bool create(Arg*); // _08

	// _00      = VTBL
	// _00-_2C  = TChasePos2
};

struct TJgmAttackW : public TChasePosYRot3 {
	inline TJgmAttackW(Vector3f* pos, f32* dir)
	    : TChasePosYRot3(pos, dir, PID_JgmAttackW_1, PID_JgmAttackW_2, PID_JgmAttackW_3)
	{
	}

	virtual bool create(Arg*); // _08

	// _00      = VTBL
	// _00-_4C  = TChasePosYRot3
};

struct TJgmBack : public TChasePos {
	inline TJgmBack(Vector3f* pos)
	    : TChasePos(pos, PID_JgmBACK)
	{
	}

	virtual bool create(Arg*); // _08
	virtual ~TJgmBack() { }    // _48 (weak)

	// _00      = VTBL
	// _00-_14  = TChasePos
};

struct TJgmBackW : public TChasePosYRot2 {
	inline TJgmBackW(Vector3f* pos, f32* dir)
	    : TChasePosYRot2(pos, dir, PID_JgmBACKW_1, PID_JgmBACKW_2)
	{
	}

	virtual bool create(Arg*); // _08

	// _00      = VTBL
	// _00-_34  = TChasePosYRot2
};

///////////////////////////////////////
//// KOGANE (FLINT BEETLE) EFFECTS ////
///////////////////////////////////////

struct TKoganeDive : public TSimple2 {
	inline TKoganeDive()
	    : TSimple2(PID_KoganeDive_1, PID_KoganeDive_2)
	{
	}

	virtual bool create(Arg*); // _08

	// _00		 = VTBL
	// _00-_10 = TSimple2
};

struct TKoganeHit : public TSimple2 {
	inline TKoganeHit()
	    : TSimple2(PID_KoganeHit_1, PID_KoganeHit_2)
	{
	}

	virtual bool create(Arg*); // _08

	// _00		 = VTBL
	// _00-_10 = TSimple2
};

////////////////////////////////////////////
//// KURAGE (LESSER JELLYFLOAT) EFFECTS ////
////////////////////////////////////////////
// Kurage is actually for OniKurage, NewKurage is for normal Kurage

struct TKurageHire : public TChaseMtx3 {
	inline TKurageHire()
	    : TChaseMtx3(0, PID_KurageHire_1, PID_KurageHire_2, PID_KurageHire_3)
	{
	}

	void setLifeTime(s16);

	// _00		 = VTBL
	// _00-_40 = TChaseMtx3
};

struct TKurageSui : public TForever2 {
	inline TKurageSui()
	    : TForever2(PID_KurageSui_1, PID_KurageSui_2)
	{
	}

	void setGlobalTranslation(Vector3f&);

	// _00		 = VTBL
	// _00-_24 = TForever2
};

struct TNewkurageHire : public TChaseMtx3 {
	inline TNewkurageHire()
	    : TChaseMtx3(0, PID_NewKurageHire_1, PID_NewKurageHire_2, PID_NewKurageHire_3)
	{
	}

	void setLifeTime(s16);

	// _00		 = VTBL
	// _00-_40 = TChaseMtx3
};

struct TNewkurageSui : public TForever2 {
	inline TNewkurageSui()
	    : TForever2(PID_NewKurageSui_1, PID_NewKurageSui_2)
	{
	}

	void setGlobalTranslation(Vector3f&);

	// _00		 = VTBL
	// _00-_24 = TForever2
};

///////////////////////////////////
//// OTAKARA (DWEEVIL) EFFECTS ////
///////////////////////////////////

struct TOtaPartsoff : public TSimple1 {
	inline TOtaPartsoff()
	    : TSimple1(PID_OtaPartsOff)
	{
	}

	virtual bool create(Arg*); // _08

	// _00      = VTBL
	// _00-_0C  = TSimple1
};

//////////////////////////////////////
//// PANMODOKI (BREADBUG) EFFECTS ////
//////////////////////////////////////

struct TPanApp : public TSimple1 {
	inline TPanApp()
	    : TSimple1(PID_PanApp)
	{
	}

	virtual bool create(Arg*); // _08

	// _00     = VTBL
	// _00-_0C = TSimple1
};

struct TPanHide : public TForever {
	inline TPanHide()
	    : TForever(PID_PanHide)
	{
	}

	virtual bool create(Arg*); // _08
	virtual ~TPanHide() { }    // _48 (weak)

	// _00     = VTBL
	// _00-_10 = TForever
};

struct TPanSmoke : public TChasePos {
	inline TPanSmoke(Vector3f* pos)
	    : TChasePos(pos, PID_PanSmoke)
	{
	}

	virtual bool create(Arg*); // _08
	virtual ~TPanSmoke() { }   // _48 (weak)

	// _00     = VTBL
	// _00-_14 = TChasePos
};

/////////////////////////////////////
//// QURIONE (HONEYWISP) EFFECTS ////
/////////////////////////////////////

struct TQuriGlow : public TChaseMtxT {
	inline TQuriGlow()
	    : TChaseMtxT(PID_QuriGlow)
	{
	}

	virtual ~TQuriGlow() { } // _48 (weak)

	void setGlobalScale(f32);

	// _00     = VTBL
	// _00-_14 = TChaseMtxT
};

////////////////////////////////////
//// UJINKO (SHEARGRUB) EFFECTS ////
////////////////////////////////////

struct TUjinkoAp_Imo : public TSimpleMtx2 {
	inline TUjinkoAp_Imo(Matrixf* mtx)
	    : TSimpleMtx2(mtx, PID_UjinkoAp_1, PID_UjinkoAp_2)
	{
	}

	virtual bool create(Arg*); // _08

	// _00     = VTBL
	// _00-_10 = TSimple2
};

struct TUjinkoHd_Imo : public TSimpleMtx2 {
	inline TUjinkoHd_Imo(Matrixf* mtx)
	    : TSimpleMtx2(mtx, PID_UjinkoHd_1, PID_UjinkoHd_2)
	{
	}

	virtual bool create(Arg*); // _08

	// _00     = VTBL
	// _00-_10 = TSimple2
};

//////////////////////////////////////
//// FALLING/ROLLING ROCK EFFECTS ////
//////////////////////////////////////

struct TRockWRun : public TBase {
	inline TRockWRun()
	    : mChasePos(&mPosition, PID_RockWRunChasePos_1, PID_RockWRunChasePos_2, PID_RockWRunChasePos_3)
	    , mIsActive(false)
	{
	}

	virtual bool create(Arg*) // _08 (weak)
	{
		mIsActive = true;
		return true;
	}
	virtual void forceKill() // _0C (weak)
	{
		mChasePos.forceKill();
		mIsActive = false;
	}
	virtual void fade() // _10 (weak)
	{
		mChasePos.fade();
		mIsActive = false;
	}

	// _00 VTBL
	TChasePos3 mChasePos; // _04
	f32 mSeaHeight;       // _44
	Vector3f mPosition;   // _48
	bool mIsActive;       // _54
};

/////////////////////////////////////////////
//// TANK (FIERY/WATERY BLOWHOG) EFFECTS ////
/////////////////////////////////////////////

struct TTankFireHit;
struct TTankWatHit;
struct TTankWat;

struct TParticleCallBack_TankFire : public JPAParticleCallBack {
	TParticleCallBack_TankFire()
	    : mMaxDistance(1000.0f)
	    , mEfxHit(nullptr)
	{
	}

	virtual ~TParticleCallBack_TankFire() { }                // _08 (weak)
	virtual void execute(JPABaseEmitter*, JPABaseParticle*); // _0C
	virtual void init(JPABaseEmitter*, JPABaseParticle*);    // _14

	// _00      = VTBL
	f32 mMaxDistance;      // _04
	TTankFireHit* mEfxHit; // _08
};

struct TTankFireHit : public TOneEmitterSimple {
	TTankFireHit()
	    : TOneEmitterSimple(PID_TankFireHit)
	{
	}

	virtual ~TTankFireHit() { } // _3C (weak)

	// _00      = VTBL
	// _00-_18  = TOneEmitterSimple
};

struct TTankFireABC : public TChaseMtx3 {
	inline TTankFireABC(Mtx mtx)
	    : TChaseMtx3(mtx, PID_TankFireABC_1, PID_TankFireABC_2, PID_TankFireABC_3)
	{
	}

	virtual bool create(Arg*); // _08
	virtual void forceKill()   // _0C (weak)
	{
		TChaseMtx3::forceKill();
		if (mParticleCallBack.mEfxHit != nullptr) {
			mParticleCallBack.mEfxHit->forceKill();
		}
	}
	virtual void fade() // _10 (weak)
	{
		TChaseMtx3::fade();
		if (mParticleCallBack.mEfxHit) {
			mParticleCallBack.mEfxHit->fade();
		}
	}
	virtual void startDemoDrawOff() // _14 (weak)
	{
		TChaseMtx3::startDemoDrawOff();
		mEfxFireHit.startDemoDrawOff();
	}
	virtual void endDemoDrawOn() // _18 (weak)
	{
		TChaseMtx3::endDemoDrawOn();
		mEfxFireHit.endDemoDrawOn();
	}

	// _00      = VTBL
	// _00-_40  = TChaseMtx3
	TParticleCallBack_TankFire mParticleCallBack; // _40
	TTankFireHit mEfxFireHit;                     // _4C
};

struct TTankFireIND : public TChaseMtx {
	TTankFireIND(Mtx mtx)
	    : TChaseMtx(PID_TankFireIND, (Matrixf*)mtx)
	{
	}

	virtual bool create(Arg*);  // _08
	virtual ~TTankFireIND() { } // _48 (weak)

	// _00      = VTBL
	// _00-_14  = TChaseMtx
	TParticleCallBack_TankFire mParticleCallBack;
};

struct TTankFire : public TBase {
	TTankFire(Mtx mtx)
	    : mEfxABC(mtx)
	    , mEfxIND(mtx)
	{
	}

	virtual bool create(Arg*); // _08
	virtual void forceKill()
	{
		mEfxABC.forceKill();
		mEfxIND.forceKill();
	} // _0C (weak)
	virtual void fade() // _10 (weak)
	{
		mEfxABC.fade();
		mEfxIND.fade();
	}

	// _00      = VTBL
	TTankFireABC mEfxABC; // _04
	TTankFireIND mEfxIND; // _6C
};

struct TTankFireYodare : public TChaseMtx {
	TTankFireYodare(Mtx mtx)
	    : TChaseMtx(PID_TankFireYodare, (Matrixf*)mtx)
	{
	}

	virtual ~TTankFireYodare() { } // _48 (weak)

	// _00      = VTBL
	// _00-_14  = TChaseMtx
};

struct TTankEffect {
	TTankEffect(Mtx mtx)
	    : mEfxFire(mtx)
	    , mEfxFireYodare(mtx)
	{
	}

	TTankFire mEfxFire;             // _00
	TTankFireYodare mEfxFireYodare; // _8C
};

struct TTankWatHit : public TOneEmitterSimple {
	TTankWatHit()
	    : TOneEmitterSimple(PID_TankWatHit)
	{
	}

	virtual ~TTankWatHit() { } // _3C (weak)

	// _00      = VTBL
	// _00-_18  = TOneEmitterSimple
};

struct TTankWatYodare : public TChaseMtx {
	TTankWatYodare(Mtx mtx)
	    : TChaseMtx(PID_TankWatYodare, (Matrixf*)mtx)
	{
	}

	virtual ~TTankWatYodare() { } // _48 (weak)

	// _00      = VTBL
	// _00-_14  = TChaseMtx
};

struct TTankWat : public TChaseMtx4 {
	inline TTankWat(Mtx mtx)
	    : TChaseMtx4(mtx, PID_TankWat_1, PID_TankWat_2, PID_TankWat_3, PID_TankWat_4)
	{
	}

	virtual bool create(Arg*); // _08
	virtual void forceKill()
	{
		TSyncGroup4::forceKill();
		if (mParticleCallBack.mEfxHit != nullptr) {
			mParticleCallBack.mEfxHit->forceKill();
		}
	} // _0C (weak)
	virtual void fade() // _10 (weak)
	{
		TChaseMtx4::fade();
		if (mParticleCallBack.mEfxHit) {
			mParticleCallBack.mEfxHit->fade();
		}
	}
	virtual void startDemoDrawOff() // _14 (weak)
	{
		TChaseMtx4::startDemoDrawOff();
		mEfxHit.startDemoDrawOff();
	}
	virtual void endDemoDrawOn() // _18 (weak)
	{
		TChaseMtx4::endDemoDrawOn();
		mEfxHit.endDemoDrawOn();
	}

	// _00      = VTBL
	// _00-_54  = TChaseMtx4
	TParticleCallBack_TankFire mParticleCallBack; // _54
	TTankWatHit mEfxHit;                          // _60
};

struct TWtankEffect {
	inline TWtankEffect(Mtx mtx)
	    : mEfxWat(mtx)
	    , mEfxWatYodare(mtx)
	{
	}

	TTankWat mEfxWat;             // _00
	TTankWatYodare mEfxWatYodare; // _8C
};

////////////////////////////////////////////
//// FIRECHAPPY (FIERY BULBLAX) EFFECTS ////
////////////////////////////////////////////

struct TYakiBody : public TChaseMtx4 {
	inline TYakiBody()
	    : TChaseMtx4(nullptr, PID_YakiBody_1, PID_YakiBody_2, PID_YakiBody_3, PID_YakiBody_4)
	{
	}

	void setRateLOD(int);

	// _00     = VTBL
	// _00-_54 = TChaseMtx4
};

}; // namespace efx

#endif
