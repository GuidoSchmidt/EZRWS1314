#include "MipMapExtractionPass.h"


MipMapExtractionPass::MipMapExtractionPass(SlimQuad* pQuad, int pWidth, int pHeight)
: ISlimRenderPass(pQuad)
{
	quad = pQuad;
	width = pWidth;
	height = pHeight;

	renderPassShader = new SlimShader(RESOURCES_PATH "/shader_source/", "post.vert.glsl", "mipMapExtraction.frag.glsl");

	screenSizeUniform = glGetUniformLocation(renderPassShader->shaderProgram, "screenSize");
	inputUniform = glGetUniformLocation(renderPassShader->shaderProgram, "inputTexture");
	firstPassUniform = glGetUniformLocation(renderPassShader->shaderProgram, "firstPass");

	//fbos = std::vector;
	nPasses = 0;
	float w = width;
	float h = height;
	do {
		w = ceil(w / 2.0);
		h = ceil(h / 2.0);
		fbos.push_back(new SlimFBO(w, h, 1, false, GL_NEAREST));
		nPasses++;
	} while (w > 1);
}


MipMapExtractionPass::~MipMapExtractionPass() {
	//delete renderPassShader;
}

void MipMapExtractionPass::doExecute() {

	inputTexture = inputFBOs[0]->texPointer[0];
	GLint x, y;
	renderPassShader->enable();

	for (int pass = 0; pass < nPasses; )
	{
		outputFBO = fbos[pass];
		x = (GLint)outputFBO->xRes;
		y = (GLint)outputFBO->yRes;

		outputFBO->write();
			if (pass == 0)
				glUniform1f(firstPassUniform, 1.0);
			else
				glUniform1f(firstPassUniform, 0.0);

			glUniform2i(screenSizeUniform, x, y);

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, inputTexture);
			glUniform1i(inputUniform, 0);

			glViewport(0, 0, x, y);
			quad->draw();

			pass++;
			inputTexture = fbos[pass-1]->texPointer[0];
		outputFBO->unbind();
	}
	outputTexture = fbos[nPasses-1]->texPointer[0];
	renderPassShader->disable();
	glViewport(0, 0, width, height);
}

