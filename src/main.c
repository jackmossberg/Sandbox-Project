#include "graphics.h"

int main() {
    gl_app* app = glapi_CreateApp(800, 650, 800, 650, "window", false);

    while (!glapi_ShouldAppClose(app)) {
        glapi_RenderApp(app, 0.12f, 0.11f, 0.04f);
    }

    glapi_DestroyApp(app);
    return 0;
}