#include "pch.h"
#include "gmbindparticle.h"

#include "gm/gmMachine.h"
#include "gm/gmThread.h"
#include "gm/gmVariable.h"

#include "game/particlesystem.h"

#include "gmbindfuncgen.h"
#include "imgui.h"
#include "renderer/renderer.h"

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
		Renderer::Ref().Submit(*ps);
		return GM_OK;
	}

	GM_GEN_MEMFUNC_VOID_VOID(ParticleSystem, Update)
	GM_GEN_MEMFUNC_VOID_VOID(ParticleSystem, Gui)

	GM_SETDOT_FUNC_BEGIN(ParticleSystem)
		GM_SETDOT_PARAM_FLOAT("Life", m_LifeSpan)
		GM_SETDOT_PARAM_FLOAT("LifeVariance", m_LifeSpanVaraince)
		GM_SETDOT_PARAM_VEC2("Position", m_StartPosition)
		GM_SETDOT_PARAM_VEC2("PositionVariance", m_StartPositionVariance)
		GM_SETDOT_PARAM_VEC2("StartScale", m_StartScale)
		GM_SETDOT_PARAM_VEC2("StartScaleVariance", m_StartScaleVariance)
		GM_SETDOT_PARAM_VEC2("EndScale", m_EndScale)
		GM_SETDOT_PARAM_VEC2("EndScaleVariance", m_EndScaleVariance)
		GM_SETDOT_PARAM_VEC2("Velocity", m_Velocity)
		GM_SETDOT_PARAM_VEC2("VelocityVariance", m_VelocityVariance)
		GM_SETDOT_PARAM_VEC2("Acceleration", m_Accel)
		GM_SETDOT_PARAM_VEC2("AttractorPosition", m_AttractorPosition)
		GM_SETDOT_PARAM_FLOAT("AttractorStrenght", m_AttractorStrenght)
		GM_SETDOT_PARAM_VEC3("StartColor", m_StartColor)
		GM_SETDOT_PARAM_VEC3("StartColorVariance", m_StartColorVariance)
		GM_SETDOT_PARAM_VEC3("EndColor", m_EndColor)
		GM_SETDOT_PARAM_VEC3("EndColorVariance", m_EndColorVariance)
		GM_SETDOT_PARAM_FLOAT("StartAlpha", m_StartAlpha)
		GM_SETDOT_PARAM_FLOAT("StartAlphaVariance", m_StartAlphaVariance)
		GM_SETDOT_PARAM_FLOAT("EndAlpha", m_EndAlpha)
		GM_SETDOT_PARAM_FLOAT("EndAlphaVariance", m_EndAlphaVariance)
	GM_SETDOT_FUNC_END(ParticleSystem)

	GM_GETDOT_FUNC_BEGIN(ParticleSystem)
		GM_GETDOT_PARAM_FLOAT("Life", m_LifeSpan)
		GM_GETDOT_PARAM_FLOAT("LifeVariance", m_LifeSpanVaraince)
		GM_GETDOT_PARAM_VEC2("Position", m_StartPosition)
		GM_GETDOT_PARAM_VEC2("PositionVariance", m_StartPositionVariance)
		GM_GETDOT_PARAM_VEC2("StartScale", m_StartScale)
		GM_GETDOT_PARAM_VEC2("StartScaleVariance", m_StartScaleVariance)
		GM_GETDOT_PARAM_VEC2("EndScale", m_EndScale)
		GM_GETDOT_PARAM_VEC2("EndScaleVariance", m_EndScaleVariance)
		GM_GETDOT_PARAM_VEC2("Velocity", m_Velocity)
		GM_GETDOT_PARAM_VEC2("VelocityVariance", m_VelocityVariance)
		GM_GETDOT_PARAM_VEC2("Acceleration", m_Accel)
		GM_GETDOT_PARAM_VEC2("AttractorPosition", m_AttractorPosition)
		GM_GETDOT_PARAM_FLOAT("AttractorStrenght", m_AttractorStrenght)
		GM_GETDOT_PARAM_VEC3("StartColor", m_StartColor)
		GM_GETDOT_PARAM_VEC3("StartColorVariance", m_StartColorVariance)
		GM_GETDOT_PARAM_VEC3("EndColor", m_EndColor)
		GM_GETDOT_PARAM_VEC3("EndColorVariance", m_EndColorVariance)
		GM_GETDOT_PARAM_FLOAT("StartAlpha", m_StartAlpha)
		GM_GETDOT_PARAM_FLOAT("StartAlphaVariance", m_StartAlphaVariance)
		GM_GETDOT_PARAM_FLOAT("EndAlpha", m_EndAlpha)
		GM_GETDOT_PARAM_FLOAT("EndAlphaVariance", m_EndAlphaVariance)
	GM_GETDOT_FUNC_END(ParticleSystem)
}

GM_REG_MEM_BEGIN(ParticleSystem)
	GM_REG_HANDLED_DESTRUCTOR(ParticleSystem)
	GM_REG_MEMFUNC(ParticleSystem, Draw)
	GM_REG_MEMFUNC(ParticleSystem, Update)
	GM_REG_MEMFUNC(ParticleSystem, Gui)
	GM_REG_SET_GET_DOT_FUNC(ParticleSystem)
GM_REG_MEM_END(ParticleSystem)

GM_BIND_DEFINE(ParticleSystem)
