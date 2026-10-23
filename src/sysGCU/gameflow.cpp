#include "types.h"
#include "GameFlow.h"
#include "BootSection.h"
#include "Demo.h"
#include "Game/SingleGameSection.h"
#include "Game/VsGameSection.h"
#include "JSystem/JKernel/JKRHeap.h"
#include "P2Macros.h"
#include "RootMenuSection.h"
#include "Title.h"

u32 GameFlow::mActiveSectionFlag = SN_Boot;

/**
 * @note Address: 0x804241A4
 * @note Size: 0x2C
 */
GameFlow::GameFlow()
{
	mActiveSectionFlag = SN_Boot;
	mSection           = nullptr;
}

/**
 * @note Address: N/A
 * @note Size: 0x48
 */
GameFlow::~GameFlow()
{
}

/**
 * @note Address: 0x804241D0
 * @note Size: 0xDC
 */
void GameFlow::run()
{
	JKRExpHeap* expHeap;
	JKRHeap* parentHeap;

	while (true) {
		parentHeap = JKRGetCurrentHeap();

		JKRHeap::TState state(parentHeap);
		parentHeap->state_register(&state, -1);
		expHeap = makeExpHeap(parentHeap->getFreeSize(), parentHeap, true);

		setSection();

		mSection->init();
		mSection->run();
		mSection->exit();

		expHeap->destroy();
		parentHeap->becomeCurrentHeap();
	}
}

/**
 * @note Address: 0x804242AC
 * @note Size: 0xC0
 */
void GameFlow::setSection()
{
	JKRHeap::sCurrentHeap->getFreeSize();

	switch (mActiveSectionFlag) {
	case SN_Boot:
		mSection           = new BootSection(JKRGetCurrentHeap());
		mActiveSectionFlag = SN_RootMenu;
		break;
	case SN_RootMenu:
		mSection           = new RootMenuSection(JKRGetCurrentHeap());
		mActiveSectionFlag = SN_MainTitle;
		break;
	default:
		JUT_PANICLINE(188, "Unknown SectionFlag. %d \n", mActiveSectionFlag);
		break;
	}
}

/**
 * @note Address: 0x8042441C
 * @note Size: 0x148
 */
ISection* GameFlow::createSection(JKRHeap* heap)
{
	ISection* section;
	switch (mActiveSectionFlag) {
	case SN_Demo:
		section = new Demo::Section(heap);
		break;
	case SN_MainTitle:
		section = new Title::Section(heap);
		break;
	case SN_SingleGame:
		section = new Game::SingleGameSection(heap);
		break;
	case SN_ChallengeGame:
		section = new Game::VsGameSection(heap, false);
		break;
	case SN_VSGame:
		section = new Game::VsGameSection(heap, true);
		break;
	default:
		section = new Title::Section(heap);
		break;
	}

	mActiveSectionFlag = SN_MainTitle;
	return section;
}
