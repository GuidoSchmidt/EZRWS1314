#ifndef SLIMPASS_H_
#define SLIMPASS_H_



#include "SlimShader.h"
#include "SlimFBO.h"
#include "SlimQuad.h"


#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>



class ISlimRenderPass
{
public:
	virtual ~ISlimRenderPass() {};
	ISlimRenderPass(SlimQuad* pQuad);
	virtual void doExecute() = 0;

	SlimFBO* outputFBO;
	std::vector<SlimFBO*> inputFBOs;
	SlimShader* renderPassShader;
	SlimQuad* quad;

	int width;
	int height;
};

#endif
