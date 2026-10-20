#include "Game/Entities/Item.h"
#include "System.h"

namespace Game {

template <typename T, typename T2>
inline void GenerateMgr(T2& mgr)
{
	mgr = new T();
	mgr->loadResources();
	itemMgr->addMgr(mgr);
}

/**
 * @note Address: 0x801E4B98
 * @note Size: 0x664
 */
void ItemMgr::createManagers(u32 flags)
{
	bool isGround = (flags >> 1) & 1;
	bool isCave   = flags & 1;

	GenerateMgr<ItemBarrel::Mgr>(ItemBarrel::mgr);
	GenerateMgr<ItemUjamushi::Mgr>(ItemUjamushi::mgr);

	if (isGround) {
		ItemWeed::mgr = new ItemWeed::Mgr;
		ItemWeed::mgr->loadResources();
		itemMgr->addMgr(ItemWeed::mgr);
	} else {
		ItemWeed::mgr = nullptr;
	}

	GenerateMgr<ItemDownFloor::Mgr>(ItemDownFloor::mgr);
	GenerateMgr<ItemTreasure::Mgr>(ItemTreasure::mgr);
	GenerateMgr<ItemPikihead::Mgr>(ItemPikihead::mgr);

	if (isGround) {
		ItemPlant::mgr = new ItemPlant::Mgr;
		ItemPlant::mgr->loadResources();
		itemMgr->addMgr(ItemPlant::mgr);
	} else {
		ItemPlant::mgr = nullptr;
	}

	if (isGround) {
		ItemRock::mgr = new ItemRock::Mgr;
		ItemRock::mgr->loadResources();
		itemMgr->addMgr(ItemRock::mgr);
	} else {
		ItemRock::mgr = nullptr;
	}

	GenerateMgr<ItemHoney::Mgr>(ItemHoney::mgr);

	ItemOnyon::mgr = new ItemOnyon::Mgr;
	itemMgr->addMgr(ItemOnyon::mgr);
	ItemOnyon::mgr->init();

	if (isCave) {
		ItemHole::mgr = new ItemHole::Mgr;
		ItemHole::mgr->loadResources();
		itemMgr->addMgr(ItemHole::mgr);
	} else {
		ItemHole::mgr = nullptr;
	}

	if (isGround) {
		ItemCave::mgr = new ItemCave::Mgr;
		ItemCave::mgr->loadResources();
		itemMgr->addMgr(ItemCave::mgr);
	} else {
		ItemCave::mgr = nullptr;
	}

	if (isCave) {
		ItemBigFountain::mgr = new ItemBigFountain::Mgr;
		ItemBigFountain::mgr->loadResources();
		itemMgr->addMgr(ItemBigFountain::mgr);
	} else {
		ItemBigFountain::mgr = nullptr;
	}

	if (isGround) {
		ItemBridge::mgr = new ItemBridge::Mgr;
		ItemBridge::mgr->loadResources();
		itemMgr->addMgr(ItemBridge::mgr);
	} else {
		ItemBridge::mgr = nullptr;
	}

	itemGateMgr = new ItemGateMgr();
	itemMgr->addMgr(itemGateMgr);

	if (isGround) {
		ItemDengekiGate::mgr = new ItemDengekiGate::Mgr;
		itemMgr->addMgr(ItemDengekiGate::mgr);
	} else {
		ItemDengekiGate::mgr = nullptr;
	}
}

/**
 * @note Address: 0x801E51FC
 * @note Size: 0x48
 */
void ItemMgr::clearGlobalPointers()
{
	ItemBarrel::mgr      = nullptr;
	ItemUjamushi::mgr    = nullptr;
	ItemWeed::mgr        = nullptr;
	ItemDownFloor::mgr   = nullptr;
	ItemTreasure::mgr    = nullptr;
	ItemPikihead::mgr    = nullptr;
	ItemPlant::mgr       = nullptr;
	ItemRock::mgr        = nullptr;
	ItemHoney::mgr       = nullptr;
	ItemOnyon::mgr       = nullptr;
	ItemHole::mgr        = nullptr;
	ItemCave::mgr        = nullptr;
	ItemBigFountain::mgr = nullptr;
	ItemBridge::mgr      = nullptr;
	itemGateMgr          = nullptr;
	ItemDengekiGate::mgr = nullptr;
}

/**
 * @note Address: 0x801E5244
 * @note Size: 0xE8
 */
void ItemMgr::killAllExceptOnyonMgr()
{
	if (ItemPlant::mgr) {
		ItemPlant::mgr->killAll();
		if (gameSystem) {
			gameSystem->detachObjectMgr(ItemPlant::mgr);
			itemMgr->delNode(ItemPlant::mgr);
		}
		ItemPlant::mgr = nullptr;
	}

	if (ItemRock::mgr) {
		ItemRock::mgr->killAll();
		if (gameSystem) {
			gameSystem->detachObjectMgr(ItemRock::mgr);
			itemMgr->delNode(ItemRock::mgr);
		}
		ItemRock::mgr = nullptr;
	}

	if (ItemHoney::mgr) {
		ItemHoney::mgr->killAll();
		if (gameSystem) {
			gameSystem->detachObjectMgr(ItemHoney::mgr);
			itemMgr->delNode(ItemHoney::mgr);
		}
		ItemHoney::mgr = nullptr;
	}
}

} // namespace Game
