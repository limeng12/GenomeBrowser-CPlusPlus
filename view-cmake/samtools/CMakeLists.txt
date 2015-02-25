IF(MINGW)
ADD_DEFINITIONS(-D_USE_KNETFILE -D_CURSES_LIB=2)
ELSE(UNIX)
ADD_DEFINITIONS(-D_FILE_OFFSET_BITS=64 -D_LARGEFILE64_SOURCE -D_USE_KNETFILE -D_CURSES_LIB=1 -O2)
ENDIF()


INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR} )

IF(MINGW)
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/win32 )
ENDIF()

ADD_LIBRARY(bam bgzf.c kstring.c bam_aux.c bam.c bam_import.c sam.c bam_index.c bam_pileup.c bam_lpileup.c bam_md.c razf.c faidx.c knetfile.c bam_sort.c sam_header.c bam_reheader.c kprobaln.c bedidx.c)
