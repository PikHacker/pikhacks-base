#include "Title.h"
#include "ebi/title/TTitle.h"
#include "og/ogLib2D.h"
#include "Pikmin2ARAM.h"
#include "Game/THPPlayer.h"
#include "Screen/Game2DMgr.h"
#include "JSystem/JFramework/JFWDisplay.h"
#include "GameFlow.h"
#include "Game/Data.h"
#include "Game/GameConfig.h"
#include "Game/MemoryCard/Mgr.h"
#include "og/newScreen/ogUtil.h"
#include "TParticle2dMgr.h"
#include "JSystem/JUtility/JUTProcBar.h"
#include "Dolphin/rand.h"
#include "Morimura/HiScore.h"
#include "ebi/E2DGraph.h"
#include "JSystem/J2D/J2DPrint.h"
#include "JSystem/JFramework/JFWSystem.h"

namespace {
static u8 sMovieIndex[7] = { 0, 2, 4, 1, 3, 11, 11 };
static s8 sSeasonIndex   = 255;
#if defined(VERSION_PAL)
static u16 sBuildInfoButtons[10]
    = { Controller::PRESS_A,          Controller::PRESS_B,         Controller::PRESS_X,         Controller::PRESS_R,
        Controller::PRESS_L,          Controller::PRESS_DPAD_LEFT, Controller::PRESS_DPAD_DOWN, Controller::PRESS_DPAD_UP,
        Controller::PRESS_DPAD_RIGHT, Controller::PRESS_Z };
#endif
} // namespace

namespace Title {

/**
 * @note Address: 0x8044A03C
 * @note Size: 0xF0
 */
Section::Section(JKRHeap* heap)
    : BaseHIOSection(heap)
    , mState(State_Init)
    , mGoToDemoTimer(0.0f)
    , mButtonCallback(nullptr)
{
	mMovieIndex     = -1;
	mButtonCallback = new Delegate<Section>(this, &loadResident);
#if defined(VERSION_PAL)
	mReloadMessageCallback = new Delegate<Section>(this, &reloadMessageResource);
#endif
	og::Lib2D::create();
	gPikmin2AramMgr->setLoadPermission(true);
	mDoCheckShortCut = false;
#if defined(VERSION_PAL)
	mDebugKeyIndex = 0;
	mShowBuildInfo = false;
#endif
}

/**
 * @note Address: 0x8044A460
 * @note Size: 0xA4
 */
Section::~Section()
{
	ebi::title::TTitleMgr::deleteInstance();
}

/**
 * @note Address: 0x8044A504
 * @note Size: 0xC4
 */
void Section::doExit()
{
	PSSystem::SceneMgr* mgr = PSSystem::getSceneMgr();
	mgr->deleteCurrentScene();
	mThpPlayer->stop();
	if (!Screen::gGame2DMgr->mScreenMgr->reset()) {
#if defined(VERSION_PAL)
		JUT_PANICLINE(577, "game2DMgr::reset error\n");
#elif defined(VERSION_JP)
		JUT_PANICLINE(497, "game2DMgr::reset error\n");
#elif defined(VERSION_US_DEMO1)
		JUT_PANICLINE(533, "game2DMgr::reset error\n");
#else
		JUT_PANICLINE(527, "game2DMgr::reset error\n");
#endif
	}
}

#if defined(VERSION_PAL)
/**
 * @note Address: 0x8044AFAC (PAL)
 * @note Size: 0x24
 * @note Fabricated name. Could be reloadMessage or loadMessageResource or something.
 */
void Section::reloadMessageResource()
{
	gP2JMEMgr->reloadMessageResource();
}
#endif

/**
 * @note Address: 0x8044A5C8
 * @note Size: 0x104
 */
void Section::loadResident()
{
	JKRHeap* backup = JKRGetCurrentHeap();

	if (!sys->isFlag(System::SF_LoadResident)) {
		sys->mSysHeap->becomeCurrentHeap();

		char* path      = "/user/Kando/piki/pikis.szs";
		JKRArchive* arc = JKRMountArchive(path, JKRArchive::EMM_Mem, nullptr, JKRArchive::EMD_Head);
#if defined(VERSION_PAL)
		JUT_ASSERTLINE(631, arc, "%s : mount failed !!\n", path);
#elif defined(VERSION_JP)
		JUT_ASSERTLINE(552, arc, "%s : mount failed !!\n", path);
#elif defined(VERSION_US_DEMO1)
		JUT_ASSERTLINE(588, arc, "%s : mount failed !!\n", path);
#else
		JUT_ASSERTLINE(582, arc, "%s : mount failed !!\n", path);
#endif

		path = "user/Kando/onyon/arc.szs";
		arc  = JKRMountArchive(path, JKRArchive::EMM_Mem, nullptr, JKRArchive::EMD_Head);
#if defined(VERSION_PAL)
		JUT_ASSERTLINE(639, arc, "%s : mount failed !!\n", path);
#elif defined(VERSION_JP)
		JUT_ASSERTLINE(560, arc, "%s : mount failed !!\n", path);
#elif defined(VERSION_US_DEMO1)
		JUT_ASSERTLINE(596, arc, "%s : mount failed !!\n", path);
#else
		JUT_ASSERTLINE(590, arc, "%s : mount failed !!\n", path);
#endif
		sys->setFlag(System::SF_LoadResident);
	}

	gPikmin2AramMgr->load();
	backup->becomeCurrentHeap();
}

/**
 * @note Address: 0x8044A6CC
 * @note Size: 0x434
 */
void Section::init()
{
	initHIO(new HIORootNode(this, "title")); // "Title Section"

	setDisplay(JFWDisplay::createManager(nullptr, mDisplayHeap, JUTXfb::DoubleBuffer, false), 1);

	mController1 = new Controller(JUTGamePad::PORT_0);

	sys->setFrameRate(1); // 60fps title screen

	PSM::ObjMgr::newInstance();

	mThpPlayer = new Game::THPPlayer;
	mThpPlayer->init(mDisplayHeap);
	addGenNode(mThpPlayer);
	Screen::Game2DMgr::create();
	Screen::gGame2DMgr->setGamePad(mController1);
	JUTProcBar::sManager->setVisible(false);
	JUTProcBar::sManager->setVisibleHeapBar(false);
	mTimeStep = 0.0f;
}

/**
 * @note Address: 0x8044AB00
 * @note Size: 0x2C
 */
void Section::menuCancel(Menu&)
{
	PSSystem::spSysIF->playSystemSe(PSSE_SY_MENU_CANCEL, 0);
}

/**
 * @note Address: 0x8044AB2C
 * @note Size: 0x54
 */
void Section::menuSelect(Menu& menu)
{
}

/**
 * @note Address: 0x8044AB80
 * @note Size: 0x110
 */
void Section::doDraw(Graphics& gfx)
{
	gfx.mOrthoGraph.setPort();
	switch (mState) {
	case State_Init:
	case State_MainTitle:
		mMainTitleMgr.draw();
		break;
	case State_Options:
		mOptionMgr.draw();
		break;
	case State_Bonus:
		if (mMovieIndex >= 0) {
			mThpPlayer->draw(gfx);
		}
		mOmakeMgr.draw();
		break;
	case State_ReloadMessages:
		break;
	case State_HiScore:
		Screen::gGame2DMgr->draw(gfx);
		Screen::gGame2DMgr->drawIndirect(gfx);
		break;
	}
	gfx.mPerspGraph.setPort();
	particle2dMgr->draw(1, 0);
	particle2dMgr->draw(0, 0);
#if defined(VERSION_PAL)
	if (mShowBuildInfo) {
		gfx.mOrthoGraph.setPort();
		J2DPrint print(JFWSystem::systemFont, 0.0f);
		print.initiate();
		print.mGlyphWidth  = 16.0f;
		print.mGlyphHeight = 16.0f;
		print.print(32.0f, 370.0f, "%s %s \n", "0721", _2F4C);
	}
#endif
}

/**
 * @note Address: N/A
 * @note Size: 0x200
 */
void Section::drawShortCuts(Graphics& gfx)
{
	// UNUSED FUNCTION
}

/**
 * @note Address: N/A
 * @note Size: 0x150
 */
void Section::drawShortCut(Graphics&, int p2, int, int, char*)
{
	// these are just here for JP data ordering - no clue what this used to be though.
#if defined(VERSION_JP)
	mTimeStep = 40.0f + p2;
#else
	mTimeStep = p2;
#endif

	// UNUSED FUNCTION
}

/**
 * @note Address: N/A
 * @note Size: 0x4
 */
void Section::drawDebugInfo(Graphics& gfx)
{
	// size indicates this function was entirely stubbed out before release
}

/**
 * @note Address: N/A
 * @note Size: 0x118
 */
void Section::updateMenu()
{
}

/**
 * @note Address: 0x8044AC90
 * @note Size: 0x688
 */
void Section::doUpdateMainTitle()
{
	mGoToDemoTimer += sys->mDeltaTime;
	updateMenu();
	mMainTitleMgr.update();
	if (mController1->isButtonHeld(~JUTGamePad::False)) {
		mGoToDemoTimer = 0.0f;
	}

	if (mController1->getButtonDown() & Controller::PRESS_Y) {
		OSReport("code size           %dKB\n", ((int)JKRHeap::getCodeEnd() - (int)JKRHeap::getCodeStart()) / 1024);
		OSReport("GameSystemHeap Free %dKB\n", (int)sys->mSysHeap->getTotalFreeSize() / 1024);
#if defined(VERSION_PAL)
		OSReport("SystemHeap free     %dKB\n", (int)JKRHeap::sSystemHeap->getTotalFreeSize() >> 10);
#endif
	}

	PSSystem::SceneMgr* mgr;
	PSSystem::SeqBase* seq;
	if (mMainTitleMgr.mDoEndBGM) {
		mgr = PSSystem::getSceneMgr();
		mgr->checkScene();
		PSSystem::SeqBase* seq = PSSystem::getSeqData(mgr, BGM_MainTheme);

		f32 rate = (ebi::TMainTitleMgr::kFadeOutTime / sys->mDeltaTime);
		rate     = ROUND_F32_TO_U8(rate);
		seq->stopSeq((int)rate);
	}

	if (mMainTitleMgr.isFinish()) {
		int id        = mMainTitleMgr.getSelectedMenu();
		mIsMainActive = false;
		switch (id) {
		case ebi::TMainTitleMgr::Select_Story:
			GameFlow::mActiveSectionFlag = GameFlow::SN_SingleGame;
			break;
		case ebi::TMainTitleMgr::Select_Challenge:
			GameFlow::mActiveSectionFlag = GameFlow::SN_ChallengeGame;
			break;
		case ebi::TMainTitleMgr::Select_Vs:
			GameFlow::mActiveSectionFlag = GameFlow::SN_VSGame;
			break;
		case ebi::TMainTitleMgr::Select_Options:
			mState = State_Options;
			mOptionMgr.start();
			mIsMainActive = true;
			mgr           = PSSystem::getSceneMgr();
			mgr->checkScene();
			seq = PSSystem::getSeqData(mgr, BGM_Options);
			seq->startSeq();
			mLanguageID = sys->mPlayData->mLanguage;
			break;
		case ebi::TMainTitleMgr::Select_HiScore:
			if (isFinishable()) {
				mState = State_HiScore;
				Morimura::DispMemberHighScore disp;
				disp.mImageArchive = mHiScoreTex;
				Screen::gGame2DMgr->open_HighScore(disp);
				mgr = PSSystem::getSceneMgr();
				mgr->checkScene();
				seq = PSSystem::getSeqData(mgr, BGM_HiScore);
				seq->startSeq();
			}
			mIsMainActive = true;
			break;
		case ebi::TMainTitleMgr::Select_Bonus:
			mState = State_Bonus;
			mOmakeMgr.start();
			mIsMainActive = true;
			mgr           = PSSystem::getSceneMgr();
			mgr->checkScene();
			seq = PSSystem::getSeqData(mgr, BGM_Bonus);
			seq->startSeq();
			break;
		case ebi::TMainTitleMgr::Select_NULL:
			mIsMainActive = true;
			break;
		default:
			GameFlow::mActiveSectionFlag = GameFlow::SN_MainTitle;
			break;
		}
	} else {
		if (mGoToDemoTimer > 43.0f) {
			if (!Game::gGameConfig.mParms.mKFesVersion.mData && !Game::gGameConfig.mParms.mNintendoVersion.mData) {
				mMainTitleMgr.forceQuit();
				mIsMainActive                = false;
				GameFlow::mActiveSectionFlag = GameFlow::SN_Demo;
			} else {
				mGoToDemoTimer = 0.0f;
			}
		}
	}
}

/**
 * @note Address: N/A
 * @note Size: 0x214
 */
void Section::doUpdateHiScore()
{
	Screen::gGame2DMgr->update();
	if (Screen::gGame2DMgr->isEndHighScore()) {

		PSSystem::SeqBase* seq = PSSystemGetSeqCheck(BGM_HiScore);
		seq->stopSeq(0);
		returnToMainTitle(ebi::TMainTitleMgr::Select_HiScore);
		Screen::gGame2DMgr->mScreenMgr->reset();
	}
}

/**
 * @note Address: 0x8044B318
 * @note Size: 0x4C8
 */
void Section::doUpdateOmake()
{
	PSSystem::SceneMgr* mgr;
	PSSystem::SeqBase* seq;

	mOmakeMgr.update();
	if (mOmakeMgr.mIsFinished) {
		mgr = PSSystem::getSceneMgr();
		PSSystem::SeqBase* seq = PSSystem::getSeqData(mgr, BGM_Bonus);
		f32 rate               = (ebi::E2DFader::kFadeTime / sys->mDeltaTime);
		rate                   = ROUND_F32_TO_U8(rate);
		seq->stopSeq((int)rate);
	}

	if (mOmakeMgr.isMovieState()) {
		if (mMovieIndex < 0) {
			mgr = PSSystem::getSceneMgr();
			PSSystem::SeqBase* seq = PSSystem::getSeqData(mgr, BGM_Bonus);
			seq->stopSeq(0);
			mMovieIndex = sMovieIndex[mOmakeMgr.getMovieID()];
			if (mMovieIndex < 0 || mMovieIndex >= 12) {
				mMovieIndex = Game::THPPlayer::PLAY_5;
			}
			mThpPlayer->load((Game::THPPlayer::EMovieIndex)mMovieIndex);
			mThpPlayer->pause();
#if defined(VERSION_PAL)
		} else if (isFinishable() && mThpPlayer->isFinishLoading()) {
#else
		} else if (mThpPlayer->isFinishLoading()) {
#endif
			mThpPlayer->play();
		}
		mThpPlayer->update();
		if (mThpPlayer->isFinishLoading()) {
			if (mThpPlayer->isFinishPlaying() || mController1->getButtonDown() & Controller::PRESS_B) {
				if (mController1->getButtonDown() & Controller::PRESS_B) {
					PSSystem::spSysIF->playSystemSe(PSSE_SY_MENU_CANCEL, 0);
				}
				mThpPlayer->pause();
				mThpPlayer->stop();
				mOmakeMgr.restartFromMovieState();
				mMovieIndex = -1;
				mgr         = PSSystem::getSceneMgr();
				mgr->checkScene();
				seq = PSSystem::getSeqData(mgr, BGM_Bonus);
				seq->startSeq();
			}
		}
	}

	if (mOmakeMgr.isFinish()) {
		returnToMainTitle(ebi::TMainTitleMgr::Select_Bonus);
	}
}

/**
 * @note Address: N/A
 * @note Size: 0x25C
 */
void Section::doUpdateOption()
{
	mOptionMgr.update();
#if defined(VERSION_PAL)
	if (mOptionMgr.getStateID() != 1) {
		sys->mPlayData->mFlags.set(Game::CommonSaveData::Mgr::SaveFlag_Language);
	}
#endif
	if (mOptionMgr.mIsFinished) {

		PSSystem::SeqBase* seq = PSSystemGetSeqCheck(BGM_Options);
		f32 rate               = (ebi::E2DFader::kFadeTime / sys->mDeltaTime);
		rate                   = ROUND_F32_TO_U8(rate);
		seq->stopSeq((int)rate);
	}
	if (mOptionMgr.isFinish()) {
#if defined(VERSION_PAL)
		sys->mPlayData->mFlags.unset(Game::CommonSaveData::Mgr::SaveFlag_Language);
		u8 lang = sys->mPlayData->mLanguage;
		if (mLanguageID != lang) {
			mState = State_ReloadMessages;
			gPikmin2AramMgr->freeAll();
			sys->dvdLoadUseCallBack(&mThreadCommand, mReloadMessageCallback);
		} else {
#endif
			returnToMainTitle(ebi::TMainTitleMgr::Select_Options);
#if defined(VERSION_PAL)
		}
#endif
	}
}

/**
 * @note Address: 0x8044B7E0
 * @note Size: 0x108
 */
void Section::run()
{
#if defined(VERSION_PAL)
	sys->getLanguage();
#endif
	if (!Game::gGameConfig.mParms.mNintendoVersion.mData && !Game::gGameConfig.mParms.mE3version.mData) {
#if defined(VERSION_PAL)
		sys->mCardMgr->loadGameOption(false);
#else
		sys->mCardMgr->loadGameOption();
#endif
	}

	bool dorun = true;
	while (dorun) {
		beginFrame();
		beginRender();
		endRender();
		sys->mCardMgr->update();
		if (sys->mCardMgr->isSaveInvalid()) {
			sys->mCardMgr->checkStatus();
			sys->mPlayData->setup();
			dorun = false;
		}
		endFrame();
	}
	::Section::run();
	mIsMainActive = false;
}

/**
 * @note Address: 0x8044B8E8
 * @note Size: 0x5B8
 */
bool Section::doUpdate()
{
	sys->mCardMgr->update();
	switch (mState) {
	case State_Init:
		mMainTitleMgr.start();
		mState = State_MainTitle;
		mMainTitleMgr.update();
		break;
	case State_MainTitle:
		doUpdateMainTitle();
		break;
	case State_Options:
		if (isFinishable()) {
			doUpdateOption();
		}
		break;
	case State_Bonus:
		doUpdateOmake();
		break;
	case State_HiScore:
		doUpdateHiScore();
		break;
	case State_ReloadMessages:
		if (sys->dvdLoadSyncNoBlock(&mThreadCommand)) {
#if defined(VERSION_PAL)
			mIsMainActive                = false;
			GameFlow::mActiveSectionFlag = GameFlow::SN_MainTitle;
#else
			returnToMainTitle(ebi::TMainTitleMgr::Select_Options);
#endif
		}
		break;
	}

	BaseHIOSection::doUpdate();
	particle2dMgr->update();
#if defined(VERSION_PAL)
	// build info button sequence??
	if (mController2->isButtonDown(Controller::PRESS_ANY)) {
		if (mController2->isButtonDown(sBuildInfoButtons[mDebugKeyIndex])) {
			if (++mDebugKeyIndex == 10) {
				mShowBuildInfo ^= 1;
			}
		} else {
			mDebugKeyIndex = 0;
		}
	}
#endif
	return mIsMainActive;
}

/**
 * @note Address: 0x8044BEA0
 * @note Size: 0x40
 */
bool Section::isFinishable()
{
	gPikmin2AramMgr->setLoadPermission(false);
	sys->dvdLoadSyncNoBlock(&mThreadCommand);
}

/**
 * @note Address: 0x8044BEE0
 * @note Size: 0x8C
 */
void Section::doLoadingStart()
{
	sys->dvdLoadUseCallBack(&mThreadCommand, new Delegate<Section>(this, loadResource));
}

/**
 * @note Address: 0x8044BF6C
 * @note Size: 0xEC
 */
bool Section::doLoading()
{
	bool done = sys->dvdLoadSyncNoBlock(&mThreadCommand);
	if (done) {
		sys->dvdLoadUseCallBack(&mThreadCommand, mButtonCallback);
		PSMGetSceneMgrCheck()->doStartMainSeq();
	}
	return !done;
}

/**
 * @note Address: 0x8044C058
 * @note Size: 0x3C8
 */
void Section::loadResource()
{
	createScreenRootNode();
	PSGame::SceneInfo info;
	info.mSceneType          = PSGame::SceneInfo::TITLE_SCREEN;
	info.mCameras            = 0;
	PSGame::PikSceneMgr* mgr = static_cast<PSGame::PikSceneMgr*>(PSSystem::getSceneMgr());
	mgr->newAndSetCurrentScene(info);

	PSSystem::SceneMgr* mgr2 = PSSystem::getSceneMgr();
	mgr2->checkScene();
	mgr2->mScenes->mChild->scene1stLoadSync();

	TParticle2dMgr::globalInstance();
	particle2dMgr->createHeap(TITLESCREEN_PARTICLE_HEAP_SIZE);
	particle2dMgr->createMgr("user/Ebisawa/effect/eff2d_game2d.jpc", 2000, 0x80, 0x80);

	int id;
	if (sSeasonIndex == -1) {
		id = randInt(12);
	} else {
		id = 12;
	}

	switch (id) {
	case 0:
		sSeasonIndex = ebi::title::TTitleMgr::LEVEL_Winter;
		break;
	case 1:
		sSeasonIndex = ebi::title::TTitleMgr::LEVEL_Winter;
		break;
	case 2:
		sSeasonIndex = ebi::title::TTitleMgr::LEVEL_Spring;
		break;
	case 3:
		sSeasonIndex = ebi::title::TTitleMgr::LEVEL_Spring;
		break;
	case 4:
		sSeasonIndex = ebi::title::TTitleMgr::LEVEL_Spring;
		break;
	case State_ReloadMessages:
		sSeasonIndex = ebi::title::TTitleMgr::LEVEL_Summer;
		break;
	case 6:
		sSeasonIndex = ebi::title::TTitleMgr::LEVEL_Summer;
		break;
	case 7:
		sSeasonIndex = ebi::title::TTitleMgr::LEVEL_Summer;
		break;
	case 8:
		sSeasonIndex = ebi::title::TTitleMgr::LEVEL_Autumn;
		break;
	case 9:
		sSeasonIndex = ebi::title::TTitleMgr::LEVEL_Autumn;
		break;
	case 10:
		sSeasonIndex = ebi::title::TTitleMgr::LEVEL_Autumn;
		break;
	case 11:
		sSeasonIndex = ebi::title::TTitleMgr::LEVEL_Winter;
		break;
	default:
		if ((s8)(++sSeasonIndex) > ebi::title::TTitleMgr::LEVEL_Winter) {
			sSeasonIndex = ebi::title::TTitleMgr::LEVEL_Spring;
		}
	}

	if (Game::gGameConfig.mParms.mKFesVersion.mData) {
		mMainTitleMgr.setMode(ebi::title::TTitleMgr::LEVEL_Summer);
	} else {
		mMainTitleMgr.setMode(sSeasonIndex);
	}
	mMainTitleMgr.loadResource();
	mMainTitleMgr.setController(mController1);

	mOptionMgr.loadResource();
	mOptionMgr.setController(mController1);

	mHiScoreTex = nullptr;

	const char* name = "res_hiscoreTexture.szs";
	char buf[52];
	og::newScreen::makeLanguageResName(buf, name);

	mHiScoreTex = JKRMountArchive(buf, JKRArchive::EMM_Mem, nullptr, JKRArchive::EMD_Head);
	JUT_ASSERTLINE(1700, mHiScoreTex, "arcName = %s\n", buf);
	mOmakeMgr.newCardEMgrAndTask();
	mOmakeMgr.loadResource();
	mOmakeMgr.setControllers(mController1);
}

} // namespace Title
