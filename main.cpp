#include <iostream>
#include <SDL.h>

// Starts up SDL and creates the window
bool init();
// Loads the media
bool loadMedia();
// Frees the media and shits down SDL
void close();

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

SDL_Window* gWindow = nullptr;
SDL_Surface* gScreenSurface = nullptr;
SDL_Surface* gHelloWorld = nullptr;

bool init() {
    // Initialization flag
    bool success = true;

    // Initialise SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cout << "SDL could not initialise! SDL_Error: %s\n", SDL_GetError();
        success = false;
    } else {
        // Create window
        gWindow = SDL_CreateWindow(
                "Game", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                SCREEN_WIDTH, SCREEN_HEIGHT,
                SDL_WINDOW_SHOWN);
        if (gWindow == nullptr) {
            std::cout << "Window could not be created! SDL_Error: %s\n" << SDL_GetError();
            success = false;
        } else {
            // Get window surface
            gScreenSurface = SDL_GetWindowSurface(gWindow);
        }
    }
    return success;
}

bool loadMedia() {
    // Loading success flag
    bool success = true;

    // Loading splash image
    gHelloWorld = SDL_LoadBMP("Images/hello-devsoc.bmp");
    if (gHelloWorld == nullptr) {
        std::cout << "Unable to load image %s! SDL Error: %s\n" << "Images/hello-devsoc.bmp" << SDL_GetError();
        success = false;
    }
    return success;
}

void close() {
    // Deallocate surface
    SDL_FreeSurface(gHelloWorld);
    gHelloWorld = nullptr;

    // Destroy window
    SDL_DestroyWindow(gWindow);
    gWindow = nullptr;

    // Quit SDL subsystems
    SDL_Quit();
}

int main(int argc, char* args[]) {
    // Start up SDL and create window
    if (!init()) {
        std::cout << "Failed to initialise!\n";
    } else {
        // Load media
        if (!loadMedia()) {
            std::cout << "Failed to load media!\n";
        } else {
            // Apply the image
            SDL_BlitSurface(gHelloWorld, nullptr, gScreenSurface, nullptr);

            // Update the surface
            SDL_UpdateWindowSurface(gWindow);

            // Wait 2 seconds
            SDL_Delay(2000);
        }
    }

    // Close SDL
    close();

    return 0;
}
