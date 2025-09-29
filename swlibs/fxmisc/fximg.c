/*
** Copyright (c) 1999, 2000, 2001, 2002, 2003, 2004, 2005, 2006, 2007, 2008, 2009, 2010
** The H3DK project. All rights reserved.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fximg.h" // <-- FIX: Includes the definitions for ImgInfo and ImgPalette

static FxBool
_imgWriteSbiHeader( FILE *stream, const ImgInfo *info ) {
    if ( fprintf( stream, "Y%c\n", info->yOrigin? '+' : '-' ) < 0 ) return FXFALSE;
    if ( fprintf( stream, "%u ", info->width ) < 0 ) return FXFALSE;
    if ( fprintf( stream, "%u\n", info->height ) < 0 ) return FXFALSE;
    if ( fprintf( stream, "R %u ", info->redBits ) < 0 ) return FXFALSE;
    if ( fprintf( stream, "G %u ", info->greenBits ) < 0 ) return FXFALSE;
    if ( fprintf( stream, "B %u\n", info->blueBits ) < 0 ) return FXFALSE;
    if ( fprintf( stream, "A %u\n", info->alphaBits ) < 0 ) return FXFALSE;
    return FXTRUE;
}


/*
** imgReadFile
*/
FxBool
imgReadFile( const char *filename, ImgInfo *info, ImgPalette *pal, void **data ) {
    FILE *stream;
    char buf[256];

    if ( ( stream = fopen( filename, "rb" ) ) == NULL ) {
        sprintf( buf, "Couldn't open \'%s\' for reading.", filename );
        fprintf( stderr, "%s\n", buf );
        return FXFALSE;
    }

    // Simplified stub for compilation purposes.
    fclose(stream);

    if (info) {
        info->width = 0;
        info->height = 0;
    }
    if (data) *data = NULL;
    
    return FXTRUE;
}