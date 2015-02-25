
cmake_minimum_required(VERSION 2.8)

PROJECT(VIEW)

IF(MINGW)
ADD_DEFINITIONS(-D__GNUWIN32__ -D__WXMSW__ -DWXUSINGDLL -march=i686 -o3)
ELSE(UNIX)
ADD_DEFINITIONS(-D_FILE_OFFSET_BITS=64 -D_LARGEFILE64_SOURCE -D_USE_KNETFILE -D__WXGTK__ -o3)
ENDIF()

INCLUDE_DIRECTORIES(${PROJECT_BINARY_DIR}/samtools/ ${PROJECT_BINARY_DIR}/tabix/ ${PROJECT_BINARY_DIR})

IF(MINGW)
INCLUDE_DIRECTORIES(${PROJECT_BINARY_DIR}/samtools/win32)
SET(wxHome "/wxWidgets-2.9.4")
INCLUDE_DIRECTORIES(${wxHome}/include ${wxHome}/lib/gcc_lib/mswud)

ELSE(UNIX)
INCLUDE_DIRECTORIES("/usr/local/include/wx-2.9/" "/usr/local/lib/wx/include/gtk2-unicode-2.9")
ENDIF()


ADD_SUBDIRECTORY(${PROJECT_BINARY_DIR}/samtools/)

ADD_SUBDIRECTORY(${PROJECT_BINARY_DIR}/tabix/)

LINK_DIRECTORIES(${PROJECT_BINARY_DIR}/samtools/ ${PROJECT_BINARY_DIR}/tabix/)


IF(MINGW)
LINK_DIRECTORIES(${PROJECT_BINARY_DIR}/samtools/win32/)
SET(wxWidgets_USE_LIBS wxmsw29ud_core wxbase29ud wxmsw29ud_adv wxmsw29ud_xrc wxpngd wxbase29ud_xml wxexpatd)
LINK_DIRECTORIES(${wxHome}/lib/gcc_lib/)
ELSE(UNIX)
LINK_DIRECTORIES("/usr/local/lib")
SET(wxWidgets_USE_LIBS wx_baseu-2.9 wx_gtk2u_core-2.9 wx_baseu_xml-2.9 wx_gtk2u_adv-2.9 wx_gtk2u_aui-2.9)

ENDIF()


SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin) 


SET(viewsrc)

AUX_SOURCE_DIRECTORY(${PROJECT_BINARY_DIR}/src viewsrc)

IF(MINGW)
ADD_EXECUTABLE(view WIN32 ${viewsrc})
ELSE(UNIX)
ADD_EXECUTABLE(view ${viewsrc})
ENDIF()

IF(MINGW)
TARGET_LINK_LIBRARIES(view ${wxWidgets_USE_LIBS}  bam tabix curses z comctl32 ole32 oleaut32 pthread uuid winspool wsock32)
ELSE(UNIX)

TARGET_LINK_LIBRARIES(view ${wxWidgets_USE_LIBS} curses bam tabix z)
ENDIF()


