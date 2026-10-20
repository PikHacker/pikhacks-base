#include "Game/SingleGame.h"
#include "JSystem/JKernel/JKRHeap.h"

namespace Game {
namespace SingleGame {

/**
 * @note Address: 0x8021EED8
 * @note Size: 0x8C
 */
MovieState::MovieState()
    : State(SGS_Movie)
{
	mController = new Controller(JUTGamePad::PORT_0);
	mMovieHeap  = nullptr;
	mBackupHeap = nullptr;
}

/**
 * @note Address: 0x8021EF64
 * @note Size: 0x290
 */
void MovieState::init(SingleGameSection* gs, StateArg* arg)
{
	mMovieHeap = nullptr;

	mBackupHeap = nullptr;
	mBackupHeap = JKRGetCurrentHeap();
	mBackupHeap->getFreeSize();

	mMovieHeap = JKRExpHeap::create(mBackupHeap->getFreeSize(), mBackupHeap, true);
	mMovieHeap->becomeCurrentHeap();
	mMovieHeap->getFreeSize();
	mMoviePlayer = new THPPlayer;
	mMoviePlayer->init(nullptr);
	mIsMovieLoaded = false;
	mMoviePlayer->load(THPPlayer::OPENING_GameStart);
}

/**
 * @note Address: 0x8021F1F4
 * @note Size: 0x114
 */
void MovieState::exec(SingleGameSection* gs)
{
	if (mMovieHeap) {
		mMoviePlayer->update();
		switch (mIsMovieLoaded) {
		case false:
			if (mMoviePlayer->isFinishLoading()) {
				mIsMovieLoaded = true;
				mMoviePlayer->play();
			}
			break;
		case true:
			if ((mController->getButtonDown() & Controller::PRESS_START) || mMoviePlayer->isFinishPlaying()) { // skip the movie with start
				gs->mDisplayWiper = gs->mWipeInFader;
				gs->mWipeInFader->start(4.0f);
				gs->mCurrentCourseInfo = stageList->getCourseInfo(0);
				LoadArg arg(MapEnter_NewGame, false, true, false);
				transit(gs, SGS_Load, &arg);
			}
			break;
		}
	}
}

/**
 * @note Address: 0x8021F308
 * @note Size: 0x40
 */
void MovieState::draw(SingleGameSection* gs, Graphics& gfx)
{
	if (mMovieHeap) {
		mMoviePlayer->draw(gfx);
	}
}

/**
 * @note Address: 0x8021F348
 * @note Size: 0x48
 */
void MovieState::cleanup(SingleGameSection* gs)
{
	mMovieHeap->freeAll();
	mMovieHeap->destroy();
	mMovieHeap = nullptr;
	mBackupHeap->becomeCurrentHeap();
}

} // namespace SingleGame
} // namespace Game
