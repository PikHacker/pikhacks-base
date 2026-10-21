#ifndef _EFX_2DEFFECT_H
#define _EFX_2DEFFECT_H

#include "efx2d/TSimple.h"
#include "efx2d/TForever.h"
#include "efx2d/TChasePos.h"

namespace efx2d {

struct T2DCountKira : public TForever {
	T2DCountKira()
	    : TForever(PID_2DCountKira)
	{
		mScale = 1.0f;
	}
	virtual bool create(Arg*);  // _08
	virtual ~T2DCountKira() { } // _34 (weak)

	// _00     = VTBL
	// _00-_14 = TForever
	f32 mScale;
};

struct T2DCursor : public TChasePos {
	T2DCursor(Vector2f* pos)
	    : TChasePos(PID_2DCursor_1, pos)
	{
		mScale = 1.0f;
	}

	virtual bool create(Arg*); // _08
	virtual ~T2DCursor() { }   // _34 (weak)

	// _00     = VTBL
	// _00-_18 = TChasePos
	f32 mScale; // _18
};

struct T2DSensorAct_forVS : public TSimple2 {
	inline T2DSensorAct_forVS()
	    : TSimple2(PID_SensorAct_1, PID_SensorAct_2)
	{
	}

	bool create(Arg*);

	// _00     = VTBL
	// _00-_14 = TSimple2
};

struct T2DSensorGet_forVS : public TSimple2 {
	inline T2DSensorGet_forVS()
	    : TSimple2(PID_SensorGet_1, PID_SensorGet_2)
	{
	}

	virtual bool create(Arg*); // _08

	// _00     = VTBL
	// _00-_14 = TSimple2
};

struct T2DSprayset_forVS : public TSimple2 {
	inline T2DSprayset_forVS()
	    : TSimple2(PID_SpraySet_1, PID_SpraySet_2)
	{
	}

	virtual bool create(Arg*); // _08

	// _00     = VTBL
	// _00-_14 = TSimple2
};

}; // namespace efx2d
#endif
