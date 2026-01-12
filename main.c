
#include <SDL2/SDL.h>

#include <SDL2/SDL_error.h>
#include <SDL2/SDL_shape.h>
#include <SDL2/SDL_stdinc.h>
#include <SDL2/SDL_surface.h>
#include <SDL2/SDL_video.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>


typedef struct
{
    char *ptype;
    int width;
    int height;
    int maxrgb;
} ppm_header;


int
parse_header (FILE *fd, ppm_header *hdr)
{
    char buf[8];
    fgets (buf, sizeof (buf), fd);                     // P-specifier -- skip
    fscanf (fd, "%d %d\n", &hdr->width, &hdr->height); // dimensions
    fgets (buf, sizeof (buf), fd); // maximum rgb value -- skip

    if (!hdr->height || !hdr->width)
    {
        fprintf (stderr, "[x] unable to parse dimensions.");
        fclose (fd);
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}


int
fill_rgb (SDL_Window *window, FILE *fd, ppm_header *hdr)
{
    SDL_Surface *surface = SDL_GetWindowSurface (window);

    if (surface == NULL)
    {
        fprintf (stderr, "[x] unable to get window surface: '%s'\n",
                 SDL_GetError ());
        return EXIT_FAILURE;
    }

    SDL_Rect pix = (SDL_Rect){ 0, 0, 1, 1 };
    Uint32 color = 0;
    Uint8 rgb[3];

    for (int y = 0; y < hdr->height; y++)
    {
        for (int x = 0; x < hdr->width; x++)
        {
            pix.x = x;
            pix.y = y;

            rgb[0] = getc (fd);
            rgb[1] = getc (fd);
            rgb[2] = getc (fd);
            // printf ("%d %d %d\n", rgb[0], rgb[1], rgb[2]);

            color = SDL_MapRGB (surface->format, rgb[0], rgb[1], rgb[2]);

            int res = SDL_FillRect (surface, &pix, color);

            if (res != EXIT_SUCCESS)
            {
                fprintf (stderr, "[x] unable to set pixel.");
                fclose (fd);
                return EXIT_FAILURE;
            }
        }
    }
    return EXIT_SUCCESS;
}


int
setup_window (SDL_Window *window, ppm_header *hdr)
{
    if (SDL_Init (SDL_INIT_VIDEO) != EXIT_SUCCESS)
    {
        fprintf (stderr, "[x] init error: %s\n", SDL_GetError ());
        return EXIT_FAILURE;
    }

    window = SDL_CreateWindow ("image viewer", SDL_WINDOWPOS_UNDEFINED,
                               SDL_WINDOWPOS_UNDEFINED, 100, 100, 0);
    if (window == NULL)
    {
        fprintf (stderr, "[x] window error: %s\n", SDL_GetError ());
        return 1;
    }

    return EXIT_SUCCESS;
}


int
main (int argc, char **argv)
{
    if (argc != 2)
    {
        fprintf (stderr, "[x] usage: %s IMAGE\n", argv[0]);
        return 1;
    }

    char *fname = argv[1];
    FILE *fd = fopen (fname, "r");

    if (fd == NULL)
    {
        fprintf (stderr, "[x] unable to open file '%s'\n", fname);
        return EXIT_FAILURE;
    }

    // parse ppm header
    ppm_header hdr = (ppm_header){ NULL, 0, 0, 0 };
    parse_header (fd, &hdr);

    // window setup
    SDL_Window *window = NULL;
    // setup_window (window, &hdr);
    if (SDL_Init (SDL_INIT_VIDEO) != EXIT_SUCCESS)
    {
        fprintf (stderr, "[x] init error: %s\n", SDL_GetError ());
        return EXIT_FAILURE;
    }

    window
        = SDL_CreateWindow ("image viewer", SDL_WINDOWPOS_UNDEFINED,
                            SDL_WINDOWPOS_UNDEFINED, hdr.width, hdr.height, 0);
    if (window == NULL)
    {
        fprintf (stderr, "[x] window error: %s\n", SDL_GetError ());
        return 1;
    }

    // parsing rgb
    // filling window
    fill_rgb (window, fd, &hdr);

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



