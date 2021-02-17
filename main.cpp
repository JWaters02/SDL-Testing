#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <string>


// Starts up SDL and creates the window
bool init();
// Loads the media
bool loadMedia();
// Loads individual image as texture
SDL_Texture* loadTexture(std::string path);
// Loads individual image
SDL_Surface* loadSurface(std::string path);
// Frees the media and shits down SDL
void close();


enum KeyPressSurfaces {
    KEY_PRESS_SURFACE_DEFAULT,
    KEY_PRESS_SURFACE_UP,
    KEY_PRESS_SURFACE_DOWN,
    KEY_PRESS_SURFACE_LEFT,
    KEY_PRESS_SURFACE_RIGHT,
    KEY_PRESS_SURFACE_TOTAL
};

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

SDL_Renderer* gRenderer = nullptr;
SDL_Texture* gTexture = nullptr;
SDL_Window* gWindow = nullptr;
SDL_Surface* gScreenSurface = nullptr;
SDL_Surface* gCurrentSurface = nullptr;


bool init() {
    // Initialization flag
    bool success = true;

    // Initialise SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cout << "SDL could not initialise! SDL_Error:" << SDL_GetError() << std::endl;
        success = false;
    } else {
        // Create window
        gWindow = SDL_CreateWindow(
                "Game",
                SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                 SCREEN_WIDTH, SCREEN_HEIGHT,
                SDL_WINDOW_SHOWN
        );
        if (gWindow == nullptr) {
            std::cout << "Window could not be created! SDL_Error:" << SDL_GetError() << std::endl;
            success = false;
        } else {
            // Create renderer for window
            gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);
            if (gRenderer == nullptr) {
                std::cout << "Renderer could not be created! SDL_Error:" << SDL_GetError() << std::endl;
                success = false;
            } else {
                // Initialise renderer colour
                SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);

                // Initialise PNG loading
                int imgFlags = IMG_INIT_PNG;
                if (!(IMG_Init(imgFlags) & imgFlags)) {
                    std::cout << "SDL_image could not initialise! SDL_Error:" << SDL_GetError() << std::endl;
                    success = false;
                } else {
                    // Get window surface
                    gScreenSurface = SDL_GetWindowSurface(gWindow);
                }
            }
        }
    }
    return success;
}

SDL_Texture* loadTexture(std::string path) {
    // The final texture
    SDL_Texture* newTexture = nullptr;

    // Load image at specified path
    SDL_Surface* loadedSurface = IMG_Load(path.c_str());
    if (loadedSurface == nullptr) {
        std::cout << "Unable to load image" << path.c_str() << "! SDL Error: " << IMG_GetError() << std::endl;
    } else {
        // Convert texture from surface pixels
        newTexture = SDL_CreateTextureFromSurface(gRenderer, loadedSurface);
        if (newTexture == nullptr) {
            std::cout << "Unable to create texture from image" << path.c_str() << "! SDL Error: " << SDL_GetError() << std::endl;
        }
        // Get rid of old loaded surface
        SDL_FreeSurface(loadedSurface);
    }
    return newTexture;
}

bool loadMedia() {
    // Loading success flag
    bool success = true;

    // Load PNG texture
    gTexture = loadTexture("Images/hello-devsoc.png");
    if (gTexture == nullptr) {
        std::cout << "Failed to load texture image!\n";
        success = false;
    }

    return success;
}

void close() {
    //Free loaded image
    SDL_DestroyTexture(gTexture);
    gTexture = nullptr;

    //Destroy window
    SDL_DestroyRenderer(gRenderer);
    SDL_DestroyWindow(gWindow);
    gWindow = nullptr;
    gRenderer = nullptr;

    //Quit SDL subsystems
    IMG_Quit();
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
            // Main loop flag
            bool quit = false;

            // Event handler
            SDL_Event e;

            // While application is running
            while (!quit) {
                // Handle events on queue
                while (SDL_PollEvent(&e) != 0) {
                    // User requests quit
                    if (e.type == SDL_QUIT) {
                        quit = true;
                    }
                }

                // Clear screen
                SDL_RenderClear(gRenderer);

                // Render texture to screen
                SDL_RenderCopy(gRenderer, gTexture, nullptr, nullptr);

                // Update screen
                SDL_RenderPresent(gRenderer);
            }
        }
    }

    // Close SDL
    close();

    return 0;
}
