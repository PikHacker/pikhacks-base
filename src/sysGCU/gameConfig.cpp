#include "string.h"
#include "System.h"
#include "Dolphin/print.h"
#include "Game/GameConfig.h"

namespace Game {

GameConfig gGameConfig;

/**
 * @note Address: 0x80432170
 * @note Size: 0x130
 */
GameConfig::GameConfig()
    : mParms()
{
	mParms.mAutosaveOff.mData          = 0;
	mParms.mGamePrint.mData            = 0;
	mParms.mPrint.mData                = 0;
	mParms.mOgawaPrint.mData           = 0;
	mParms.mShimizuPrint.mData         = 0;
	mParms.mYamashitaPrint.mData       = 0;
	mParms.mKandoPrint.mData           = 0;
	mParms.mNishimuraPrint.mData       = 0;
	mParms.mMorimuraPrint.mData        = 0;
	mParms.mEbisawaPrint.mData         = 0;
	mParms.mPsoundPrint.mData          = 0;
	mParms.mShortCutUp.mData           = -1;
	mParms.mShortCutDown.mData         = -1;
	mParms.mShortCutLeft.mData         = -1;
	mParms.mShortCutRight.mData        = -1;
	mParms.mAllocAllEnemy.mData        = 0;
	mParms.mAI.mData                   = 0;
	mParms.mE3version.mData            = 0;
	mParms.mTimers.mData               = 0;
	mParms.mMukki_cherry.mData         = 0;
	mParms.mMarioClubDevelop.mData     = 0;
	mParms.mPublicity.mData            = 1;
	mParms.mMapparts_path.mData        = "";
	mParms.mLanguage.mData             = "";
	mParms.mPelletMultiLang.mData      = 1;
	mParms.mVsTest.mData               = 0;
	mParms.mHeapFreeSize.mData         = 1;
	mParms.mKFesVersion.mData          = 0;
	mParms.mNintendoVersion.mData      = 0;
	mParms.mVsDeathType.mData          = 1;
	mParms.mVsHiba.mData               = 0;
	mParms.mVsY.mData                  = 0;
	mParms.mBaseGameNewCheck.mData     = 0;
	mParms.mVsDebugSelectPattern.mData = 0;
	mParms.mVsFifo.mData               = 0;
}

/**
 * @note Address: 0x80432770
 * @note Size: 0x128
 */
bool GameConfig::load(char*)
{
	sys->mRegion = System::LANG_English;
	return false;
}

} // namespace Game
