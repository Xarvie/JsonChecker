if(MSVC)
    IF(USE_STATIC_MFC)
        SET(M_MFC_LINK_FLAG "STATIC")
        SET(CMAKE_MFC_FLAG 1)
    ELSE(USE_STATIC_MFC)
        SET(M_MFC_LINK_FLAG "SHARED")
        SET(CMAKE_MFC_FLAG 2)
        ADD_DEFINITIONS(-D_AFXDLL)
    ENDIF(USE_STATIC_MFC)
endif()