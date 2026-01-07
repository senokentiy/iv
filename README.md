# description
simple image viewer written with SDL2.\
only supports PPM (P6).
# dependencies
```
SDL2
SDL2-devel
```

# build
```
gcc main.c -Wall -Wextra -g -o iv `sdl2-config --cflags --libs`
```

# usage
```
./iv IMAGE
```

