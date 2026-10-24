#include "Game/BirthMgr.h"
#include "Game/Data.h"
#include "Game/DeathMgr.h"
#include "Game/gameGeneratorCache.h"
#include "Game/gamePlayData.h"
#include "Game/GameSystem.h"
#include "Game/TimeMgr.h"
#include "Game/gameStages.h"

namespace Game {

u32 PlayData::mVersion = 'j009';

/**
 * @note Address: N/A
 * @note Size: 0xC
 */
void PlayData::prepareSave()
{
	// UNUSED FUNCTION
}

/**
 * @note Address: 0x8021CFF4
 * @note Size: 0x740
 */
void PlayData::write(Stream& output)
{
	int startPosition = output.mPosition;
	int courseCount;
	int dataSize;

	ID32 IStack312(PlayData::mVersion);
	IStack312.write(output);

	output.writeInt(mTreasureCount);

	getDebtProgressFlags().writeBytes(output);
	mStoryFlags.writeBytes(output);

	output.writeByte(mLoadType);
	output.writeByte(mHasContainerFlags);
	output.writeByte(mHasBootContainerFlags);
	output.writeByte(mMeetPikminFlags);

	mTekiStatMgr.write(output);

	output.writeInt(gameSystem->mTimeMgr->mDayCount);

	mOlimarData[0].write(output);
	mOlimarData[1].write(output);

	mCaveSaveData.write(output);

	mMailSaveData.write(output);

	mPikiContainer.write(output);

	mZukanStat->write(output);

	mMainCropMemory->write(output);

	mCaveCropMemory->write(output);

	for (int i = 0; i < 2; i++) {
		output.writeInt(mSprayCount[i]);
	}
	for (int i = 0; i < 2; i++) {
		output.writeInt(mBerryCount[i]);
	}

	courseCount = stageList->getCourseCount();
	output.writeInt(courseCount);
	for (int i = 0; i < courseCount; i++) {
		output.writeByte(mBitfieldPerCourse[i]);
		output.writeByte(mGroundOtakaraCollected[i]);
	}

	write_CaveOtakara(output);

	courseCount = stageList->getCourseCount();
	for (int i = 0; i < courseCount; i++) {
		mLimitGen[i].write(output);
	}

	output.writeInt(mPokoCount);
	output.writeInt(mCavePokoCount);


	Game::BirthMgr::write(output);

	Game::DeathMgr::write(output);

	mDeadNaviID.writeBytes(output);
	output.writeFloat(mNaviLifeMax[0]);
	output.writeFloat(mNaviLifeMax[1]);


	mDemoFlags.write(output);
	mFindItemFlags.write(output);

	courseCount = stageList->getCourseCount();
	for (int i = 0; i < courseCount; i++) {
		output.writeByte(mGroundOtakaraCollectedOld[i]);
	}
	output.writeInt(mPokoCountOld);
	write_CaveOtakara_Old(output);

	for (int i = FirstPikmin; i < StoredPikiCount + 1; i++) {
		output.writeInt(mPikminYesterday[i]);
		output.writeInt(mPikminToday[i]);
	}

	generatorCache->write(output);

	mBeforeSaveDelegate         = nullptr;
}

/**
 * @note Address: 0x8021D734
 * @note Size: 0x404
 */
void PlayData::read(Stream& input)
{
	int startPosition = input.mPosition;
	mOsTime           = OSGetTime();
	ID32 version;
	version.read(input);
	int id        = version.getID();
	u32 versionID = id;

	if (versionID >= 'j004') {
		mTreasureCount = input.readInt();
	} else {
		mTreasureCount = 0;
	}

	if (versionID >= 'j001') {
		getDebtProgressFlags().readBytes(input);
	}

	mStoryFlags.readBytes(input);
	mLoadType              = input.readByte();
	mHasContainerFlags     = input.readByte();
	mHasBootContainerFlags = input.readByte();

	if (versionID >= 'j007') {
		mMeetPikminFlags = input.readByte();
	}

	mTekiStatMgr.read(input);
	gameSystem->mTimeMgr->mDayCount = input.readInt();
	mOlimarData[0].read(input);
	mOlimarData[1].read(input);
	mCaveSaveData.read(input, versionID);
	mMailSaveData.read(input);
	mPikiContainer.read(input);
	mZukanStat->read(input);
	mMainCropMemory->read(input);
	mCaveCropMemory->read(input);
	for (int i = 0; i < 2; i++) {
		mSprayCount[i] = input.readInt();
	}
	for (int i = 0; i < 2; i++) {
		mBerryCount[i] = input.readInt();
	}

	int courseNum = stageList->getCourseCount();
	int cardNum   = input.readInt();
	JUT_ASSERTLINE(633, cardNum == courseNum, "SaveData ERROR : CourseNum=%d (card num=%d)\n", courseNum, cardNum);
	for (int i = 0; i < courseNum; i++) {
		mBitfieldPerCourse[i] = input.readByte();
		if (versionID >= 'j005') {
			mGroundOtakaraCollected[i] = input.readByte();
		}
	}

	read_CaveOtakara(input);
	courseNum = stageList->getCourseCount();
	for (int i = 0; i < courseNum; i++) {
		mLimitGen[i].read(input);
	}

	mPokoCount     = input.readInt();
	mCavePokoCount = input.readInt();

	BirthMgr::read(input);
	DeathMgr::read(input);
	mDeadNaviID.readBytes(input);

	if (versionID >= 'j006') {
		mNaviLifeMax[0] = input.readFloat();
		mNaviLifeMax[1] = input.readFloat();
	}

	mDemoFlags.read(input);
	mFindItemFlags.read(input);

	if (versionID >= 'j008') {
		courseNum = stageList->getCourseCount();
		for (int i = 0; i < courseNum; i++) {
			mGroundOtakaraCollectedOld[i] = input.readByte();
		}
		mPokoCountOld = input.readInt();
		read_CaveOtakara_Old(input);
	}

	for (int i = FirstPikmin; i < StoredPikiCount + 1; i++) {
		mPikminYesterday[i] = input.readInt();
		mPikminToday[i]     = input.readInt();
	}

	generatorCache->read(input);
}

/**
 * read__Q34Game8PlayData8LimitGenFR6Stream
 * @note Address: 0x8021DB38
 * @note Size: 0x44
 */
void PlayData::LimitGen::read(Stream& input)
{
	mNonLoops.read(input);
	mLoops.read(input);
}

/**
 * write__Q34Game8PlayData8LimitGenFR6Stream
 * @note Address: 0x8021DB7C
 * @note Size: 0x78
 */
void PlayData::LimitGen::write(Stream& output)
{
	mNonLoops.write(output);
	mLoops.write(output);
}

/**
 * write__Q24Game11KindCounterFR6Stream
 * @note Address: N/A
 * @note Size: 0xC8
 */
void KindCounter::write(Stream& output)
{
	output.writeShort(mNumKinds);
	for (int i = 0; i < mNumKinds; i++) {
		int kind = mKinds[i];
		output.writeByte(kind);
	}
}

/**
 * read__Q24Game11KindCounterFR6Stream
 * @note Address: N/A
 * @note Size: 0x98
 */
void KindCounter::read(Stream& input)
{
	u16 count = input.readShort();
	JUT_ASSERTLINE(794, count == (int)mNumKinds, "read count %d : mNumKinds %d : mismatch !\n", count, mNumKinds);
	for (int i = 0; i < mNumKinds; i++) {
		mKinds[i] = input.readByte();
	}
}

/**
 * write__Q24Game16PelletCropMemoryFR6Stream
 * @note Address: 0x8021DBF4
 * @note Size: 0x208
 */
void PelletCropMemory::write(Stream& output)
{
	mOtakara.write(output);

	mItem.write(output);

	mCarcass.write(output);
}

/**
 * read__Q24Game16PelletCropMemoryFR6Stream
 * @note Address: 0x8021DDFC
 * @note Size: 0x150
 */
void PelletCropMemory::read(Stream& input)
{
	mOtakara.read(input);
	mItem.read(input);
	mCarcass.read(input);
}

/**
 * write__Q24Game10OlimarDataFR6Stream
 * @note Address: 0x8021DF4C
 * @note Size: 0x7C
 */
void OlimarData::write(Stream& output)
{
	mFlags.writeBytes(output);
}

/**
 * read__Q24Game10OlimarDataFR6Stream
 * @note Address: 0x8021DFC8
 * @note Size: 0x5C
 */
void OlimarData::read(Stream& input)
{
	mFlags.readBytes(input);
}

/**
 * write__Q24Game12CaveSaveDataFR6Stream
 * @note Address: 0x8021E024
 * @note Size: 0x198
 */
void CaveSaveData::write(Stream& output)
{
	mCavePikis.write(output);

	output.writeFloat(mDayTime);

	output.writeByte(mCourseIdx);

	mCurrentCaveID.write(output);

	output.writeByte(mCurrentFloor);

	output.writeByte(mIsWaterwraithAlive);

	output.writeFloat(mWaterwraithTimer);

	output.writeByte(mActiveNaviID);
}

/**
 * @note Address: 0x8021E1BC
 * @note Size: 0xD0
 */
void CaveSaveData::read(Stream& input, u32 size)
{
	mCavePikis.read(input);
	mDayTime   = input.readFloat();
	mCourseIdx = (s8)input.readByte();
	mCurrentCaveID.read(input);
	mCurrentFloor       = (s8)input.readByte();
	mIsWaterwraithAlive = input.readByte() != 0;
	mWaterwraithTimer   = input.readFloat();
	if ('j009' <= size) {
		mActiveNaviID = input.readByte();
	}
	return;
}
} // namespace Game
