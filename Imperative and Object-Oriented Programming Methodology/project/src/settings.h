/**
 *   \file settings.h
 *   \brief Settings for GC and submodules
 */


#ifndef GC_SETTINGS
#define GC_SETTINGS


//////////////////////// GENERAL SETTINGS /////////////////////////////

#ifndef PAGESIZE
#define PAGESIZE 2048
#endif

#ifndef MAX_HEADER_SIZE
#define MAX_HEADER_SIZE 1024
#endif

/**
 * Whether to override the threshold passed to h_init.
 * If set to 'true' the value of GC_THRESHOLD_OVERRIDE_VALUE
 * will instead be used.
 */
#ifndef GC_THRESHOLD_OVERRIDE
#define GC_THRESHOLD_OVERRIDE true
#endif

/**
 * The value to be used if GC_THRESHOLD_OVERRIDE is 'true'.
 *
 */
#ifndef GC_THRESHOLD_OVERRIDE_VALUE
#define GC_THRESHOLD_OVERRIDE_VALUE 0.49
#endif


///////////////////////// OS  SETTINGS ///////////////////////////////

/**
 * \def UP
 * \def DOWN
 * Constants for ease of use.
 */
#define UP 0
#define DOWN 1

/**
 *  \def STACK_GROWTH
 *  \def HEAP_GROWTH
 *
 *  How the stack/heap grows in memory
 *
 *  | VALUE  |  DESCRIPTION                   |
 *  |--------|--------------------------------|
 *  | UP     |  **LOW**- to **HIGH** address  |
 *  | DOWN   |  **HIGH**- to **LOW** address  |
 */



/**
 * Settings on SPARC architecture
 */
#ifdef SPARC
#define WORDSIZE          4
#define HEAP_GROWTH       UP
#define STACK_GROWTH      DOWN

#define ALIGNMENT_CHAR    1
#define ALIGNMENT_INT     4
#define ALIGNMENT_LONG    4
#define ALIGNMENT_DOUBLE  8
#define ALIGNMENT_POINTER 4
#define ALIGNMENT_FLOAT   4
#endif

/**
 * Settings on SOLARIS x86 architecture
 */
#ifdef SOLARIS_32
#define WORDSIZE          4
#define HEAP_GROWTH       UP
#define STACK_GROWTH      DOWN

#define ALIGNMENT_CHAR    1
#define ALIGNMENT_INT     4
#define ALIGNMENT_LONG    4
#define ALIGNMENT_DOUBLE  4
#define ALIGNMENT_POINTER 4
#define ALIGNMENT_FLOAT   4
#endif

/**
 * Settings on Linux x86_64 architecture
 */
#ifdef LINUX_64
#define WORDSIZE          8
#define HEAP_GROWTH       UP
#define STACK_GROWTH      DOWN

#define ALIGNMENT_CHAR    1
#define ALIGNMENT_INT     4
#define ALIGNMENT_LONG    8
#define ALIGNMENT_DOUBLE  8
#define ALIGNMENT_POINTER 8
#define ALIGNMENT_FLOAT   4
#endif

/**
 * Defaults to generic linux x86 architecture
 */
#if !defined(SPARC) && !defined(SOLARIS_32) && !defined(LINUX_64)
#define WORDSIZE          4
#define HEAP_GROWTH       UP
#define STACK_GROWTH      DOWN

#define ALIGNMENT_CHAR    1
#define ALIGNMENT_INT     4
#define ALIGNMENT_LONG    4
#define ALIGNMENT_DOUBLE  4
#define ALIGNMENT_POINTER 4
#define ALIGNMENT_FLOAT   4
#endif

#endif
