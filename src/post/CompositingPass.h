#ifndef CompositingPass_H_
#define CompositingPass_H_

#include "ISlimRenderPass.h"

class CompositingPass : public ISlimRenderPass
{
public:
	CompositingPass(SlimQuad* pQuad, int pWidth, int pHeight);
	virtual ~CompositingPass();

	virtual void doExecute();
	GLint bloomAmountUniform;

	GLint colorTextureUniform;
	GLint bloomTextureUniform;


	// + weitere

	float param_bloomAmount;
};

#endif /* CompositingPass_H_ */
