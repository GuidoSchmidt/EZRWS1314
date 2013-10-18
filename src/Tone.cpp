#include "./renderer/Context.h"
#include "./utils/Common.h"

int main(void)
{
	tone::renderer::Context* context = new tone::renderer::Context( glm::ivec2(1024, 768) );

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(1, 1, 1, 1);

    while(context->isLive())
	{

		context->swapBuffers();
	}

	return 0;
}
