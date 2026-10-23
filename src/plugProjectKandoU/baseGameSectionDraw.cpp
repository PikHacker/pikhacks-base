#include "Game/BaseGameSection.h"
#include "Game/GameSystem.h"
#include "Game/GameLight.h"

#include "Sys/DrawBuffers.h"

#include "JSystem/JUtility/JUTTexture.h"
#include "JSystem/J3D/J3DSys.h"

#include "ParticleMgr.h"
#include "IDelegate.h"
#include "System.h"
#include "Light.h"

namespace Game {
/**
 * @note Address: 0x802398D8
 * @note Size: 0x1F4
 */
void BaseGameSection::newdraw_draw3D_all(Graphics& gfx)
{
	// Setup viewport callback to be newdraw_drawAll
	Delegate1<BaseGameSection, Viewport*> vpDelegate(this, &BaseGameSection::newdraw_drawAll);
	gfx.mapViewport(&vpDelegate);

	// Initialise both draw buffers for the frame
	mOpaqueDrawBuffer->frameInitAll();
	mTransparentDrawBuffer->frameInitAll();

	if (!gameSystem->isMultiplayerMode()) {
		particleMgr->setXfb(mXfbImage->mTexInfo);
	}

	// Draw particles for both viewports
	drawParticle(gfx, PLAYER1_VIEWPORT);
	drawParticle(gfx, PLAYER2_VIEWPORT);

	// Draw counters for both viewports
	// (Life gauge & Carry info)
	mLightMgr->set(gfx);
	Viewport* vp = gfx.getViewport(PLAYER1_VIEWPORT);
	if (vp && vp->viewable()) {
		gfx.mCurrentViewport = vp;
		directDrawPost(gfx, vp);
	}

	mLightMgr->set(gfx);
	vp = gfx.getViewport(PLAYER2_VIEWPORT);
	if (vp && vp->viewable()) {
		gfx.mCurrentViewport = vp;
		directDrawPost(gfx, vp);
	}
}

/**
 * @note Address: 0x80239ACC
 * @note Size: 0x360
 */
void BaseGameSection::newdraw_drawAll(Viewport* vp)
{
	Graphics& gfx = *sys->mGfx;

	doSetView(vp->mVpId);
	vp->setJ3DViewMtx(true)->setViewCalcModeImm();
	doViewCalc();
	vp->setViewport();
	vp->setProjection();

	j3dSys.drawInit();

	mLightMgr->set(gfx);
	mOpaqueDrawBuffer->get(DB_FirstLayer)->draw();
	mOpaqueDrawBuffer->get(DB_MapLayer)->draw();
	mOpaqueDrawBuffer->get(DB_FarmLayer)->draw();
	mOpaqueDrawBuffer->get(DB_PikiLayer)->draw();
	mOpaqueDrawBuffer->get(DB_NormalLayer)->draw();
	doSimpleDraw(vp);
	mLightMgr->set(gfx);
	mTransparentDrawBuffer->get(DB_PikiLayer)->draw();
	mTransparentDrawBuffer->get(DB_NormalLayer)->draw();
	mLightMgr->mFogMgr->off(gfx);
	mOpaqueDrawBuffer->get(DB_NormalFogOffLayer)->draw();
	mTransparentDrawBuffer->get(DB_NormalFogOffLayer)->draw();
	mLightMgr->mFogMgr->set(gfx);

	gfx.setToken("direct");

	j3dSys.drawInit();
	directDraw(gfx, vp);

	Game::shadowMgr->draw(gfx, vp->mVpId);
	vp->setViewport();
	vp->setProjection();

	mOpaqueDrawBuffer->get(DB_PostShadowLayer)->draw();
	mTransparentDrawBuffer->get(DB_PostShadowLayer)->draw();

	// only capture normal xfb image if the 2d one doesnt exist
	if (!mXfbTexture2d && (mXfbFlags & 3) == 0) {
		mXfbImage->capture(mXfbBoundsX, mXfbBoundsY, GX_TF_RGB565, true, 0);
	}
	mLightMgr->set(gfx);
	mLightMgr->mFogMgr->off(gfx);

	mOpaqueDrawBuffer->get(DB_ObjectLastLayer)->draw();
	mTransparentDrawBuffer->get(DB_ObjectLastLayer)->draw();

	vp->setJ3DViewMtx(true);

	mLightMgr->mFogMgr->off(gfx);
	mOpaqueDrawBuffer->get(DB_PostRenderLayer)->draw();
	mTransparentDrawBuffer->get(DB_PostRenderLayer)->draw();
	mLightMgr->mFogMgr->set(gfx);
	vp->setJ3DViewMtx(true);

	mTransparentDrawBuffer->get(DB_MapLayer)->draw();
	vp->setJ3DViewMtx(false);
}

} // namespace Game
