#include "JSystem/JUtility/JUTException.h"
#include "LoadResource.h"
#include "og/newScreen/Floor.h"
#include "og/newScreen/ogUtil.h"
#include "og/Screen/ogScreen.h"
#include "Screen/Enums.h"
#include "System.h"

namespace og {
namespace newScreen {

/**
 * @note Address: 0x8031BE60
 * @note Size: 0x3C
 */
Floor::Floor()
{
}

/**
 * @note Address: 0x8031BE9C
 * @note Size: 0x13C
 */
void Floor::doUserCallBackFunc(Resource::MgrCommand*)
{
	if (mDispMember->isID(MEMBER_FLOOR)) {
#if defined(VERSION_PAL)
		const char* path = (sys->getLanguage() == System::LANG_Japanese) ? "res_floor_name_jpn.szs" : "res_floor_name_eng_pal.szs";
#else
		const char* path = (sys->mRegion == System::LANG_Japanese) ? "res_floor_name_jpn.szs" : "res_floor_name_eng_pal.szs";
#endif
		og::newScreen::makeLanguageResName(mName, path);
		LoadResource::Arg loadArg(mName);
		LoadResource::Node* resourceNode = gLoadResourceMgr->mountArchive(loadArg);

		JKRArchive* archive = nullptr;
		if (resourceNode) {
			archive = resourceNode->mArchive;

			if (resourceNode->mArchive == nullptr) {
				JUT_PANICLINE(91, "arc is NULL!!\n");
			}

		} else {
			JUT_PANICLINE(96, "node is NULL!!\n");
		}

		registObj(new ObjFloor("floor screen"), archive);
		setColorBG(0, 0, 0, 0);

	} else {
		JUT_PANICLINE(111, "DispMember is not 'FLOOR'!");
	}
}

/**
 * @note Address: 0x8031BFD8
 * @note Size: 0x4
 */
void Floor::doCreateObj(JKRArchive*)
{
}

} // namespace newScreen
} // namespace og
