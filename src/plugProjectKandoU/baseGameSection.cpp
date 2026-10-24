#include "Game/Navi.h"
#include "Game/PikiMgr.h"
#include "Game/Entities/ItemBigFountain.h"
#include "Game/Entities/PelletOtakara.h"
#include "Game/Entities/PelletCarcass.h"
#include "Game/Entities/PelletNumber.h"
#include "Game/Entities/ItemPikihead.h"
#include "Game/Entities/PelletFruit.h"
#include "Game/Entities/PelletItem.h"
#include "Game/PelletBirthBuffer.h"
#include "Game/Cave/RandMapUnit.h"
#include "Game/BaseGameSection.h"
#include "Game/generalEnemyMgr.h"
#include "Game/MoviePlayer.h"
#include "Game/AIConstants.h"
#include "Game/PikiState.h"
#include "Game/GameLight.h"
#include "Game/CameraMgr.h"
#include "Game/DeathMgr.h"
#include "Game/rumble.h"
#include "Game/Farm.h"

#include "JSystem/JFramework/JFWDisplay.h"
#include "JSystem/J2D/J2DPrint.h"
#include "Screen/Game2DMgr.h"
#include "Sys/DrawBuffers.h"
#include "TParticle2dMgr.h"
#include "PSGame/Global.h"
#include "PSM/BossBgmFader.h"
#include "efx/efxObject.h"
#include "Dolphin/rand.h"
#include "LifeGaugeMgr.h"
#include "og/ogLib2D.h"
#include "utilityU.h"
#include "PikiAI.h"

static Delegate1<Game::BaseGameSection, Game::CameraArg*>* cameraMgrCallback;
static JKRExpHeap* theExpHeap;

namespace Game {

u8 BaseGameSection::sOptDraw = 3;

/**
 * @note Address: 0x8014ADA0
 * @note Size: 0x21C
 */
BaseGameSection::BaseGameSection(JKRHeap* heap)
    : BaseHIOSection(heap)
{
	mXfbFlags = 0;
	setDisplay(JFWDisplay::createManager(nullptr, mDisplayHeap, JUTXfb::DoubleBuffer, true), 1);
	mPlayerMode         = 2;
	mDraw2DCreature     = nullptr;
	mTreasureZoomCamera = nullptr;
	mKanteiDelegate     = new Delegate1<BaseGameSection, Rectf&>(this, &BaseGameSection::onKanteiDone);
	cameraMgrCallback   = new Delegate1<BaseGameSection, CameraArg*>(this, &BaseGameSection::onCameraBlendFinished);
	mBlendCamera        = nullptr;
	Game::cameraMgr     = nullptr;
	Game::rumbleMgr     = nullptr;
	Game::shadowMgr     = nullptr;
	lifeGaugeMgr        = nullptr;
	carryInfoMgr        = nullptr;
	mLightMgr           = nullptr;
	mSplitter           = nullptr;
	mTheExpHeap         = nullptr;
	theExpHeap          = nullptr;
	mBackupHeap         = nullptr;
	mXfbTexture2d       = nullptr;
	mFbTexture          = nullptr;
	mXfbImage           = nullptr;
	mXfbBoundsX         = 0;
	mXfbBoundsY         = 0;
	mXfbBounds2dY       = 0;
	mXfbBounds2dX       = 0;
	mBlackFader         = new BlackFader;
	mWipeInFader        = new WipeInFader;
	mWipeOutFader       = new WipeOutFader;
	mWipeOutInFader     = new WipeOutInFader;
}

/**
 * @note Address: 0x8014B0FC
 * @note Size: 0x74
 */
void BaseGameSection::useSpecificFBTexture(JUTTexture* texture)
{
#if defined(VERSION_PAL)
	JUT_ASSERTLINE(1526, !mFbTexture, "２回は無理ｗ\n"); // 'it's impossible to do twice lol'
#elif defined(VERSION_JP)
	JUT_ASSERTLINE(1518, !mFbTexture, "２回は無理ｗ\n"); // 'it's impossible to do twice lol'
#else
	JUT_ASSERTLINE(1523, !mFbTexture, "２回は無理ｗ\n"); // 'it's impossible to do twice lol'
#endif
	mFbTexture                    = mXfbImage;
	mXfbImage                     = texture;
	Game::gameSystem->mXfbTexture = mXfbImage;
}

/**
 * @note Address: 0x8014B170
 * @note Size: 0x6C
 */
void BaseGameSection::restoreFBTexture()
{
#if defined(VERSION_PAL)
	JUT_ASSERTLINE(1536, mFbTexture, "useSpecificFBTexture してないｗ\n"); // 'i haven't used useSpecificFBTexture lol'
#elif defined(VERSION_JP)
	JUT_ASSERTLINE(1528, mFbTexture, "useSpecificFBTexture してないｗ\n"); // 'i haven't used useSpecificFBTexture lol'
#else
	JUT_ASSERTLINE(1533, mFbTexture, "useSpecificFBTexture してないｗ\n"); // 'i haven't used useSpecificFBTexture lol'
#endif
	mXfbImage                     = mFbTexture;
	mFbTexture                    = nullptr;
	Game::gameSystem->mXfbTexture = mXfbImage;
}

/**
 * @note Address: 0x8014B1DC
 * @note Size: 0x114
 */
BaseGameSection::~BaseGameSection()
{
	theExpHeap                   = nullptr;
	PSSystem::SceneMgr* sceneMgr = PSSystem::getSceneMgr();
	sceneMgr->deleteCurrentScene();
	TParticle2dMgr::deleteInstance();
	particleMgr->deleteInstance_TPkEffectMgr();
	ParticleMgr::deleteInstance();

	itemMgr->clearGlobalPointers();

	PelletOtakara::mgr = nullptr;
	PelletFruit::mgr   = nullptr;
	PelletItem::mgr    = nullptr;
	PelletNumber::mgr  = nullptr;
	PelletCarcass::mgr = nullptr;
}

/**
 * @note Address: 0x8014B2F0
 * @note Size: 0x50
 */
void BaseGameSection::loadSync(IDelegate* delegate, bool p2)
{
	sys->dvdLoadUseCallBack(&mDvdThreadCommand, delegate);
	waitSyncLoad(p2);
}

/**
 * @note Address: 0x8014B340
 * @note Size: 0x120
 */

u32 BaseGameSection::waitSyncLoad(bool allowPause)
{
	static int col = 0;
	col++;
	endFrame();

	if (!allowPause) {
		gameSystem->setPause(true, 3);
	}

	while (true) {
		beginFrame();
		beginRender();

		j3dSys.drawInit();
		GXSetViewport(0.0f, 0.0f, SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f, 1.0f);
		GXSetScissor(0, 0x10, SCREEN_SCISSOR_WIDTH, SCREEN_SCISSOR_HEIGHT);
		endRender();

		if (mDvdThreadCommand.mMode != DvdThreadCommand::CM_Completed) {
			// Wait for the DVD thread to finish
		} else if (!allowPause) {
			gameSystem->setPause(false, 3);
			return;
		} else {
			break;
		}

		endFrame();
	}
}

/**
 * @note Address: 0x8014B460
 * @note Size: 0x50
 */
void BaseGameSection::dvdloadGameSystem()
{
	GameSystem* game = new GameSystem(this);
	Game::gameSystem = game;
	game->init();
}

/**
 * @note Address: 0x8014B4B0
 * @note Size: 0x390
 */
void BaseGameSection::init()
{
	mXfbFlags       = 0;
	mMoney          = 0;
	mDraw2DCreature = nullptr;

	sys->enableCPULockDetector(480);
	Delegate<BaseGameSection> delegate(this, dvdloadGameSystem);
	beginFrame();
	beginRender();
	j3dSys.drawInit();
	GXSetViewport(0.0f, 0.0f, 608.0f, 480.0f, 0.0f, 1.0f);
	GXSetScissor(0, 0x10, 0x260, 0x1c0);
	mGraphics->mOrthoGraph.setPort();
	endRender();
	sys->dvdLoadUseCallBack(&mDvdThreadCommand, &delegate);

	waitSyncLoad(true);

	BaseHIOSection::initHIO(nullptr);

	mTreasureLightMgr    = new TreasureLight::Mgr;
	moviePlayer          = new MoviePlayer;
	mMovieFinishCallback = new Delegate3<BaseGameSection, MovieConfig*, u32, u32>(this, movieDone);
	mMovieStartCallback  = new Delegate3<BaseGameSection, MovieConfig*, u32, u32>(this, onMovieStart);

	sys->setFrameRate(2);
	initJ3D();
	mapMgr = nullptr;

	og::Lib2D::create();
	Screen::Game2DMgr::create();
	static_cast<newScreen::Mgr*>(Screen::gGame2DMgr->mScreenMgr)->mInitialised = 1;
	mXfbBoundsY                                                                = 0;
	mXfbBoundsX                                                                = 0;
	onInit();
	mTreasureGetState = 0;
}

/**
 * @note Address: 0x8014B844
 * @note Size: 0x34
 */
void BaseGameSection::drawInit(Graphics& gfx, Section::EDrawInitMode mode)
{
	if (mode == Two) {
		section_fadeout();
	}
}

/**
 * @note Address: 0x8014B87C
 * @note Size: 0x3A8
 */
bool BaseGameSection::doUpdate()
{
	SysShape::Model::cullCount = 0;
	gameSystem->startFrame();
	Screen::gGame2DMgr->update();
	if (mIsBlendCameraActive) {
		updateBlendCamera();
	}
	mapMgr->update();
	doAnimation();
	doEntry();

	if (rumbleMgr) {
		rumbleMgr->update();
	}
	if (shadowMgr) {
		shadowMgr->update();
	}
	if (lifeGaugeMgr) {
		lifeGaugeMgr->update();
	}
	if (carryInfoMgr) {
		carryInfoMgr->update();
	}
	if (mLightMgr) {
		mLightMgr->update();
	}
	SysShape::Model::setViewCalcModeInd();
	for (int vpIdx = 0; vpIdx < sys->mGfx->mActiveViewports; vpIdx++) {
		Viewport* viewport = sys->mGfx->getViewport(vpIdx);
		if (viewport && viewport->viewable()) {
			j3dSetView(viewport, false);
		}
	}
	if (platMgr) {
		platMgr->resetOnCount();
	}

	if (!gameSystem->paused()) {
		f32 frameRate = sys->getFrameRate(1.0f);
		if (!(gameSystem->isFlag(GAMESYS_DisableCollision))) {
			sys->getTime();
			cellMgr->resolveCollision();
			CellPyramid::sSpeedUpResolveColl = true;
		}
		doSimulation(frameRate);
	}

	if (!gameSystem->mIsFrozen && !gameSystem->paused() && particleMgr) {
		particleMgr->update();
	}
	if (particle2dMgr) {
		particle2dMgr->update();
	}
	onUpdate();
	if (moviePlayer && !gameSystem->mIsMoviePause) {
		if (gameSystem->isMultiplayerMode()) {
			moviePlayer->update(mControllerP1, mControllerP2);
		} else {
			moviePlayer->update(mControllerP1, nullptr);
		}
	}
	if (shadowMgr) {
		shadowMgr->init();
	}
	gameSystem->endFrame();
	return mIsMainActive;
}

/**
 * @note Address: 0x8014BC28
 * @note Size: 0x170
 */
void BaseGameSection::doDraw(Graphics& gfx)
{
	captureRadarmap(gfx);
	if (gameSystem->paused()) {
		if (cameraMgr) {
			cameraMgr->controllerLock(CAMNAVI_Both);
			cameraMgr->update();
			cameraMgr->controllerUnLock(CAMNAVI_Both);
		}

	} else if (cameraMgr) {
		cameraMgr->update();
	}

	draw3D(gfx);
	if (moviePlayer && !gameSystem->mIsMoviePause) {
		moviePlayer->drawLoading(gfx);
	}
	pre2dDraw(gfx);
	gfx.setToken("2d");
	draw2D(gfx);
	if (mDraw2DCreature) {
		drawOtakaraWindow(gfx);
	}
	Screen::gGame2DMgr->drawKanteiMsg(gfx);
	if (moviePlayer && !gameSystem->mIsMoviePause) {
		moviePlayer->draw(gfx);
	}
}

/**
 * @note Address: 0x8014BD9C
 * @note Size: 0x78
 */
void BaseGameSection::movieDone(MovieConfig* config, u32 id1, u32 id2)
{
	onMovieDone(config, id1, id2);
	if (mDraw2DCreature) {
		mDraw2DCreature->movie_end(true);
		mDraw2DCreature->kill(nullptr);
		mDraw2DCreature = nullptr;
		Screen::gGame2DMgr->close_SpecialItem();
		Screen::gGame2DMgr->close_Kantei();
		mTreasureGetState = 0;
	}
}

/**
 * @note Address: 0x8014BE18
 * @note Size: 0x8C
 */
void BaseGameSection::onMovieCommand(int cmd)
{
	switch (cmd) {
	case 0:
		break;
	case 2:
		if (moviePlayer && !(moviePlayer->isFlag(MVP_IsFinished))) {
			createFallPikminSound();
		}
		break;
	case 3:
		if (gameSystem->isStoryMode() && gameSystem->mTimeMgr->mDayCount == 0) {
			pikiMgr->forceEnterPikmins(false);
		}
		break;
	}
}

/**
 * @note Address: 0x8014BEA4
 * @note Size: 0x450
 */

// unfortunately this probably isn't real inline, however I'm not writing the full code out bc that's stupid
inline void j3dStuff(Sys::DrawBuffers*& buffer, Sys::DrawBuffer::CreateArg& drawArg, bool doFog)
{

	drawArg.mSize = 0x80;
	drawArg.mName = "normal";
	buffer->get(DB_NormalLayer)->create(drawArg);

	drawArg.mSize = 1;
	drawArg.mName = (doFog) ? "normal-fogoff" : "normal";
	buffer->get(DB_NormalFogOffLayer)->create(drawArg);

	drawArg.mSize = 1;
	drawArg.mName = "map";
	if (doFog) {
		drawArg.mSortType = J3DDrawBuffer::J3DSORT_NonSort;
	}
	buffer->get(DB_MapLayer)->create(drawArg);

	drawArg.mSize = 1;
	drawArg.mName = "piki";
	if (doFog) {
		drawArg.mSortType = J3DDrawBuffer::J3DSORT_Mat;
	}
	buffer->get(DB_PikiLayer)->create(drawArg);

	drawArg.mSize = 1;
	drawArg.mName = "post";
	buffer->get(DB_PostRenderLayer)->create(drawArg);

	drawArg.mSize = 1;
	drawArg.mName = "2d";
	buffer->get(DB_2DLayer)->create(drawArg);

	drawArg.mSize = 1;
	drawArg.mName = "first";
	buffer->get(DB_FirstLayer)->create(drawArg);

	drawArg.mSize = 1;
	drawArg.mName = "postshadow";
	buffer->get(DB_PostShadowLayer)->create(drawArg);

	drawArg.mSize = 1;
	drawArg.mName = "objectlast";
	buffer->get(DB_ObjectLastLayer)->create(drawArg);

	drawArg.mSize = 1;
	drawArg.mName = "farm";
	buffer->get(DB_FarmLayer)->create(drawArg);
}

void BaseGameSection::initJ3D()
{
	mOpaqueDrawBuffer      = new Sys::DrawBuffers;
	mTransparentDrawBuffer = new Sys::DrawBuffers;

	mOpaqueDrawBuffer->allocate(10);
	mOpaqueDrawBuffer->mName = "OPA";
	{
		Sys::DrawBuffer::CreateArg drawArg;
		drawArg.mSortType = J3DDrawBuffer::J3DSORT_Mat;
		drawArg.mDrawType = J3DDrawBuffer::J3DDRAW_Head;
		j3dStuff(mOpaqueDrawBuffer, drawArg, true);
	}

	mTransparentDrawBuffer->allocate(10);
	mTransparentDrawBuffer->mName = "XLU";

	{
		Sys::DrawBuffer::CreateArg drawArg;
		drawArg.mSortType = J3DDrawBuffer::J3DSORT_Mat;
		drawArg.mDrawType = J3DDrawBuffer::J3DDRAW_Head;
		drawArg.mFlags.set(Sys::DrawBuffer::DRAWBUFF_Unk1);
		j3dStuff(mTransparentDrawBuffer, drawArg, false);
	}

	addGenNode(mOpaqueDrawBuffer);
	addGenNode(mTransparentDrawBuffer);

	j3dSys.setDrawBuffer(mOpaqueDrawBuffer->get(DB_NormalLayer)->mBuffer, J3DSys::SYSDRAW_Opa);
	j3dSys.setDrawBuffer(mTransparentDrawBuffer->get(DB_NormalLayer)->mBuffer, J3DSys::SYSDRAW_Xlu);
}

/**
 * @note Address: 0x8014C2F4
 * @note Size: 0x34
 */
void BaseGameSection::initResources()
{
	setupFixMemory();
	setupFloatMemory();
}

/**
 * @note Address: 0x8014C328
 * @note Size: 0x1E4
 */
void BaseGameSection::initViewports(Graphics& gfx)
{
	mSplitter = new HorizonalSplitter(&gfx);
	setSplitter(false);

	gfx.getViewport(PLAYER1_VIEWPORT)->setCamera(mOlimarCamera);
	gfx.getViewport(PLAYER2_VIEWPORT)->setCamera(mLouieCamera);

	shadowMgr->setViewport(gfx.getViewport(PLAYER1_VIEWPORT), 0);
	shadowMgr->setViewport(gfx.getViewport(PLAYER2_VIEWPORT), 1);

	cameraMgr->setViewport(gfx.getViewport(PLAYER1_VIEWPORT), CAMNAVI_Olimar);
	cameraMgr->setViewport(gfx.getViewport(PLAYER2_VIEWPORT), CAMNAVI_Louie);

	cameraMgr->init(CAMNAVI_Olimar);
	mTreasureZoomCamera         = new ZoomCamera;
	mTreasureGetViewport        = new Viewport;
	mTreasureGetViewport->mVpId = 2;

	u16 x = sys->getRenderModeObj()->fbWidth;
	u16 y = sys->getRenderModeObj()->efbHeight;
	sys->getRenderModeObj();
	sys->getRenderModeObj();

	Vector2f origin(0.0f, 0.0f);
	origin.y -= 80.0f;
	Rectf rect(origin.x, origin.y, origin.x + x, origin.y + y);
	mTreasureGetViewport->setRect(rect);
	mTreasureGetViewport->setCamera(mTreasureZoomCamera);
}

#define LoadTextFile(x) JKRDvdToMainRam(x, nullptr, Switch_0, 0, nullptr, JKRDvdRipper::ALLOC_DIR_BOTTOM, 0, nullptr, nullptr)

/**
 * @note Address: 0x8014C5CC
 * @note Size: 0x1120
 */
void BaseGameSection::initGenerators()
{

	generatorCache->clearGeneratorList();
	Generator::initialiseSystem();

	{ // Init Generator Managers
		generatorMgr        = new GeneratorMgr;
		generatorMgr->mName = "Generator(Default)";
		addGenNode(generatorMgr);

		onceGeneratorMgr        = new GeneratorMgr;
		onceGeneratorMgr->mName = "Generator(Init)";
		addGenNode(onceGeneratorMgr);

		limitGeneratorMgr        = new GeneratorMgr;
		limitGeneratorMgr->mName = "Generator(Limit)";
		addGenNode(limitGeneratorMgr);

		plantsGeneratorMgr        = new GeneratorMgr;
		plantsGeneratorMgr->mName = "Generator(植物)";
		addGenNode(plantsGeneratorMgr);

		dayGeneratorMgr        = new GeneratorMgr;
		dayGeneratorMgr->mName = "Generator(DAY)";
		addGenNode(dayGeneratorMgr);

		GenObjectEnemy::initialise();
		GenItem::initialise();
		GenPellet::initialise();
		GenObjectPiki::initialise();
		GenObjectNavi::initialise();
	}

	// load courseinfo
	if (mapMgr->mCourseInfo) {

		PelletBirthBuffer::clear();
		generatorCache->loadGenerators(mapMgr->mCourseInfo->mCourseIndex);
		generatorCache->updateUseList();

		int fileIdx = 0;
		void* file;

		char filenameCharArr[PATH_MAX];
		void* generatorFiles[64];
		GeneratorMgr* generatorManagers[64];

#pragma region defaultgen

		sprintf(filenameCharArr, "%s/defaultgen.txt", mapMgr->mCourseInfo->mAbeFolder);

		file = LoadTextFile(filenameCharArr);

		if (file) {
			RamStream defaultGenTxt(file, -1);
			defaultGenTxt.setMode(STREAM_MODE_TEXT, 1);
			generatorMgr->read(defaultGenTxt, false);
			generatorMgr->updateUseList();

			generatorFiles[0]    = file;
			generatorManagers[0] = generatorMgr;
			fileIdx++;
		}

#pragma endregion

#pragma region plantsgen

		sprintf(filenameCharArr, "/%s/plantsgen.txt", mapMgr->mCourseInfo->mAbeFolder);
		int entrynum = DVDConvertPathToEntrynum(filenameCharArr);

		if (entrynum != -1) {
			file = LoadTextFile(filenameCharArr);
			if (file) {
				RamStream plantsGenTxt(file, -1);
				plantsGenTxt.setMode(STREAM_MODE_TEXT, 1);
				plantsGeneratorMgr->read(plantsGenTxt, false);
				plantsGeneratorMgr->updateUseList();
				generatorFiles[fileIdx]    = file;
				generatorManagers[fileIdx] = plantsGeneratorMgr;
				fileIdx++;
			}
		}

#pragma endregion

#pragma region initgen

		CourseInfo* courseInfo = mapMgr->mCourseInfo;

		bool firstVisit = playData->courseVisited(courseInfo->mCourseIndex);

		if (!firstVisit) {
			playData->visitCourse(courseInfo->mCourseIndex);
			sprintf(filenameCharArr, "%s/initgen.txt", courseInfo->mAbeFolder);
			file = LoadTextFile(filenameCharArr);
			if (file) {
				RamStream initgenTxt(file, -1);
				initgenTxt.setMode(STREAM_MODE_TEXT, 1);
				onceGeneratorMgr->read(initgenTxt, false);
				onceGeneratorMgr->updateUseList();
				generatorFiles[fileIdx]    = file;
				generatorManagers[fileIdx] = onceGeneratorMgr;
				fileIdx++;
			}
		}

#pragma endregion
		void** fileSlot;
		GeneratorMgr** managerSlot;
		int floorDay;
		int today = gameSystem->mTimeMgr->mDayCount;

		{ // nonloop

			for (int i = 0; i < courseInfo->mLimitGenInfo.mCount; i++) {
				LimitGen* currentGen = static_cast<LimitGen*>(courseInfo->mLimitGenInfo.mOwner.getChildAt(i));

				if (currentGen->mMinimumDay > today || today > currentGen->mMaximumDay)
					continue;
				if (playData->mLimitGen[courseInfo->mCourseIndex].mNonLoops.isFlag(i))
					continue;

				sprintf(filenameCharArr, "%s/nonloop/%s", courseInfo->mAbeFolder, currentGen->mName);

				file = LoadTextFile(filenameCharArr);

				if (file) {
					RamStream nonloopTxt(file, -1);
					nonloopTxt.setMode(STREAM_MODE_TEXT, 1);

					GeneratorMgr* currentNonloopMgr = new GeneratorMgr;

					currentNonloopMgr->read(nonloopTxt, false);
					currentNonloopMgr->setDayLimit(currentGen->mDayLimit);
					currentNonloopMgr->updateUseList();

					generatorFiles[fileIdx]    = file;
					generatorManagers[fileIdx] = currentNonloopMgr;
					fileIdx++;

					limitGeneratorMgr->addMgr(currentNonloopMgr);
					playData->mLimitGen[courseInfo->mCourseIndex].mNonLoops.setFlag(i);
				}
			}

		} // end nonloop
		{ // loop

			int day = gameSystem->mTimeMgr->mDayCount;

			today %= 30;

			int intervalIterations = day / 30;

			// effectively day > 30
			if (intervalIterations >= 1) {

				fileSlot    = generatorFiles + fileIdx;
				managerSlot = generatorManagers + fileIdx;
				floorDay    = intervalIterations * 30;

				for (int i = 0; i < courseInfo->mLoopGenInfo.mCount; i++) {
					LimitGen* currentGen = static_cast<LimitGen*>(courseInfo->mLoopGenInfo.mOwner.getChildAt(i));

					int intervalMin = currentGen->mMinimumDay % 30;
					int intervalMax = currentGen->mMaximumDay % 30;

					if (intervalMin > today || today > intervalMax)
						continue;

					bool loopLoaded = playData->mLimitGen[courseInfo->mCourseIndex].mLoops.isFlag(i);

					if (loopLoaded)
						continue;

					sprintf(filenameCharArr, "%s/loop/%s", courseInfo->mAbeFolder, currentGen->mName);
					file = LoadTextFile(filenameCharArr);
					if (file) {
						RamStream loopTxt(file, -1);
						loopTxt.setMode(STREAM_MODE_TEXT, 1);

						GeneratorMgr* currentLoopMgr = new GeneratorMgr;

						currentLoopMgr->read(loopTxt, false);
						u32 dayLimit = currentGen->mDayLimit - 30;
						dayLimit += floorDay;
						currentLoopMgr->setDayLimit(s32(dayLimit));
						currentLoopMgr->updateUseList();

						*fileSlot++    = file;
						*managerSlot++ = currentLoopMgr;
						fileIdx++;
						limitGeneratorMgr->addMgr(currentLoopMgr);

						playData->mLimitGen[courseInfo->mCourseIndex].mLoops.setFlag(i);
					}
				}
			}

		} // end loop
		{ // weird day ujidani thing
			int today = gameSystem->mTimeMgr->mDayCount;
			sprintf(filenameCharArr, "%s/day/%d.txt", courseInfo->mAbeFolder, today % 30);
			int fileNum = DVDConvertPathToEntrynum(filenameCharArr);
			if (fileNum != -1) {
				file = LoadTextFile(filenameCharArr);
				if (file) {
					RamStream dayTxt(file, -1);
					dayTxt.setMode(STREAM_MODE_TEXT, 1);
					dayGeneratorMgr->read(dayTxt, false);
					dayGeneratorMgr->updateUseList();
					generatorFiles[fileIdx]    = file;
					generatorManagers[fileIdx] = dayGeneratorMgr;
					fileIdx++;
				}
			}
		}

		generalEnemyMgr->allocateEnemys(mPlayerMode, -1);
		generalEnemyMgr->setupSoundViewerAndBas();
		pelletMgr->setupResources();

		// cleanup file ptrs that have been left lying around
		for (int i = fileIdx - 1; i >= 0; i--) {
			delete[] generatorFiles[i];
		}

		for (int i = 0; i < fileIdx; i++) {
			generatorManagers[i]->generate();
		}

		generatorCache->createNumberGenerators();
		generatorCache->loadCreatures(mapMgr->mCourseInfo->mCourseIndex);
	}

	PelletBirthBuffer::birthAll();
	int naviCount = naviMgr->mActiveCount;
	switch (naviCount) {
	case 0: {
		Navi* olimar;
		Navi* louie;
		bool olimarAlive = false;
		Vector3f position;

		f32 mapRotation = mapMgr->getMapRotation();
		position        = Vector3f(-40.0f, 0.0f, 2.0f);
		if (gameSystem->isVersusMode()) {
			Onyon* redOnyon = ItemOnyon::mgr->getOnyon(Red);
			P2ASSERTLINE(2739, redOnyon);
			position = redOnyon->getPosition();
		} else {
			if (!mapMgr->getDemoMatrix()) {
				mapMgr->getStartPosition(position, 0);
				position.y = mapMgr->getMinY(position) + 8.5f;
				position.x += -4.526f;
				position.z += 7.453f;
			} else {
				position   = mapMgr->getDemoMatrix()->mtxMult(position);
				position.y = mapMgr->getMinY(position);
			}
		}
		olimar = naviMgr->birth();
		olimar->init(nullptr);
		olimar->mFaceDir = roundAng(mapRotation);
		olimar->setCamera(mOlimarCamera);
		olimar->setController(mControllerP1);
		olimar->setPosition(position, false);

		if (playData->mDeadNaviID.typeView & 1) {

			olimar->setDeadLaydown();
			olimarAlive = true;
		} else {
			olimar->mHealth = playData->mNaviLifeMax[0];
		}

		mapRotation = mapMgr->getMapRotation();
		position    = Vector3f(-60.0f, 0.0f, -10.0f);
		if (gameSystem->isVersusMode()) {
			Onyon* blueOnyon = ItemOnyon::mgr->getOnyon(Blue);
			P2ASSERTLINE(2791, blueOnyon);
			position = blueOnyon->getPosition();
		} else {
			if (!mapMgr->getDemoMatrix()) {
				mapMgr->getStartPosition(position, 0);
				position.y = mapMgr->getMinY(position) + 8.5f;
				position.x += 18.082f;
				position.z += -11.482f;
			} else {
				position   = mapMgr->getDemoMatrix()->mtxMult(position);
				position.y = mapMgr->getMinY(position);
			}
		}
		louie = naviMgr->birth();
		louie->init(nullptr);

		louie->setCamera(mLouieCamera);

		louie->setController(mControllerP2);
		louie->mFaceDir = roundAng(mapRotation);
		louie->setPosition(position, false);
		if (!(playData->mDeadNaviID.typeView >> 1 & 1)) {
			louie->mHealth = playData->mNaviLifeMax[1];
		}
		if (playData->mDeadNaviID.typeView & 2) {
			louie->setDeadLaydown();
			return;
		}
		if (!gameSystem->isMultiplayerMode() && !olimarAlive) {
			InteractFue callNavi(olimar, false, true); // don't combine parties, is new to party
			louie->stimulate(callNavi);
		}
		break;
	}

	case 2: {
		Navi* olimar = naviMgr->getAt(NAVIID_Olimar);
		olimar->setCamera(mOlimarCamera);
		olimar->setController(mControllerP1);
		Navi* louie = naviMgr->getAt(NAVIID_Louie);
		louie->setCamera(mLouieCamera);
		louie->setController(mControllerP2);
		break;
	}
	}
}

void BaseGameSection::advanceDayCount()
{
	int newDayCount = gameSystem->mTimeMgr->mDayCount + 1;
	if (newDayCount % 30 == 0) {
		for (int i = 0; i < 4; i++) {
			playData->mLimitGen[i].mLoops.all_zero();
		}
	}
	gameSystem->mTimeMgr->mDayCount = newDayCount;
}

void BaseGameSection::saveToGeneratorCache(CourseInfo* courseinfo)
{
#if defined(VERSION_PAL)
	P2ASSERTLINE(2926, courseinfo);
#elif defined(VERSION_JP)
	P2ASSERTLINE(2918, courseinfo);
#else
	P2ASSERTLINE(2923, courseinfo);
#endif
	generatorCache->beginSave(courseinfo->mCourseIndex);
	FOREACH_NODE(Generator, generatorCache->getFirstGenerator(), node)
	{
		if (node->isReservedFlag(Generator::Reserved_doSaveGen)) {
			generatorCache->saveGenerator(node);
		}
	}
	FOREACH_NODE(Generator, generatorCache->getFirstGenerator(), node)
	{
		if (node->isReservedFlag(Generator::Reserved_doSaveGen) && node->isReservedFlag(Generator::Reserved_doSaveCreature)) {
			generatorCache->saveCreature(node);
		}
	}
	generatorCache->savePikiheads();
	generatorCache->endSave();
}

void BaseGameSection::pmTogglePlayer()
{
	if (mPrevNaviIdx == NAVIID_Olimar) {
		setPlayerMode(NAVIID_Louie);
		moviePlayer->mViewport     = sys->mGfx->getViewport(PLAYER2_VIEWPORT);
		moviePlayer->mActingCamera = mLouieCamera;
	} else if (mPrevNaviIdx == NAVIID_Louie) {
		setPlayerMode(NAVIID_Olimar);
		moviePlayer->mViewport     = sys->mGfx->getViewport(PLAYER1_VIEWPORT);
		moviePlayer->mActingCamera = mOlimarCamera;
	}
	onTogglePlayer();
}

/**
 * @note Address: N/A
 * @note Size: 0x64
 */
void BaseGameSection::pmPlayerJoin()
{
	if (mPrevNaviIdx == NAVIID_Olimar) {
		// something
	} else if (mPrevNaviIdx == NAVIID_Louie) {
		// something else
	}
	onPlayerJoin();
}

/**
 * @note Address: 0x8014D918
 * @note Size: 0x2B8
 */
void BaseGameSection::setPlayerMode(int mode)
{
	Navi* navis[2];
	navis[NAVIID_Olimar] = navis[NAVIID_Louie] = nullptr;

	navis[NAVIID_Olimar] = naviMgr->getAt(NAVIID_Olimar);
	navis[NAVIID_Louie]  = naviMgr->getAt(NAVIID_Louie);

	navis[NAVIID_Olimar]->disableController();
	navis[NAVIID_Louie]->disableController();

	switch (mode) {
	case NAVIID_Olimar: {
		mSecondViewportHeight = 1.0f;
		mSplit                = 0.0f;
		mSplitter->split2(1.0f);
		Matrixf* viewMtx = mLouieCamera->getViewMatrix(false);
		PSMTXCopy((PSQuaternion*)viewMtx, (PSQuaternion*)&mOlimarCamera->mCurViewMatrix);
		mOlimarCamera->update();
		cameraMgr->changePlayerMode(NAVIID_Olimar, cameraMgrCallback);
		if (mPlayerMode == 1) {
			Graphics* gfx = sys->mGfx;
			gfx->getViewport(PLAYER1_VIEWPORT)->setCamera(mOlimarCamera);
			gfx->getViewport(PLAYER2_VIEWPORT)->setCamera(mLouieCamera);
		}
		Viewport* olimarViewport    = sys->mGfx->getViewport(PLAYER1_VIEWPORT);
		sys->mGfx->mCurrentViewport = olimarViewport;
		mLightMgr->updatePosition(sys->mGfx->mCurrentViewport);
		break;
	}
	case NAVIID_Louie: {
		if (mPlayerMode == 1) {
			Graphics* gfx = sys->mGfx;

			gfx->getViewport(PLAYER1_VIEWPORT)->setCamera(mLouieCamera);
			gfx->getViewport(PLAYER2_VIEWPORT)->setCamera(mOlimarCamera);

			mSecondViewportHeight = 1.0f;
			mSplitter->split2(1.0f);
		} else {
			mSecondViewportHeight = 0.0f;
			mSplitter->split2(0.0f);
		}

		mSplit = 0.0f;

		Matrixf* viewMtx = mOlimarCamera->getViewMatrix(false);
		PSMTXCopy((PSQuaternion*)viewMtx, (PSQuaternion*)&mLouieCamera->mCurViewMatrix);

		mLouieCamera->update();
		cameraMgr->changePlayerMode(NAVIID_Louie, cameraMgrCallback);

		Viewport* louieViewport     = sys->mGfx->getViewport(PLAYER2_VIEWPORT);
		sys->mGfx->mCurrentViewport = louieViewport;
		mLightMgr->updatePosition(sys->mGfx->mCurrentViewport);
		break;
	}
	case NAVIID_Multiplayer: {
		mSecondViewportHeight = 0.5f;
		mSplit                = 0.0f;
		mSplitter->split2(0.5f);
		cameraMgr->changePlayerMode(NAVIID_Multiplayer, cameraMgrCallback);
		break;
	}
	}
	mPrevNaviIdx = mode;
}

/**
 * @note Address: 0x8014DBD4
 * @note Size: 0x14C
 */
void BaseGameSection::onCameraBlendFinished(CameraArg* arg)
{
	setCamController();
	if (gameSystem->isStoryMode()) {
		if (!playData->isDemoFlag(DEMO_First_Use_Louie) && playData->isDemoFlag(DEMO_Unlock_Captain_Switch)) {
			Navi* louie = naviMgr->getAt(NAVIID_Louie);
			MoviePlayArg louieStart("x05_louiestart", nullptr, nullptr, 0);
			louieStart.setTarget(louie);
			moviePlayer->mTargetObject = louie;
			moviePlayer->play(louieStart);
			playData->setDemoFlag(DEMO_First_Use_Louie);
		}
	}
}

/**
 * @note Address: 0x8014DD20
 * @note Size: 0x68
 */
void BaseGameSection::setFixNearFar(bool isFixed, f32 near, f32 far)
{
	mOlimarCamera->setFixNearFar(isFixed, near, far);
	mLouieCamera->setFixNearFar(isFixed, near, far);
}

/**
 * @note Address: 0x8014DD88
 * @note Size: 0x210
 */
void BaseGameSection::setCamController()
{
	Navi* navis[2];

	navis[NAVIID_Olimar] = naviMgr->getAt(NAVIID_Olimar);
	navis[NAVIID_Louie]  = naviMgr->getAt(NAVIID_Louie);

	switch (mPrevNaviIdx) {
	case NAVIID_Olimar: {
		PlayCamera* olimarCam              = mOlimarCamera;
		navis[NAVIID_Olimar]->mCamera      = olimarCam;
		navis[NAVIID_Olimar]->mCamera2     = olimarCam;
		Controller* olimarController       = mControllerP1;
		navis[NAVIID_Olimar]->mController1 = olimarController;
		navis[NAVIID_Olimar]->mController2 = olimarController;
		navis[NAVIID_Louie]->disableController();
		moviePlayer->mTargetNavi   = navis[NAVIID_Olimar];
		moviePlayer->mViewport     = sys->mGfx->getViewport(PLAYER1_VIEWPORT);
		moviePlayer->mActingCamera = mOlimarCamera;
		if (!gameSystem->isMultiplayerMode()) {
			PSSetCurCameraNo(NAVIID_Olimar);
			PSPlayerChangeToOrimer();
		}
		break;
	}
	case NAVIID_Louie: {
		navis[NAVIID_Olimar]->disableController();
		PlayCamera* louieCam              = mLouieCamera;
		navis[NAVIID_Louie]->mCamera      = louieCam;
		navis[NAVIID_Louie]->mCamera2     = louieCam;
		Controller* louieController       = mControllerP1;
		navis[NAVIID_Louie]->mController1 = louieController;
		navis[NAVIID_Louie]->mController2 = louieController;
		moviePlayer->mTargetNavi          = navis[NAVIID_Louie];
		moviePlayer->mViewport            = sys->mGfx->getViewport(PLAYER2_VIEWPORT);
		moviePlayer->mActingCamera        = mLouieCamera;
		if (!gameSystem->isMultiplayerMode()) {
			PSSetCurCameraNo(NAVIID_Louie);
			PSPlayerChangeToLugie();
		}
		break;
	}
	case NAVIID_Multiplayer: {
		PlayCamera* olimarCam              = mOlimarCamera;
		navis[NAVIID_Olimar]->mCamera      = olimarCam;
		navis[NAVIID_Olimar]->mCamera2     = olimarCam;
		Controller* olimarController       = mControllerP1;
		navis[NAVIID_Olimar]->mController1 = olimarController;
		navis[NAVIID_Olimar]->mController2 = olimarController;
		PlayCamera* louieCam               = mLouieCamera;
		navis[NAVIID_Louie]->mCamera       = louieCam;
		navis[NAVIID_Louie]->mCamera2      = louieCam;
		Controller* louieController        = mControllerP2;
		navis[NAVIID_Louie]->mController1  = louieController;
		navis[NAVIID_Louie]->mController2  = louieController;

		moviePlayer->mTargetNavi   = navis[NAVIID_Olimar];
		moviePlayer->mActingCamera = mOlimarCamera;
		if (gameSystem->isStoryMode()) {
			PSSetCurCameraNo(NAVIID_Olimar);
		}
		break;
	}
	}
	on_setCamController(mPrevNaviIdx);
}

/**
 * @note Address: N/A
 * @note Size: 0x18
 */
int BaseGameSection::getNumWindows()
{
	// UNUSED FUNCTION
}

/**
 * @note Address: N/A
 * @note Size: 0x8
 */
int BaseGameSection::getActivePlayerID()
{
	// UNUSED FUNCTION
}

/**
 * @note Address: 0x8014DF9C
 * @note Size: 0x184
 */
void BaseGameSection::setDefaultPSSceneInfo(PSGame::SceneInfo& sceneInfo)
{
#if defined(VERSION_PAL)
	P2ASSERTLINE(3200, mOlimarCamera);
#elif defined(VERSION_JP)
	P2ASSERTLINE(3192, mOlimarCamera);
#else
	P2ASSERTLINE(3197, mOlimarCamera);
#endif
#if defined(VERSION_PAL)
	P2ASSERTLINE(3201, mLouieCamera);
#elif defined(VERSION_JP)
	P2ASSERTLINE(3193, mLouieCamera);
#else
	P2ASSERTLINE(3198, mLouieCamera);
#endif

	sceneInfo.mCameras                     = 2;
	sceneInfo.mCam1Position[NAVIID_Olimar] = mOlimarCamera->getSoundPositionPtr();
	sceneInfo.mCam2Position[NAVIID_Olimar] = mOlimarCamera->getSoundPositionPtr();
	sceneInfo.mCameraMtx[NAVIID_Olimar]    = mOlimarCamera->getSoundMatrixPtr();
	sceneInfo.mCam1Position[NAVIID_Louie]  = mLouieCamera->getSoundPositionPtr();
	sceneInfo.mCam2Position[NAVIID_Louie]  = mLouieCamera->getSoundPositionPtr();
	sceneInfo.mCameraMtx[NAVIID_Louie]     = mLouieCamera->getSoundMatrixPtr();
	BoundBox box;

	mapMgr->getBoundBox(box);

	Vector3f min = box.mMin;
	Vector3f max = box.mMax;

	sceneInfo.mBounds.i.x = min.x;
	sceneInfo.mBounds.i.y = min.y;
	sceneInfo.mBounds.i.z = min.z;
	sceneInfo.mBounds.f.x = max.x;
	sceneInfo.mBounds.f.y = max.y;
	sceneInfo.mBounds.f.z = max.z;
}

/**
 * @note Address: 0x8014E130
 * @note Size: 0x68C
 */
void BaseGameSection::prepareHoleIn(Vector3f& surroundPos, bool killPikihead)
{
	Screen::gGame2DMgr->mScreenMgr->reset();
	Navi* aliveOrima = naviMgr->getAliveOrima(ALIVEORIMA_Active);
	if (killPikihead) {
		Iterator<ItemPikihead::Item> iPikihead = ItemPikihead::mgr;
		CI_LOOP(iPikihead)
		{
			ItemPikihead::Item* item = *iPikihead;
			if (item->isAlive()) {
				DeathMgr::inc(DeathCounter::COD_Battle);
				DeathMgr::inc(DeathCounter::COD_All);
				if (gameSystem->isChallengeMode()) {
					GameMessageVsPikminDead deadPikmin;
					sendMessage(deadPikmin);
				}
			}
		}
	}
	Iterator<Piki> iPiki = pikiMgr;
	CI_LOOP(iPiki)
	{
		Piki* piki = *iPiki;
		if (piki->getKind() != Bulbmin || piki->isPikmin()) {
			if (!piki->isZikatu()) {
				if (piki->getKind() == Bulbmin) {
					piki->getCurrActionID();
					piki->getStateID();
				}
				piki->endStick();
				piki->mFsm->transitForce(piki, PIKISTATE_Walk, nullptr);
				piki->mNavi   = aliveOrima;
				f32 randAngle = randFloat() * TAU;

				Vector3f surroundCircle(sinf(randAngle), 0, cosf(randAngle));

				Vector3f vec = Vector3f(sinf(randAngle) * 50.0f, 0.0f, cosf(randAngle) * 50.0f);
				vec          = vec + surroundPos;
				vec.y        = mapMgr->getMinY(vec);
				piki->setPosition(vec, false);
				PikiAI::ActFormationInitArg arg(aliveOrima);
				arg.mIsDemoFollow = true;
				piki->mBrain->start(PikiAI::ACT_Formation, &arg);
				piki->movie_begin(false);
			}
		}
	}
	if (aliveOrima) {
		aliveOrima->demowaitAllPikis();
	}
}

/**
 * @note Address: 0x8014E7BC
 * @note Size: 0x714
 */
void BaseGameSection::prepareFountainOn(Vector3f& surroundPos)
{
	if (ItemBigFountain::mgr) {
		Iterator<BaseItem> iFountain = ItemBigFountain::mgr;
		CI_LOOP(iFountain)
		{
			ItemBigFountain::Item* fountain = static_cast<ItemBigFountain::Item*>(*iFountain);
			fountain->killAllEffect();
		}
	}
	Screen::gGame2DMgr->mScreenMgr->reset();
	Navi* aliveOrima = naviMgr->getAliveOrima(ALIVEORIMA_Active);

	Iterator<ItemPikihead::Item> iPikihead = ItemPikihead::mgr;
	CI_LOOP(iPikihead)
	{
		ItemPikihead::Item* item = *iPikihead;
		if (item->isAlive()) {
			DeathMgr::inc(DeathCounter::COD_Battle);
			DeathMgr::inc(DeathCounter::COD_All);
			if (gameSystem->isChallengeMode()) {
				GameMessageVsPikminDead deadPikmin;
				sendMessage(deadPikmin);
			}
		}
	}
	PikiCond_ExceptChappyPikmin pikiCond;
	pikiMgr->moveAllPikmins(surroundPos, 50.0f, &pikiCond);
	Iterator<Piki> iPiki(pikiMgr, NULL, nullptr);

	CI_LOOP(iPiki)
	{
		Piki* piki = *iPiki;
		if (piki->getKind() == Bulbmin) {
			piki->movie_begin(false);
		} else {
			piki->endStick();
			piki->mFsm->transitForce(piki, PIKISTATE_Walk, nullptr);

			PikiAI::ActFormationInitArg arg(aliveOrima, false);

			piki->mNavi = aliveOrima;

			arg.mIsDemoFollow = true; // MAKE UP YOUR DAMN MIND I STG

			piki->mBrain->start(PikiAI::ACT_Formation, &arg);
			piki->movie_begin(false);
		}
	}
	if (aliveOrima) {
		aliveOrima->demowaitAllPikis();
	}
}

/**
 * @note Address: 0x8014EED0
 * @note Size: 0x74
 */
void BaseGameSection::initLights()
{
	mLightMgr           = new GameLightMgr("ゲームライトマネージャ"); // game light manager
	mLightMgr->mTimeMgr = gameSystem->mTimeMgr;
	addGenNode(mLightMgr);
	particleMgr->mLightMgr = mLightMgr;
}

/**
 * @note Address: 0x8014EF44
 * @note Size: 0x20
 */
void BaseGameSection::draw3D(Graphics& gfx)
{
	newdraw_draw3D_all(gfx);
}

/**
 * @note Address: 0x8014EF64
 * @note Size: 0x1D4
 */
void BaseGameSection::drawParticle(Graphics& gfx, int viewport)
{
	Viewport* port = gfx.getViewport(viewport);
	if (!port || !port->viewable()) {
		return;
	}

	port->setProjection();
	port->setViewport();
	if (!gameSystem->isMultiplayerMode() && mPrevNaviIdx != NAVIID_Multiplayer) {
		mLightMgr->mFogMgr->off(gfx);
		particleMgr->draw(port, 0);
		mLightMgr->mFogMgr->set(gfx);
	}
	if (moviePlayer && moviePlayer->isFlag(MVP_IsActive)) {
		for (u8 i = 3; i <= 5; i++) {
			particleMgr->draw(port, i);
		}
	}
	particleMgr->draw(port, 1);
	mLightMgr->mFogMgr->off(gfx);
	if (moviePlayer && moviePlayer->isFlag(MVP_IsActive)) {
		for (u8 i = 6; i <= 8; i++) {
			particleMgr->draw(port, i);
		}
	}
	particleMgr->draw(port, 2);
}

/**
 * @note Address: 0x8014F138
 * @note Size: 0xA0
 */
void BaseGameSection::draw_Ogawa2D(Graphics& gfx)
{
	gfx.mPerspGraph.setPort();
	particle2dMgr->draw(true, 0);
	Screen::gGame2DMgr->draw(gfx);
	gfx.mPerspGraph.setPort();
	particle2dMgr->draw(false, 0);
}

/**
 * @note Address: 0x8014F1DC
 * @note Size: 0x1BC
 */
void BaseGameSection::draw2D(Graphics& gfx)
{
	j3dSys.reinitGX();
	gfx.mOrthoGraph.setPort();
	draw_Ogawa2D(gfx);
	if (mXfbTexture2d) {
		mXfbTexture2d->capture(mXfbBounds2dX, mXfbBounds2dY, GX_TF_RGB565, false, 0);
	}
	if (!mXfbTexture2d && mXfbFlags & 2) {
		mXfbImage->capture(mXfbBoundsX, mXfbBoundsY, GX_TF_RGB565, true, 0);
		mXfbFlags &= ~2;
		mXfbFlags |= 1;
		mXfbFlags |= 4;
	}
	Screen::gGame2DMgr->drawIndirect(gfx);
	gfx.mOrthoGraph.setPort();
	J2DPrint print(JFWSystem::systemFont, 0.0f);
	print.initiate();
	print.setCharColor(JUtility::TColor(158, 219, 255, 255));
	print.setGradColor(JUtility::TColor(56, 159, 247, 255));
}

/**
 * @note Address: N/A
 * @note Size: 0x4
 */
void BaseGameSection::setupViewportMatrix(Graphics&)
{
	// UNUSED FUNCTION
}

/**
 * @note Address: 0x8014F398
 * @note Size: 0xB8
 */
void BaseGameSection::directDraw(Graphics& gfx, Viewport* vp)
{
	vp->setViewport();
	vp->setProjection();
	gfx.initPrimDraw(vp->getMatrix(true));
	doDirectDraw(gfx, vp);
	if (TexCaster::Mgr::sInstance) {
		gfx.initPrimDraw(vp->getMatrix(true));
		mLightMgr->mFogMgr->set(gfx);
		TexCaster::Mgr::sInstance->draw(gfx);
	}
}

/**
 * @note Address: 0x8014F450
 * @note Size: 0x78
 */
void BaseGameSection::directDrawPost(Graphics& gfx, Viewport* vp)
{
	vp->setViewport();
	vp->setProjection();
	gfx.initPrimDraw(vp->getMatrix(true));
	doDirectDrawPost(gfx, vp);
}

/**
 * @note Address: N/A
 * @note Size: 0x140
 */
void BaseGameSection::j3dDraw(Viewport*)
{
	// UNUSED FUNCTION
}

/**
 * @note Address: N/A
 * @note Size: 0x4
 */
void BaseGameSection::j3dDrawPostShadow(Viewport*)
{
	// UNUSED FUNCTION
}

/**
 * @note Address: N/A
 * @note Size: 0x4
 */
void BaseGameSection::j3dDrawObjectLast(Viewport*)
{
	// UNUSED FUNCTION
}

/**
 * @note Address: N/A
 * @note Size: 0x4
 */
void BaseGameSection::j3dDrawPost(Viewport*)
{
	// UNUSED FUNCTION
}

/**
 * @note Address: N/A
 * @note Size: 0x4
 */
void BaseGameSection::j3dDrawLast(Viewport*)
{
	// UNUSED FUNCTION
}

/**
 * @note Address: 0x8014F4C8
 * @note Size: 0x54
 */
void BaseGameSection::j3dSetView(Viewport* vp, bool b)
{
	vp->setJ3DViewMtx(b);
	doSetView(vp->mVpId);
	doViewCalc();
}

/**
 * @note Address: N/A
 * @note Size: 0x68
 */
void BaseGameSection::j3dViewCalc(Viewport*)
{
	// UNUSED FUNCTION
}

/**
 * @note Address: 0x8014F51C
 * @note Size: 0x30
 */
void BaseGameSection::doSimulation(f32 rate)
{
	gameSystem->doSimulation(rate);
}

/**
 * @note Address: 0x8014F54C
 * @note Size: 0x30
 */
void BaseGameSection::doSimpleDraw(Viewport* vp)
{
	gameSystem->doSimpleDraw(vp);
}

/**
 * @note Address: 0x8014F57C
 * @note Size: 0x1D8
 */
void BaseGameSection::doAnimation()
{
	Navi* olimar = naviMgr->getAt(NAVIID_Olimar);

	if (testPathfinder) {
		testPathfinder->update();
	}

	gameSystem->doAnimation();

	if (particleMgr && !gameSystem->mIsFrozen) {
		particleMgr->doAnimation();
	}

	if (!gameSystem->isZukanMode()) {
		updateSplitter();
	}
}

/**
 * @note Address: 0x8014F7A0
 * @note Size: 0xC8
 */
void BaseGameSection::doEntry()
{
	setDrawBuffer(DB_NormalLayer);
	gameSystem->doEntry();
	if (particleMgr) {
		setDrawBuffer(DB_NormalFogOffLayer);
		particleMgr->doEntry();
	}
}

/**
 * @note Address: 0x8014F868
 * @note Size: 0x100
 */
void BaseGameSection::doSetView(int viewportNumber)
{
	if (mPlayerMode == 1)
		viewportNumber = 0;

	gameSystem->doSetView(viewportNumber);
	if (particleMgr) {
		particleMgr->doSetView(viewportNumber);
	}
}

/**
 * @note Address: 0x8014F968
 * @note Size: 0xC8
 */
void BaseGameSection::doViewCalc()
{
	gameSystem->doViewCalc();
	if (particleMgr) {
		particleMgr->doViewCalc();
	}
}

/**
 * @note Address: N/A
 * @note Size: 0x74
 */
void BaseGameSection::initBlendCamera()
{
	// UNUSED FUNCTION
}

/**
 * @note Address: 0x8014FA30
 * @note Size: 0x174
 */
void BaseGameSection::updateBlendCamera()
{
	if (mPrevNaviIdx == NAVIID_Olimar) {
		mBlendFactor -= sys->mDeltaTime / 0.2f;
		if (mBlendFactor < 0.0f) {
			mBlendFactor         = 0.0f;
			mIsBlendCameraActive = false;
			mSplitter->split2(1.0f);
		}

	} else {
		mBlendFactor += sys->mDeltaTime / 0.2f;
		if (mBlendFactor > 1.0f) {
			mBlendFactor         = 1.0f;
			mIsBlendCameraActive = false;
			if (mPlayerMode == 1) {
				mSplitter->split2(1.0f);
			} else {
				mSplitter->split2(0.0f);
			}
		}
	}

	if (mBlendCamera) {
		mBlendCamera->setBlendFactor(mBlendFactor);
		mBlendCamera->update();
	}
	if (!mIsBlendCameraActive) {
		Graphics* gfx = sys->mGfx;
		gfx->getViewport(PLAYER1_VIEWPORT)->setCamera(mOlimarCamera);
		gfx->getViewport(PLAYER2_VIEWPORT)->setCamera(mLouieCamera);
		setCamController();
	}
}

/**
 * @note Address: N/A
 * @note Size: 0xDC
 */
void BaseGameSection::blend1to2()
{
	// UNUSED FUNCTION
}

/**
 * @note Address: N/A
 * @note Size: 0xDC
 */
void BaseGameSection::blend2to1()
{
	// UNUSED FUNCTION
}

/**
 * @note Address: 0x8014FBA4
 * @note Size: 0x6C
 */
void BaseGameSection::setSplitter(bool flag)
{
	if (flag) {
		mSecondViewportHeight = 0.5f;
		mSetSplit             = true;
	} else {
		mSecondViewportHeight = 1.0f;
		mSetSplit             = false;
	}
	mSplit = 0.0f;
	mSplitter->split2(mSecondViewportHeight);
}

/**
 * @note Address: N/A
 * @note Size: 0xC
 */
void BaseGameSection::startSplit()
{
	// UNUSED FUNCTION
}

/**
 * @note Address: N/A
 * @note Size: 0xC
 */
void BaseGameSection::changeSplit()
{
	// UNUSED FUNCTION
}

/**
 * @note Address: N/A
 * @note Size: 0xC
 */
void BaseGameSection::endSplit()
{
	// UNUSED FUNCTION
}

/**
 * @note Address: 0x8014FC10
 * @note Size: 0x134
 */
void BaseGameSection::updateSplitter()
{
	if (mSplit == 0.0f && moviePlayer->mDemoState == DEMOSTATE_Inactive && gameSystem->isFlag(GAMESYS_IsGameWorldActive)) {
		return;
	}

	mSecondViewportHeight += mSplit * sys->mDeltaTime;
	int id = mPrevNaviIdx;
	if (id == NAVIID_Multiplayer && mSecondViewportHeight <= 0.5f) {
		mSecondViewportHeight = 0.5f;
		mSplit                = 0.0f;
		mSetSplit             = true;
		setCamController();
	} else if (id == NAVIID_Olimar && mSecondViewportHeight >= 1.0f) {
		mSecondViewportHeight = 1.0f;
		mSplit                = 0.0f;
		mSetSplit             = false;
		setCamController();
	} else if (id == NAVIID_Louie && mSecondViewportHeight <= 0.0f) {
		mSecondViewportHeight = 0.0f;
		mSplit                = 0.0f;
		setCamController();
	}

	if (mSplitter) {
		mSplitter->split2(mSecondViewportHeight);
	}
}

/**
 * @note Address: 0x8014FD44
 * @note Size: 0x58
 */
void BaseGameSection::doDirectDrawPost(Graphics& gfx, Viewport*)
{
	if (lifeGaugeMgr) {
		lifeGaugeMgr->draw(gfx);
	}
	if (carryInfoMgr) {
		carryInfoMgr->draw(gfx);
	}
}

/**
 * @note Address: 0x8014FD9C
 * @note Size: 0x4
 */
void BaseGameSection::doDirectDraw(Graphics&, Viewport*)
{
}

/**
 * @note Address: N/A
 * @note Size: 0x78
 */
void BaseGameSection::startHeap()
{
	mTheExpHeap = JKRExpHeap::create(JKRGetCurrentHeap()->getFreeSize(), JKRGetCurrentHeap(), true);
	theExpHeap  = mTheExpHeap;
	mBackupHeap = mTheExpHeap->becomeCurrentHeap();
	onStartHeap();
}

/**
 * @note Address: 0x8014FDA4
 * @note Size: 0x2A8
 */
void BaseGameSection::clearHeap()
{
	TexCaster::Mgr::deleteInstance();
	PSSystem::SceneMgr* mgr = PSSystem::getSceneMgr();
	mgr->deleteCurrentScene();
	GXWaitDrawDone();
	itemMgr->clearGlobalPointers();
	platCellMgr = nullptr;
	cellMgr     = nullptr;
	mOpaqueDrawBuffer->frameInitAll();
	mTransparentDrawBuffer->frameInitAll();
	particleMgr->killAll();
	particleMgr->reset();
	Generator::initialiseSystem();
	mOlimarCamera->del();
	mLouieCamera->del();
	rumbleMgr->del();
	cameraMgr->del();
	shadowMgr->del();
	generatorMgr->del();
	onceGeneratorMgr->del();
	limitGeneratorMgr->del();
	plantsGeneratorMgr->del();
	dayGeneratorMgr->del();
	itemMgr->del();
	if (generalEnemyMgr)
		generalEnemyMgr->del();
	mLightMgr->del();
	naviMgr->resetMgr();
	pikiMgr->resetMgr();
	pelletMgr->resetMgrs();
	dynParticleMgr->resetMgr();

	gameSystem->detachObjectMgr(generalEnemyMgr);
	gameSystem->detachObjectMgr(mapMgr);
	gameSystem->detachObjectMgr(itemMgr);
	mapMgr = nullptr;
	onClearHeap();
	mapMgr       = nullptr;
	platMgr      = nullptr;
	cameraMgr    = nullptr;
	shadowMgr    = nullptr;
	carryInfoMgr = nullptr;
	lifeGaugeMgr = nullptr;
	mBlendCamera = nullptr;
	if (mControllerP1) {
		delete mControllerP1;
	}
	if (mControllerP2) {
		delete mControllerP2;
	}
	mControllerP2 = nullptr;
	mControllerP1 = nullptr;
	mLouieCamera  = nullptr;
	mOlimarCamera = nullptr;

	rumbleMgr           = nullptr;
	cameraMgr           = nullptr;
	itemMgr             = nullptr;
	generalEnemyMgr     = nullptr;
	testPathfinder      = nullptr;
	mTreasureZoomCamera = nullptr;

	mTheExpHeap->destroy();
	mTheExpHeap = nullptr;
	theExpHeap  = nullptr;
	mBackupHeap->becomeCurrentHeap();
}

/**
 * @note Address: 0x801500B0
 * @note Size: 0x2C
 */
void BaseGameSection::startFadeout(f32 time)
{
	mDisplayWiper = mWipeOutFader;
	mWipeOutFader->start(time);
}

/**
 * @note Address: 0x801500DC
 * @note Size: 0x2C
 */
void BaseGameSection::startFadein(f32 time)
{
	mDisplayWiper = mWipeInFader;
	mWipeInFader->start(time);
}

/**
 * @note Address: 0x80150108
 * @note Size: 0x2C
 */
void BaseGameSection::startFadeoutin(f32 time)
{
	mDisplayWiper = mWipeOutInFader;
	mWipeOutInFader->start(time);
}

/**
 * @note Address: 0x80150134
 * @note Size: 0x3C
 */
void BaseGameSection::startFadeblack()
{
	mDisplayWiper         = mBlackFader;
	mBlackFader->mIsBlack = true;
	mBlackFader->start(999.0f);
}

/**
 * @note Address: 0x80150170
 * @note Size: 0x3C
 */
void BaseGameSection::startFadewhite()
{
	mDisplayWiper         = mBlackFader;
	mBlackFader->mIsBlack = false;
	mBlackFader->start(999.0f);
}

/**
 * @note Address: 0x801501AC
 * @note Size: 0x1C0
 */
void BaseGameSection::setupFixMemory()
{
	Delegate<BaseGameSection>* delegate = new Delegate<BaseGameSection>(this, setupFixMemory_dvdload);

	beginFrame();
	beginRender();
	j3dSys.drawInit();
	GXSetViewport(0.0f, 0.0f, 608.0f, 480.0f, 0.0f, 1.0f);
	GXSetScissor(0, 16, 608, 448);
	endRender();
	sys->dvdLoadUseCallBack(&mDvdThreadCommand, delegate);
	waitSyncLoad(false); // inlines waitSyncLoad
}

/**
 * @note Address: 0x8015036C
 * @note Size: 0x334
 */
void BaseGameSection::setupFixMemory_dvdload()
{
	ResTIMG* file = static_cast<ResTIMG*>(JKRDvdRipper::loadToMainRAM("user/Kando/mizu.bti", nullptr, Switch_0, 0, nullptr,
	                                                                  JKRDvdRipper::ALLOC_DIR_TOP, 0, nullptr, nullptr));
	mMizuTexture  = new JUTTexture(file);

	mXfbImage = new JUTTexture((System::getRenderModeWidth() >> 1) & 0x7FFF, (System::getRenderModeHeight() >> 1) & 0x7FFF, GX_TF_RGB565);
	gameSystem->mXfbTexture = mXfbImage;

	ParticleMgr::globalInstance();
	particleMgr->createHeap(PARTICLE_MGR_HEAP_SIZE);
	particleMgr->createMgr("user/Ebisawa/effect/game.jpc", 2000, 300, 0x80);
	addGenNode(particleMgr);

	TParticle2dMgr::globalInstance();
	particle2dMgr->createHeap(PARTICLE_MGR2D_HEAP_SIZE);
#if defined(VERSION_PAL)
	particle2dMgr->createMgr("user/Ebisawa/effect/eff2d_game2d.jpc", 0x1A0, 0x42, 0x80);
#elif defined(VERSION_JP)
	particle2dMgr->createMgr("user/Ebisawa/effect/eff2d_game2d.jpc", 0x1F4, 0x18, 0x80);
#else
	particle2dMgr->createMgr("user/Ebisawa/effect/eff2d_game2d.jpc", 0x1D4, 0x28, 0x80);
#endif
	addGenNode(particle2dMgr);

	particleMgr->beginEntryModelEffect();
	efx::OnyonSpotData* spot = new efx::OnyonSpotData;
	spot->entry();
	particleMgr->endEntryModelEffect();
	particleMgr->Instance_TPkEffectMgr();

	collisionUpdateMgr = new UpdateMgr;
	collisionUpdateMgr->create(3);

	naviMgr = new NaviMgr;
	gameSystem->addObjectMgr(naviMgr);
	naviMgr->loadResources();

	pikiMgr = new PikiMgr;
	gameSystem->addObjectMgr(pikiMgr);

	addGenNode(pikiMgr);
	addGenNode(naviMgr);
	pikiMgr->loadResources(mPlayerMode);

	pelletMgr = new PelletMgr;
	pelletMgr->createManagers(nullptr);
	gameSystem->addObjectMgr(pelletMgr);

	createScreenRootNode();
}

} // namespace Game

/**
 * @note Address: N/A
 * @note Size: 0x50
 */
void BaseGameAllocCallback(u32, int, JKRHeap*, void*)
{
	JUT_PANIC("allocation dameck!\n%d/%d");
	// UNUSED FUNCTION
}

namespace Game {

/**
 * @note Address: 0x80150700
 * @note Size: 0x8
 */
bool BaseGameSection::enableAllocHalt()
{
	return false;
}

/**
 * @note Address: 0x80150708
 * @note Size: 0x8
 */
bool BaseGameSection::disableAllocHalt()
{
	return false;
}

/**
 * @note Address: N/A
 * @note Size: 0x8
 */
bool BaseGameSection::isAllocHalt()
{
	// UNUSED FUNCTION
}

/**
 * @note Address: 0x80150710
 * @note Size: 0xCC0
 */
void BaseGameSection::setupFloatMemory()
{
	bool cave = false;
	gameSystem->resetFlag(GAMESYS_IsSoundSceneActive);

	PSSystem::SingletonBase<PSM::ObjMgr>::newInstance();
	PSSystem::SingletonBase<PSM::BossBgmFader::Mgr>::newInstance();

	mTheExpHeap = JKRExpHeap::create(JKRGetCurrentHeap()->getFreeSize(), JKRGetCurrentHeap(), true);
	theExpHeap  = mTheExpHeap;
	mBackupHeap = mTheExpHeap->becomeCurrentHeap();
	onStartHeap();

	naviMgr->loadResources_float();
	lifeGaugeMgr = new LifeGaugeMgr;
	lifeGaugeMgr->loadResource();

	carryInfoMgr = new CarryInfoMgr(64);

	carryInfoMgr->loadResource();

	platMgr   = new PlatMgr;
	shadowMgr = new ShadowMgr(2);
	addGenNode(shadowMgr);
	initLights();

	naviMgr->alloc(2);

	Navi* p1 = naviMgr->getAt(NAVIID_Olimar);
	Navi* p2 = naviMgr->getAt(NAVIID_Louie);

	mControllerP1 = new Controller(JUTGamePad::PORT_0);
	mControllerP2 = new Controller(JUTGamePad::PORT_1);
	mOlimarCamera = new PlayCamera(p1);
	mLouieCamera  = new PlayCamera(p2);
	cameraMgr     = new CameraMgr;
	cameraMgr->loadResource();
	addGenNode(cameraMgr);

	Camera* cams[2]      = { mOlimarCamera, mLouieCamera };
	mBlendCamera         = new BlendCamera(2, cams);
	mIsBlendCameraActive = false;
	mLightMgr->mCamera   = mOlimarCamera;

	rumbleMgr = new RumbleMgr;
	rumbleMgr->loadResource();
	rumbleMgr->init();
	addGenNode(rumbleMgr);

	bool incave = false;
	if (gameSystem->mIsInCave) {
		incave = true;
	}
	itemMgr = new ItemMgr;
	if (incave) {
		itemMgr->createManagers(1);
	} else {
		itemMgr->createManagers(2);
	}
	gameSystem->addObjectMgr(itemMgr);
	addGenNode(itemMgr);

	pikiMgr->alloc(MAX_PIKI_COUNT);

	generalEnemyMgr = new GeneralEnemyMgr;
	gameSystem->addObjectMgr(generalEnemyMgr);
	addGenNode(generalEnemyMgr);
	onSetupFloatMemory();

	// no mapMgr means in cave
	if (!mapMgr) {
		cave = true;
		char path[512];
		sprintf(path, "user/Mukki/mapunits/caveinfo/%s", getCaveFilename());
		getCaveFilename();
		Cave::CaveInfo* info       = Cave::CaveInfo::load(path);
		Cave::EditMapUnit* mapunit = nullptr;
		if (gameSystem->isVersusMode()) {
			if (strcmp("random", getEditorFilename())) {
				sprintf(path, "user/Abe/vs/%s", getEditorFilename());
				mapunit = new Cave::EditMapUnit;
				mapunit->read(path);
				mapunit->setEditNumber(getVsEditNumber());
			}
		}
		mapMgr = new RoomMapMgr(info);
		static_cast<RoomMapMgr*>(mapMgr)->createRandomMap(getCurrFloor(), mapunit);
		gameSystem->addObjectMgr(mapMgr);

		if (gameSystem->isVersusMode()) {
			GameMessageVsAddEnemy mesg1(EnemyTypeID::EnemyID_Hanachirashi, 4);
			sendMessage(mesg1);

			GameMessageVsAddEnemy mesg2(EnemyTypeID::EnemyID_Sarai, 4);
			sendMessage(mesg2);

			GameMessageVsAddEnemy mesg3(EnemyTypeID::EnemyID_Rock, 12);
			sendMessage(mesg3);

			GameMessageVsAddEnemy mesg4(EnemyTypeID::EnemyID_BombOtakara, 2);
			sendMessage(mesg4);

			GameMessageVsAddEnemy mesg5(EnemyTypeID::EnemyID_Tank, 4);
			sendMessage(mesg5);

			GameMessageVsAddEnemy mesg6(EnemyTypeID::EnemyID_Wtank, 4);
			sendMessage(mesg6);

			GameMessageVsAddEnemy mesg7(EnemyTypeID::EnemyID_Tobi, 20);
			sendMessage(mesg7);
		}

		BoundBox2d bounds(12800000.0f, 12800000.0f, -12800000.0f, -12800000.0f);
		mapMgr->getBoundBox2d(bounds);
		platCellMgr = new CellPyramid;
		platCellMgr->create(bounds, 128.0f);

		BoundBox2d bounds2(12800000.0f, 12800000.0f, -12800000.0f, -12800000.0f);
		mapMgr->getBoundBox2d(bounds2);
		mapRoomCellMgr = new CellPyramid;
		mapRoomCellMgr->create(bounds2, 170.0f);

		static_cast<RoomMapMgr*>(mapMgr)->entryToMapRoomCellMgr();
		static_cast<RoomMapMgr*>(mapMgr)->createGlobalCollision();

		generalEnemyMgr->allocateEnemys(mPlayerMode, -1);
		generalEnemyMgr->setupSoundViewerAndBas();

		pelletMgr->setupResources();
		if (getCurrFloor() == 0) {
			playData->mCaveSaveData.mIsWaterwraithAlive = true;
		}
		TexCaster::Mgr::globalInstance();
		mapMgr->mRouteMgr->setCloseAll();
	} else {
		TexCaster::Mgr::globalInstance();
	}

	if (mapMgr->mRouteMgr) {
		testPathfinder = new Pathfinder;
		testPathfinder->create(100, mapMgr->mRouteMgr);
	} else {
		testPathfinder = nullptr;
	}

	if (mapMgr->mRouteMgr) {
		mapMgr->mRouteMgr->refreshWater();
	}

	cellMgr = new CellPyramid;
	BoundBox2d bounds(12800000.0f, 12800000.0f, -12800000.0f, -12800000.0f);
	mapMgr->getBoundBox2d(bounds);
	cellMgr->create(bounds, 108.0f);
	if (!cave) {
		platCellMgr = new CellPyramid;
		platCellMgr->create(bounds, 128.0f);
	}

	if (cave) {
		static_cast<RoomMapMgr*>(mapMgr)->placeObjects();
	}

	Graphics& gfx = *sys->getGfx();
	initViewports(gfx);
	particleMgr->setViewport(gfx);
	particleMgr->start();

	initGenerators();

	itemMgr->initDependency();
	cameraMgr->init(CAMNAVI_Olimar);

	f32 angle = TORADIANS(_aiConstants->mCameraAngle.mData);
	angle     = roundAng(angle + mapMgr->getMapRotation());
	mapMgr->getMapRotation();
	cameraMgr->setCameraAngle(angle, CAMNAVI_Both);

	cameraMgr->controllerUnLock(CAMNAVI_Both);

	onSetSoundScene();
	gameSystem->setFlag(GAMESYS_IsSoundSceneActive);
	if (Farm::farmMgr) {
		Farm::farmMgr->setupSound();
	}
}

/**
 * @note Address: 0x8015145C
 * @note Size: 0xA4
 */
void BaseGameSection::setDrawBuffer(int index)
{
	j3dSys.mDrawBuffer[0] = mOpaqueDrawBuffer->get(index)->mBuffer;
	j3dSys.mDrawBuffer[1] = mTransparentDrawBuffer->get(index)->mBuffer;
}

/**
 * @note Address: 0x80151500
 * @note Size: 0x30
 */
void BaseGameSection::postSetupFloatMemory()
{
	mapMgr->setupJUTTextures();
}

/**
 * @note Address: 0x80151534
 * @note Size: 0x200
 */
void BaseGameSection::createFallPikminSound()
{
	Iterator<Piki> iterator(pikiMgr);
	CI_LOOP(iterator)
	{
		Piki* piki = *iterator;
		piki->movie_begin(false);
		piki->mSoundObj->startFreePikiSound(PSSE_PK_VC_FALL, 0, 0);
	}
}

/**
 * @note Address: 0x80151734
 * @note Size: 0x4
 */
void BaseGameSection::captureRadarmap(Graphics&)
{
}

/**
 * @note Address: N/A
 * @note Size: 0x4
 */
void BaseGameSection::drawRadarmap(Graphics&)
{
	// UNUSED FUNCTION
}

} // namespace Game
