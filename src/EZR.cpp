#include "./utils/Common.h"
#include "./renderer/Context.h"
#include "./renderer/Renderer.h"

using namespace ezr;

int main(void)
{
	renderer::Context* context = new renderer::Context( glm::ivec2(1024, 768) );
	renderer::Renderer* main_renderer;
    main_renderer = renderer::Renderer::instance();
    main_renderer->setRenderContext(*context);
    main_renderer->renderloop();

	return 0;
}
