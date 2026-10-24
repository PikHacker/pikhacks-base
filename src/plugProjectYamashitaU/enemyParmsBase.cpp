#include "Game/EnemyParmsBase.h"
#include "JSystem/JKernel/JKRArchive.h"
#include "stream.h"

namespace Game {
/**
 * @note Address: 0x8012A244
 * @note Size: 0x94
 */
bool EnemyParmsBase::loadSettingFile(JKRArchive* archive, char* name)
{
	void* handle = archive->getResource(name);
	if (handle) {
		RamStream stream(handle, -1);
		stream.setMode(STREAM_MODE_TEXT, 1);
		read(stream);
		return true;
	}

	return false;
}

} // namespace Game
