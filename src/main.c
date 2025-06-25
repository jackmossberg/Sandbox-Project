#include "graphics.h"

int main() {
    gl_app* app = glapi_CreateApp(900, 800, 800, 700, "window", true);

    while (!glapi_ShouldAppClose(app)) {
        glapi_RenderApp(app, 0.14, 0.04, 0.2);
    }

    glapi_DestroyApp(app);
    return 0;
}