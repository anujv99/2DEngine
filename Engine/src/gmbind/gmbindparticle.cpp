#include "pch.h"
#include "gmbindparticle.h"

#include "gm/gmMachine.h"
#include "gm/gmThread.h"

#include "game/particlesystem.h"
#include "renderer/particlerenderer.h"

#include "gmbindfuncgen.h"

using namespace prev;

GM_REG_NAMESPACE(ParticleSystem) {

	GM_MEMFUNC_DECL(CreateParticleSystem) {
		GM_CHECK_NUM_PARAMS(1);
		GM_CHECK_INT_PARAM(maxNumParticles, 0);
		StrongHandle<ParticleSystem> particleSystem = new ParticleSystem(maxNumParticles);
		GM_PUSH_USER_HANDLED(ParticleSystem, particleSystem.Get());
		return GM_OK;
	}

	GM_MEMFUNC_DECL(Draw) {
		GM_CHECK_NUM_PARAMS(0);
		GM_GET_THIS_PTR(ParticleSystem, ps);
		ParticleRenderer::Ref().Render(*ps);
		return GM_OK;
	}

	GM_GEN_MEMFUNC_VOID_VOID(ParticleSystem, Update)
	GM_GEN_MEMFUNC_VOID_VOID(ParticleSystem, Gui)
}

GM_REG_MEM_BEGIN(ParticleSystem)
GM_REG_HANDLED_DESTRUCTOR(ParticleSystem)
GM_REG_MEMFUNC(ParticleSystem, Draw)
GM_REG_MEMFUNC(ParticleSystem, Update)
GM_REG_MEMFUNC(ParticleSystem, Gui)
GM_REG_MEM_END(ParticleSystem)
GM_BIND_DEFINE(ParticleSystem)