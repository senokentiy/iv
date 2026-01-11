
#include <SDL2/SDL.h>

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>


int
main (int argc, char **argv)
{
    if (argc != 2)
    {
        printf ("[x] usage: %s IMAGE\n", argv[0]);
        return 1;
    }

    char *fname = argv[1];
    FILE *fd = fopen (fname, "r");

    if (fd == NULL)
    {
        printf ("[x] unable to open file '%s'\n", fname);
        return EXIT_FAILURE;
    }

    // parse ppm header
    int height = -1;
    int width = -1;

    char buf[8];
    fgets (buf, sizeof (buf), fd);               // P-specifier -- skip
    fscanf (fd, "%d %d\n", &width, &height);     // dimensions
    fgets (buf, sizeof (buf), fd);               // maximum rgb value -- skip

    if (height == -1 || width == -1)
    {
        puts ("[x] unable to parse dimensions.");
        fclose (fd);
        return EXIT_FAILURE;
    }

    // window setup
    if (SDL_Init (SDL_INIT_VIDEO) != 0)
    {
        fprintf (stderr, "[x] init error: %s\n", SDL_GetError ());
        return 1;
    }

    SDL_Window *window = NULL;

    window = SDL_CreateWindow ("image viewer", SDL_WINDOWPOS_UNDEFINED,
                               SDL_WINDOWPOS_UNDEFINED, width, height, 0);

    if (window == NULL)
    {
        fprintf (stderr, "[x] window error: %s\n", SDL_GetError ());
        return 1;
    }

    SDL_Surface *surface = SDL_GetWindowSurface (window);

    // parsing rgb
    // filling window
    int x = 0;
    int y = 0;
    SDL_Rect pix = (SDL_Rect){ x, y, 1, 1 };
    Uint32 color = 0;
    Uint8 rgb[3];

    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            pix.x = x;
            pix.y = y;

            rgb[0] = getc (fd);
            rgb[1] = getc (fd);
            rgb[2] = getc (fd);
            color = SDL_MapRGB (surface->format, rgb[0], rgb[1], rgb[2]);

            SDL_FillRect (surface, &pix, color);
        }
    }

    SDL_UpdateWindowSurface (window);

    int running = 1;
    while (running)
    {
        SDL_Event event;
        while (SDL_PollEvent (&event))
        {
            if (event.type == SDL_QUIT)
            {
                running = 0;
            }
        }
        SDL_Delay (100);
    }

    fclose (fd);
    SDL_DestroyWindow (window);
    SDL_Quit ();
    return EXIT_SUCCESS;
}



