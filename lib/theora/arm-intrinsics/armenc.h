/********************************************************************
 *                                                                  *
 * THIS FILE IS PART OF THE OggTheora SOFTWARE CODEC SOURCE CODE.   *
 * USE, DISTRIBUTION AND REPRODUCTION OF THIS LIBRARY SOURCE IS     *
 * GOVERNED BY A BSD-STYLE SOURCE LICENSE INCLUDED WITH THIS SOURCE *
 * IN 'COPYING'. PLEASE READ THESE TERMS BEFORE DISTRIBUTING.       *
 *                                                                  *
 * THE Theora SOURCE CODE IS COPYRIGHT (C) 2002-2010                *
 * by the Xiph.Org Foundation and contributors http://www.xiph.org/ *
 *                                                                  *
 ********************************************************************

  function:
    last mod: $Id: x86int.h 17344 2010-07-21 01:42:18Z tterribe $

 ********************************************************************/
#if !defined(_arm_armenc_H)
# define _arm_armenc_H (1)
# include "armint.h"

# if defined(OC_ARM_ASM)
#  define oc_enc_accel_init oc_enc_accel_init_arm
#  if defined(__aarch64__) || defined(_M_ARM64)
// Not required by spec, but practically everyone implements NEON so don't bother with vtable.
#   define oc_enc_frag_satd(_enc,...) oc_enc_frag_satd_neon(__VA_ARGS__)
#   define oc_enc_frag_satd2(_enc,...) oc_enc_frag_satd2_neon(__VA_ARGS__)
#   define oc_enc_frag_intra_satd(_enc,...) oc_enc_frag_intra_satd_neon(__VA_ARGS__)
#   define oc_enc_frag_sad(_enc,...) oc_enc_frag_sad_neon(__VA_ARGS__)
#   define oc_enc_frag_sad_thresh(_enc,_src,_ref,_ystride,_thresh) oc_enc_frag_sad_neon(_src,_ref,_ystride)
#   define oc_enc_frag_sad2_thresh(_enc,...) oc_enc_frag_sad2_thresh_neon(__VA_ARGS__)
#   define oc_enc_frag_ssd(_enc,...) oc_enc_frag_ssd_neon(__VA_ARGS__)
#   define oc_enc_frag_copy2(_enc,...) oc_enc_frag_copy2_neon(__VA_ARGS__)
#   define oc_enc_frag_sub(_enc,...) oc_enc_frag_sub_neon(__VA_ARGS__)
#   define oc_enc_frag_sub_128(_enc,...) oc_enc_frag_sub_128_neon(__VA_ARGS__)
#  else
#   define OC_ENC_USE_VTABLE (1)
#  endif
# endif

# include "../encint.h"

# if defined(OC_ARM_ASM)
#  include "armencfrag.h"
void oc_enc_accel_init_arm(oc_enc_ctx *_enc);
# endif

#endif
