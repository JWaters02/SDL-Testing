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
SDL_Surface* gKeyPressSurfaces[KEY_PRESS_SURFACE_TOTAL];
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
    return success;
}

bool loadMedia() {
    // Loading success flag
    bool success = true;

    // Load default surface
    gKeyPressSurfaces[KEY_PRESS_SURFACE_DEFAULT] = loadSurface("Images/hello-devsoc.bmp");
    if(gKeyPressSurfaces[KEY_PRESS_SURFACE_DEFAULT] == nullptr) {
        std::cout << "Failed to load default image!\n";
        success = false;
    }

    //Load up surface
    gKeyPressSurfaces[KEY_PRESS_SURFACE_UP] = loadSurface( "Images/up.bmp" );
    if(gKeyPressSurfaces[KEY_PRESS_SURFACE_UP] == nullptr) {
        std::cout << "Failed to load up image!\n";
        success = false;
    }

    //Load down surface
    gKeyPressSurfaces[KEY_PRESS_SURFACE_DOWN] = loadSurface( "Images/down.bmp" );
    if(gKeyPressSurfaces[KEY_PRESS_SURFACE_DOWN] == nullptr) {
        std::cout << "Failed to load down image!\n";
        success = false;
    }

    //Load left surface
    gKeyPressSurfaces[KEY_PRESS_SURFACE_LEFT] = loadSurface( "Images/left.bmp" );
    if(gKeyPressSurfaces[KEY_PRESS_SURFACE_LEFT] == nullptr) {
        std::cout << "Failed to load left image!\n";
        success = false;
    }

    //Load right surface
    gKeyPressSurfaces[KEY_PRESS_SURFACE_RIGHT] = loadSurface( "Images/right.bmp" );
    if(gKeyPressSurfaces[KEY_PRESS_SURFACE_RIGHT] == nullptr) {
        std::cout << "Failed to load right image!\n";
        success = false;
    }

    return success;
}

SDL_Texture* loadTexture(std::string path) {

}

SDL_Surface* loadSurface(std::string path) {
    // The final optimised image
    SDL_Surface* optimisedSurface = nullptr;

    // Load image at specified path
    SDL_Surface* loadedSurface = IMG_Load(path.c_str());
    if (loadedSurface == nullptr) {
        std::cout << "Unable to load image" << path.c_str() << "! SDL Error: " << IMG_GetError() << std::endl;
    } else {
        // Convert surface to screen format
        optimisedSurface = SDL_ConvertSurface(loadedSurface, gScreenSurface -> format, 0);
        if (optimisedSurface == nullptr) {
            std::cout << "Unable to optimise image" << path.c_str() << "! SDL Error: " << SDL_GetError() << std::endl;
        }
        // Get rid of old loaded surface
        SDL_FreeSurface(loadedSurface);
    }
    return optimisedSurface;
}

void close() {
    // Deallocate surface
    SDL_FreeSurface(gScreenSurface);
    gScreenSurface = nullptr;

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
            // Main loop flag
            bool quit = false;

            // Event handler
            SDL_Event e;

            // Set default current surface
            gCurrentSurface = gKeyPressSurfaces[KEY_PRESS_SURFACE_DEFAULT];

            // While application is running
            while (!quit) {
                // Handle events on queue
                while (SDL_PollEvent(&e) != 0) {
                    // User requests quit
                    if (e.type == SDL_QUIT) {
                        quit = true;
                    } else if (e.type == SDL_KEYDOWN) {
                        switch (e.key.keysym.sym) {
                            case SDLK_UP:
                                gCurrentSurface = gKeyPressSurfaces[KEY_PRESS_SURFACE_UP];
                                break;
                            case SDLK_DOWN:
                                gCurrentSurface = gKeyPressSurfaces[KEY_PRESS_SURFACE_DOWN];
                                break;
                            case SDLK_LEFT:
                                gCurrentSurface = gKeyPressSurfaces[KEY_PRESS_SURFACE_LEFT];
                                break;
                            case SDLK_RIGHT:
                                gCurrentSurface = gKeyPressSurfaces[KEY_PRESS_SURFACE_RIGHT];
                                break;
                            default:
                                gCurrentSurface = gKeyPressSurfaces[KEY_PRESS_SURFACE_DEFAULT];
                                break;
                        }
                    }
                }

                // Apply the image stretched
                SDL_Rect stretchRect;
                stretchRect.x = 0;
                stretchRect.y = 0;
                stretchRect.w = SCREEN_WIDTH;
                stretchRect.h = SCREEN_HEIGHT;
                SDL_BlitScaled(gCurrentSurface, nullptr, gScreenSurface, &stretchRect);

                // Update the surface
                SDL_UpdateWindowSurface(gWindow);
            }
        }
    }

    // Close SDL
    close();

    return 0;
}
