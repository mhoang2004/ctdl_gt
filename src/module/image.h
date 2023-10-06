SDL_Texture *backTexture = NULL;
SDL_Texture *hitBtnTexture = NULL;
SDL_Texture *skipBtnTexture = NULL;

// The window we'll be rendering to / surface
SDL_Window *gWindow = NULL;

// The window renderer
SDL_Renderer *gRenderer = NULL;

SDL_Texture *loadTexture(string path)
{
    // The final texture
    SDL_Texture *newTexture = NULL;

    // Load image at specified path
    SDL_Surface *loadedSurface = IMG_Load(path.c_str());
    if (loadedSurface == NULL)
    {
        printf("Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError());
    }
    else
    {
        // Create texture from surface pixels
        newTexture = SDL_CreateTextureFromSurface(gRenderer, loadedSurface);
        if (newTexture == NULL)
        {
            printf("Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError());
        }

        // Get rid of old loaded surface
        SDL_FreeSurface(loadedSurface);
    }

    return newTexture;
}

void renderBackCard()
{
    SDL_Rect destinationRect;

    // play btn
    destinationRect = {210, 450, 135, 59};
    SDL_RenderCopy(gRenderer, skipBtnTexture, NULL, &destinationRect);

    // skip btn
    destinationRect = {800, 450, 135, 59};
    SDL_RenderCopy(gRenderer, hitBtnTexture, NULL, &destinationRect);
}

void renderComputerCards()
{
    SDL_Rect destinationRect;

    // init 3 cards
    destinationRect = {50, SCREEN_HEIGHT / 2 - 145, 100, 145};
    SDL_RenderCopy(gRenderer, backTexture, NULL, &destinationRect);
    destinationRect = {SCREEN_WIDTH - 200, SCREEN_HEIGHT / 2 - 145, 100, 145};
    SDL_RenderCopy(gRenderer, backTexture, NULL, &destinationRect);
    destinationRect = {SCREEN_WIDTH / 2 - 100, 5, 100, 145};
    SDL_RenderCopy(gRenderer, backTexture, NULL, &destinationRect);
}