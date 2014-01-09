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
	//virtual ~ISlimRenderPass() {};
	virtual void doExecute() = 0;
	ISlimRenderPass(SlimQuad* pQuad);



	SlimFBO* outputFBO;
	std::vector<SlimFBO*> inputFBOs;
	SlimShader* renderPassShader;
	SlimQuad* quad;

	int width;
	int height;

protected:
	ISlimRenderPass() {}

};

#endif
