#if !defined(_FXFT_VERSION_) || _FXFT_VERSION_ == 2501
/***************************************************************************/
/*                                                                         */
/*  raster.c                                                               */
/*                                                                         */
/*    FreeType monochrome rasterer module component (body only).           */
/*                                                                         */
/*  Copyright 1996-2001 by                                                 */
/*  David Turner, Robert Wilhelm, and Werner Lemberg.                      */
/*                                                                         */
/*  This file is part of the FreeType project, and may only be used,       */
/*  modified, and distributed under the terms of the FreeType project      */
/*  license, LICENSE.TXT.  By continuing to use, modify, or distribute     */
/*  this file you indicate that you have read the license and              */
/*  understand and accept it fully.                                        */
/*                                                                         */
/***************************************************************************/


#define FT_MAKE_OPTION_SINGLE_OBJECT
#define FT2_BUILD_LIBRARY

#include "../../include/ft2build.h"
#include "rastpic.c"
#ifdef _FX_MANAGED_CODE_
#define TWorker_	TWorker_raster
#define TRaster_	TRaster_raster
#endif
#include "ftraster.c"
#include "ftrend1.c"


/* END */
#endif

