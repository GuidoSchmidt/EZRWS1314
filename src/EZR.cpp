#include "./utils/Common.h"
#include "./renderer/Context.h"
#include "./renderer/Renderer.h"

int main(void)
{
    renderer::Context* context = new renderer::Context( glm::ivec2(1024, 768) );
    renderer::Renderer* main_renderer;
    main_renderer = renderer::Renderer::instance(*context);

    return 0;
}
