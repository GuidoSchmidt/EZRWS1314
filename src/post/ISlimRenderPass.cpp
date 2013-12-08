#include "ISlimRenderPass.h"
//#include "../Globals.h"

ISlimRenderPass::ISlimRenderPass(SlimQuad* pQuad) { //Bar* pBar) {
	quad = pQuad;
}

//SlimFBO* operator<<(SlimFBO fbo, ISlimRenderPass* renderPass)
//{
//	renderPass->outputFBO = &fbo;
//	renderPass->doExecute();
//	return renderPass->outputFBO;
//}
//
//ISlimRenderPass* operator<<(ISlimRenderPass* renderPass, SlimFBO fbo)
//{
//	renderPass->inputFBOs.push_back(&fbo);
//	return renderPass;
//}