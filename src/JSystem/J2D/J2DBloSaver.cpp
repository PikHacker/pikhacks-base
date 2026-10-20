#include "JSystem/J2D/J2DBloSaver.h"
#include "JSystem/J2D/J2DIndBlock.h"
#include "JSystem/J2D/J2DTevBlock.h"
#include "JSystem/J2D/J2DTypes.h"
#include "types.h"

J2DBloSaver::CTextureNameConnect J2DBloSaver::TextureNameConnect;

/**
 * @note Address: N/A
 * @note Size: 0x1158
 */
void J2DBloSaver::writeMaterialBlock(const MaterialName*, J2DMaterial**, u16, const ResTIMGName*, u16)
{
	// obviously much much more than this, but need to spawn weak functions
	J2DTevBlock* block;
	block->setTexNo(0, 0);
	u16 texNo = block->getTexNo(0);
}

/**
 * @note Address: N/A
 * @note Size: 0x44C
 */
void J2DMaterial_SaveBlock::countParts(J2DMaterial**, u16)
{
	// obviously much much more than this, but need to spawn weak functions
	J2DTevBlock* block;
	block->getFontNo();
	block->getTevOrder(0);
	block->getTevSwapModeTable(0);
}

/**
 * @note Address: N/A
 * @note Size: 0x3FC
 */
void J2DMaterial_SaveBlock::setOffset(J2DMaterial**, u16, JUTNameTab*)
{
	// obviously much much more than this, but need to spawn weak functions
	J2DIndBlock* block;
	block->getIndTexStageNum();
}

/**
 * @note Address: 0x80049518
 * @note Size: 0x50
 */
J2DBloSaver::CTextureNameConnect::~CTextureNameConnect()
{
	clear();
}

/**
 * @note Address: 0x80049568
 * @note Size: 0x70
 */
void J2DBloSaver::CTextureNameConnect::clear()
{
	TNC* element;
	TNC* next;
	for (element = mElements; element != nullptr; element = next) {
		next = element->mNext;
		delete element;
	}
	mElements = nullptr;
}
