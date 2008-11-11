FIND_PATH(QWT_INCLUDE_DIR  qwt_plot.h /usr/include/qwt /usr/local/include/qwt /usr/qwt/include NO_DEFAULT_PATH)

FIND_LIBRARY(QWT_LIBRARY NAMES qwt PATH /usr/lib /usr/local/lib /usr/lib64 /usr/local/lib64 /usr/qwt/lib) 

IF (QWT_INCLUDE_DIR AND QWT_LIBRARY)
   SET(QWT_FOUND TRUE)
ENDIF (QWT_INCLUDE_DIR AND QWT_LIBRARY)


IF (QWT_FOUND)
   IF (NOT Qwt_FIND_QUIETLY)
      MESSAGE(STATUS "Found Qwt libraries: ${QWT_LIBRARY}")
      MESSAGE(STATUS "Found Qwt includes: ${QWT_INCLUDE_DIR}")
   ENDIF (NOT Qwt_FIND_QUIETLY)
ELSE (QWT_FOUND)
   IF (Qwt_FIND_REQUIRED)
      MESSAGE(FATAL_ERROR "Could not find Qwt")
   ENDIF (Qwt_FIND_REQUIRED)
ENDIF (QWT_FOUND)

