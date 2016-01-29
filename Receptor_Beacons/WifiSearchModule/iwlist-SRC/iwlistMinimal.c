/*
 *	Wireless Tools
 *
 *		Jean II - HPLB '99 - HPL 99->07
 *
 * This tool can access various piece of information on the card
 * not part of iwconfig...
 * You need to link this code against "iwlist.c" and "-lm".
 *
 * This file is released under the GPL license.
 *     Copyright (c) 1997-2007 Jean Tourrilhes <jt@hpl.hp.com>
 */

// Algumas Funções e Estruturas foram comentadas por mim para permitir compilação sob o C++ e compilador G++
// Estes lugares estarão comentados com:
// //TODO (everton): Corrigir para G++

#include "iwlib-private.h"		/* Private header */

/****************************** TYPES ******************************/

/*
 * Scan state and meta-information, used to decode events...
 */
typedef struct iwscan_state
{
    /* State */
    int			ap_num;		/* Access Point number 1->N */
    int			val_index;	/* Value in table 0->(N-1) */
} iwscan_state;

/*
 * Bit to name mapping
 */
typedef struct iwmask_name
{
    unsigned int	mask;	/* bit mask for the value */
    const char *	name;	/* human readable name for the value */
} iwmask_name;

/*
 * Types of authentication parameters
 */
typedef struct iw_auth_descr
{
    int				value;		/* Type of auth value */
    const char *			label;		/* User readable version */
    const struct iwmask_name *	names;		/* Names for this value */
    const int			num_names;	/* Number of names */
} iw_auth_descr;

/**************************** CONSTANTS ****************************/

#define IW_SCAN_HACK		0x8000

#define IW_EXTKEY_SIZE	(sizeof(struct iw_encode_ext) + IW_ENCODING_TOKEN_MAX)
