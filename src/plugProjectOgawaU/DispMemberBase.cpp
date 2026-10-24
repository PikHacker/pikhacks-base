#include "Screen/Enums.h"
#include "types.h"
#include "og/Screen/ogScreen.h"

namespace og {
namespace Screen {

/**
 * @note Address: 0x8030F31C
 * @note Size: 0x80
 */
bool DispMemberBase::isID(u64 memberID)
{
	u64 thisMemberID = getMemberID();
	if (memberID == thisMemberID) {
		return true;
	}
	return false;
}

/**
 * @note Address: 0x8030F39C
 * @note Size: 0x40
 */
void DispMemberBase::getMemberName(char* outName)
{
	og::Screen::TagToName(getMemberID(), outName);
}

/**
 * @note Address: 0x8030F3DC
 * @note Size: 0xE4
 */
bool DispMemberBase::setSubMember(og::Screen::DispMemberBase* newSubMember)
{
	DispMemberBase* potentialParent = this;
	newSubMember->mSubMember        = nullptr;
	for (int i = 10; i != 0; i--) {
		if (potentialParent->mSubMember == nullptr) {
			potentialParent->mSubMember = newSubMember;
			return true;
		}
		potentialParent = potentialParent->mSubMember;
	}
	return false;
}

/**
 * @note Address: 0x8030F4C0
 * @note Size: 0x10C
 */
DispMemberBase* DispMemberBase::getSubMember(u64 memberID)
{
	DispMemberBase* member = this;
	for (int i = 0; i < 10; i++) {
		DispMemberBase* subMember = member->mSubMember;

		if (subMember == nullptr) {
			break;
		}

		if (subMember->isID(memberID)) {
			return subMember;
		}
		member = subMember;
	}
	return nullptr;
}

/**
 * @note Address: 0x8030F5CC
 * @note Size: 0x34
 */
void DispMemberBase::setSubMemberAll()
{
	mSubMember = nullptr;
	doSetSubMemberAll();
}

} // namespace Screen
} // namespace og
