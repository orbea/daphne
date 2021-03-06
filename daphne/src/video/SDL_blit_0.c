/*
  Simple DirectMedia Layer
  Copyright (C) 1997-2016 Sam Lantinga <slouken@libsdl.org>

  This software is provided 'as-is', without any express or implied
  warranty.  In no event will the authors be held liable for any damages
  arising from the use of this software.

  Permission is granted to anyone to use this software for any purpose,
  including commercial applications, and to alter it and redistribute it
  freely, subject to the following restrictions:

  1. The origin of this software must not be misrepresented; you must not
     claim that you wrote the original software. If you use this software
     in a product, an acknowledgment in the product documentation would be
     appreciated but is not required.
  2. Altered source versions must be plainly marked as such, and must not be
     misrepresented as being the original software.
  3. This notice may not be removed or altered from any source distribution.
*/
#include <stdint.h>
#include "../SDL_internal.h"

#include "SDL_video.h"
#include "SDL_blit.h"

/* Functions to blit from bitmaps to other surfaces */

static void
BlitBto1(SDL_BlitInfo * info)
{
    int c;
    int width, height;
    uint8_t *src, *map, *dst;
    int srcskip, dstskip;

    /* Set up some basic variables */
    width = info->dst_w;
    height = info->dst_h;
    src = info->src;
    srcskip = info->src_skip;
    dst = info->dst;
    dstskip = info->dst_skip;
    map = info->table;
    srcskip += width - (width + 7) / 8;

    if (map) {
        while (height--) {
            uint8_t byte = 0, bit;
            for (c = 0; c < width; ++c) {
                if ((c & 7) == 0) {
                    byte = *src++;
                }
                bit = (byte & 0x80) >> 7;
                if (1) {
                    *dst = map[bit];
                }
                dst++;
                byte <<= 1;
            }
            src += srcskip;
            dst += dstskip;
        }
    } else {
        while (height--) {
            uint8_t byte = 0, bit;
            for (c = 0; c < width; ++c) {
                if ((c & 7) == 0) {
                    byte = *src++;
                }
                bit = (byte & 0x80) >> 7;
                if (1) {
                    *dst = bit;
                }
                dst++;
                byte <<= 1;
            }
            src += srcskip;
            dst += dstskip;
        }
    }
}

static void
BlitBto2(SDL_BlitInfo * info)
{
    int c;
    int width, height;
    uint8_t *src;
    uint16_t *map, *dst;
    int srcskip, dstskip;

    /* Set up some basic variables */
    width = info->dst_w;
    height = info->dst_h;
    src = info->src;
    srcskip = info->src_skip;
    dst = (uint16_t *) info->dst;
    dstskip = info->dst_skip / 2;
    map = (uint16_t *) info->table;
    srcskip += width - (width + 7) / 8;

    while (height--) {
        uint8_t byte = 0, bit;
        for (c = 0; c < width; ++c) {
            if ((c & 7) == 0) {
                byte = *src++;
            }
            bit = (byte & 0x80) >> 7;
            if (1) {
                *dst = map[bit];
            }
            byte <<= 1;
            dst++;
        }
        src += srcskip;
        dst += dstskip;
    }
}

static void
BlitBto3(SDL_BlitInfo * info)
{
    int c, o;
    int width, height;
    uint8_t *src, *map, *dst;
    int srcskip, dstskip;

    /* Set up some basic variables */
    width = info->dst_w;
    height = info->dst_h;
    src = info->src;
    srcskip = info->src_skip;
    dst = info->dst;
    dstskip = info->dst_skip;
    map = info->table;
    srcskip += width - (width + 7) / 8;

    while (height--) {
        uint8_t byte = 0, bit;
        for (c = 0; c < width; ++c) {
            if ((c & 7) == 0) {
                byte = *src++;
            }
            bit = (byte & 0x80) >> 7;
            if (1) {
                o = bit * 4;
                dst[0] = map[o++];
                dst[1] = map[o++];
                dst[2] = map[o++];
            }
            byte <<= 1;
            dst += 3;
        }
        src += srcskip;
        dst += dstskip;
    }
}

static void
BlitBto4(SDL_BlitInfo * info)
{
    int width, height;
    uint8_t *src;
    uint32_t *map, *dst;
    int srcskip, dstskip;
    int c;

    /* Set up some basic variables */
    width = info->dst_w;
    height = info->dst_h;
    src = info->src;
    srcskip = info->src_skip;
    dst = (uint32_t *) info->dst;
    dstskip = info->dst_skip / 4;
    map = (uint32_t *) info->table;
    srcskip += width - (width + 7) / 8;

    while (height--) {
        uint8_t byte = 0, bit;
        for (c = 0; c < width; ++c) {
            if ((c & 7) == 0) {
                byte = *src++;
            }
            bit = (byte & 0x80) >> 7;
            if (1) {
                *dst = map[bit];
            }
            byte <<= 1;
            dst++;
        }
        src += srcskip;
        dst += dstskip;
    }
}

static void
BlitBto1Key(SDL_BlitInfo * info)
{
    int width = info->dst_w;
    int height = info->dst_h;
    uint8_t *src = info->src;
    uint8_t *dst = info->dst;
    int srcskip = info->src_skip;
    int dstskip = info->dst_skip;
    uint32_t ckey = info->colorkey;
    uint8_t *palmap = info->table;
    int c;

    /* Set up some basic variables */
    srcskip += width - (width + 7) / 8;

    if (palmap) {
        while (height--) {
            uint8_t byte = 0, bit;
            for (c = 0; c < width; ++c) {
                if ((c & 7) == 0) {
                    byte = *src++;
                }
                bit = (byte & 0x80) >> 7;
                if (bit != ckey) {
                    *dst = palmap[bit];
                }
                dst++;
                byte <<= 1;
            }
            src += srcskip;
            dst += dstskip;
        }
    } else {
        while (height--) {
            uint8_t byte = 0, bit;
            for (c = 0; c < width; ++c) {
                if ((c & 7) == 0) {
                    byte = *src++;
                }
                bit = (byte & 0x80) >> 7;
                if (bit != ckey) {
                    *dst = bit;
                }
                dst++;
                byte <<= 1;
            }
            src += srcskip;
            dst += dstskip;
        }
    }
}

static void
BlitBto2Key(SDL_BlitInfo * info)
{
    int width = info->dst_w;
    int height = info->dst_h;
    uint8_t *src = info->src;
    uint16_t *dstp = (uint16_t *) info->dst;
    int srcskip = info->src_skip;
    int dstskip = info->dst_skip;
    uint32_t ckey = info->colorkey;
    uint8_t *palmap = info->table;
    int c;

    /* Set up some basic variables */
    srcskip += width - (width + 7) / 8;
    dstskip /= 2;

    while (height--) {
        uint8_t byte = 0, bit;
        for (c = 0; c < width; ++c) {
            if ((c & 7) == 0) {
                byte = *src++;
            }
            bit = (byte & 0x80) >> 7;
            if (bit != ckey) {
                *dstp = ((uint16_t *) palmap)[bit];
            }
            byte <<= 1;
            dstp++;
        }
        src += srcskip;
        dstp += dstskip;
    }
}

static void
BlitBto3Key(SDL_BlitInfo * info)
{
    int width = info->dst_w;
    int height = info->dst_h;
    uint8_t *src = info->src;
    uint8_t *dst = info->dst;
    int srcskip = info->src_skip;
    int dstskip = info->dst_skip;
    uint32_t ckey = info->colorkey;
    uint8_t *palmap = info->table;
    int c;

    /* Set up some basic variables */
    srcskip += width - (width + 7) / 8;

    while (height--) {
        uint8_t byte = 0, bit;
        for (c = 0; c < width; ++c) {
            if ((c & 7) == 0) {
                byte = *src++;
            }
            bit = (byte & 0x80) >> 7;
            if (bit != ckey) {
                memcpy(dst, &palmap[bit * 4], 3);
            }
            byte <<= 1;
            dst += 3;
        }
        src += srcskip;
        dst += dstskip;
    }
}

static void
BlitBto4Key(SDL_BlitInfo * info)
{
    int width = info->dst_w;
    int height = info->dst_h;
    uint8_t *src = info->src;
    uint32_t *dstp = (uint32_t *) info->dst;
    int srcskip = info->src_skip;
    int dstskip = info->dst_skip;
    uint32_t ckey = info->colorkey;
    uint8_t *palmap = info->table;
    int c;

    /* Set up some basic variables */
    srcskip += width - (width + 7) / 8;
    dstskip /= 4;

    while (height--) {
        uint8_t byte = 0, bit;
        for (c = 0; c < width; ++c) {
            if ((c & 7) == 0) {
                byte = *src++;
            }
            bit = (byte & 0x80) >> 7;
            if (bit != ckey) {
                *dstp = ((uint32_t *) palmap)[bit];
            }
            byte <<= 1;
            dstp++;
        }
        src += srcskip;
        dstp += dstskip;
    }
}

static void
BlitBtoNAlpha(SDL_BlitInfo * info)
{
    int width = info->dst_w;
    int height = info->dst_h;
    uint8_t *src = info->src;
    uint8_t *dst = info->dst;
    int srcskip = info->src_skip;
    int dstskip = info->dst_skip;
    const SDL_Color *srcpal = info->src_fmt->palette->colors;
    SDL_PixelFormat *dstfmt = info->dst_fmt;
    int dstbpp;
    int c;
    uint32_t pixel;
    unsigned sR, sG, sB;
    unsigned dR, dG, dB, dA;
    const unsigned A = info->a;

    /* Set up some basic variables */
    dstbpp = dstfmt->BytesPerPixel;
    srcskip += width - (width + 7) / 8;

    while (height--) {
        uint8_t byte = 0, bit;
        for (c = 0; c < width; ++c) {
            if ((c & 7) == 0) {
                byte = *src++;
            }
            bit = (byte & 0x80) >> 7;
            if (1) {
                sR = srcpal[bit].r;
                sG = srcpal[bit].g;
                sB = srcpal[bit].b;
                DISEMBLE_RGBA(dst, dstbpp, dstfmt, pixel, dR, dG, dB, dA);
                ALPHA_BLEND_RGBA(sR, sG, sB, A, dR, dG, dB, dA);
                ASSEMBLE_RGBA(dst, dstbpp, dstfmt, dR, dG, dB, dA);
            }
            byte <<= 1;
            dst += dstbpp;
        }
        src += srcskip;
        dst += dstskip;
    }
}

static void
BlitBtoNAlphaKey(SDL_BlitInfo * info)
{
    int width = info->dst_w;
    int height = info->dst_h;
    uint8_t *src = info->src;
    uint8_t *dst = info->dst;
    int srcskip = info->src_skip;
    int dstskip = info->dst_skip;
    SDL_PixelFormat *srcfmt = info->src_fmt;
    SDL_PixelFormat *dstfmt = info->dst_fmt;
    const SDL_Color *srcpal = srcfmt->palette->colors;
    int dstbpp;
    int c;
    uint32_t pixel;
    unsigned sR, sG, sB;
    unsigned dR, dG, dB, dA;
    const unsigned A = info->a;
    uint32_t ckey = info->colorkey;

    /* Set up some basic variables */
    dstbpp = dstfmt->BytesPerPixel;
    srcskip += width - (width + 7) / 8;

    while (height--) {
        uint8_t byte = 0, bit;
        for (c = 0; c < width; ++c) {
            if ((c & 7) == 0) {
                byte = *src++;
            }
            bit = (byte & 0x80) >> 7;
            if (bit != ckey) {
                sR = srcpal[bit].r;
                sG = srcpal[bit].g;
                sB = srcpal[bit].b;
                DISEMBLE_RGBA(dst, dstbpp, dstfmt, pixel, dR, dG, dB, dA);
                ALPHA_BLEND_RGBA(sR, sG, sB, A, dR, dG, dB, dA);
                ASSEMBLE_RGBA(dst, dstbpp, dstfmt, dR, dG, dB, dA);
            }
            byte <<= 1;
            dst += dstbpp;
        }
        src += srcskip;
        dst += dstskip;
    }
}

static const SDL_BlitFunc bitmap_blit[] = {
    (SDL_BlitFunc) NULL, BlitBto1, BlitBto2, BlitBto3, BlitBto4
};

static const SDL_BlitFunc colorkey_blit[] = {
    (SDL_BlitFunc) NULL, BlitBto1Key, BlitBto2Key, BlitBto3Key, BlitBto4Key
};

SDL_BlitFunc
SDL_CalculateBlit0(SDL_Surface * surface)
{
    int which;

    if (surface->format->BitsPerPixel != 1) {
        /* We don't support sub 8-bit packed pixel modes */
        return (SDL_BlitFunc) NULL;
    }
    if (surface->map->dst->format->BitsPerPixel < 8) {
        which = 0;
    } else {
        which = surface->map->dst->format->BytesPerPixel;
    }
    switch (surface->map->info.flags & ~SDL_COPY_RLE_MASK) {
    case 0:
        return bitmap_blit[which];

    case SDL_COPY_COLORKEY:
        return colorkey_blit[which];

    case SDL_COPY_MODULATE_ALPHA | SDL_COPY_BLEND:
        return which >= 2 ? BlitBtoNAlpha : (SDL_BlitFunc) NULL;

    case SDL_COPY_COLORKEY | SDL_COPY_MODULATE_ALPHA | SDL_COPY_BLEND:
        return which >= 2 ? BlitBtoNAlphaKey : (SDL_BlitFunc) NULL;
    }
    return (SDL_BlitFunc) NULL;
}

/* vi: set ts=4 sw=4 expandtab: */
