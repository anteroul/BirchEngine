#include "Engine.h"
#include <GL/glew.h>
#include <SDL2/SDL.h>

#define FPS 75

Engine::Engine(int w, int h, int sceneID) :
        running(true), window(w, h),
        camera(),
        day(camera, true),
        night(camera, false),
        evening(camera, false),
        cameraPosition(glm::vec3(0.0f)) {
    switch (sceneID)
    {
        case 0:
            activeScene = &day;
            break;
        case 1:
            activeScene = &night;
            break;
        case 2:
            activeScene = &evening;
            break;
        default:
            activeScene = &day;
            break;
    }
    printf("FPS capped at %d.\n", FPS);
}

void Engine::RunApplication()
{
    const int frameDelay = 1000 / FPS;
    Uint32 frameStart;
    unsigned int frameTime;

    glEnable(GL_DEPTH_TEST);

    // Game loop:
    while(running)
    {
        frameStart = SDL_GetTicks();

        render();
        trackInput();
        updateCamera();

        frameTime = SDL_GetTicks() - frameStart;

        if (frameDelay > frameTime)
            SDL_Delay(frameDelay - frameTime);

        //printf("FPS: %d\n", frameTime);
    }
}

bool Engine::ApplicationShouldClose() const
{
    if (running)
        return false;

    printf("Closing application...\n");
    return true;
}

void Engine::render()
{
    if (activeScene == &day)
        glClearColor(0.0f, 0.5f, 0.7f, 1.0f);
    if (activeScene == &night)
        glClearColor(0.0f, 0.0f, 0.2f, 1.0f);
    if (activeScene == &evening)
        glClearColor(0.3f, 0.0f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    activeScene->update();
    // main SDL rendering
    SDL_GL_SwapWindow(window.window);
}

void Engine::trackInput()
{
    const Uint8 *state = SDL_GetKeyboardState(nullptr);
    SDL_Event e;

    while (SDL_PollEvent(&e))
    {
        cameraFrontVector = camera.getNewFrontVector();

        switch (e.type) {
            case SDL_QUIT:
                running = false;
                break;
            case SDL_KEYDOWN:
                if (e.key.keysym.sym == SDLK_ESCAPE) {
                    running = false;
                    break;
                }
                if (e.key.keysym.sym == SDLK_F1)
                    activeScene = &day;
                if (e.key.keysym.sym == SDLK_F2)
                    activeScene = &night;
                if (e.key.keysym.sym == SDLK_F3)
                    activeScene = &evening;
                break;
            case SDL_MOUSEMOTION:
                camera.processMouseMovement(e.motion.xrel, e.motion.yrel);
                viewChanged = true;
                break;
            case SDL_KEYUP:
                viewChanged = false;
                cameraPosition = glm::vec3(0.0f);
                break;
            default:
                break;
        }

        if (state[SDL_SCANCODE_W]) {
            cameraPosition += camera.getSpeed() * cameraFrontVector;
            viewChanged = true;
        }
        if (state[SDL_SCANCODE_S]) {
            cameraPosition -= camera.getSpeed() * cameraFrontVector;
            viewChanged = true;
        }
        if (state[SDL_SCANCODE_A]) {
            cameraPosition -= glm::normalize(glm::cross(cameraFrontVector, camera.getUp())) * camera.getSpeed();
            viewChanged = true;
        }
        if (state[SDL_SCANCODE_D]) {
            cameraPosition += glm::normalize(glm::cross(cameraFrontVector, camera.getUp())) * camera.getSpeed();
            viewChanged = true;
        }
    }
}

void Engine::updateCamera()
{
    if (viewChanged)
        camera.translate(cameraPosition, cameraFrontVector);
}