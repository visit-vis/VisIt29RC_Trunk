# DO NOT EDIT THIS FILE! THIS FILE IS AUTOMATICALLY GENERATED BY xml2cmake
PROJECT(PeaksOverThreshold)

INCLUDE(${VISIT_SOURCE_DIR}/CMake/PluginMacros.cmake)

SET(COMMON_SOURCES
PeaksOverThresholdPluginInfo.C
PeaksOverThresholdCommonPluginInfo.C
PeaksOverThresholdAttributes.C
)

SET(LIBI_SOURCES 
PeaksOverThresholdPluginInfo.C
)

SET(LIBG_SOURCES
PeaksOverThresholdGUIPluginInfo.C
QvisPeaksOverThresholdWindow.C
${COMMON_SOURCES}
)
SET(LIBG_MOC_SOURCES
QvisPeaksOverThresholdWindow.h
)

SET(LIBV_SOURCES
PeaksOverThresholdViewerEnginePluginInfo.C
PeaksOverThresholdViewerPluginInfo.C
${COMMON_SOURCES}
)

SET(LIBE_SOURCES
PeaksOverThresholdViewerEnginePluginInfo.C
PeaksOverThresholdEnginePluginInfo.C
avtPeaksOverThresholdFilter.C
avtRPOTFilter.C
FileWriter.C
${COMMON_SOURCES}
)


IF(VISIT_PYTHON_SCRIPTING)
    SET(PYINCLUDES ${PYTHON_INCLUDE_PATH} ${VISIT_INCLUDE_DIR}/visitpy/visitpy)
ENDIF(VISIT_PYTHON_SCRIPTING)

INCLUDE_DIRECTORIES(
${CMAKE_CURRENT_SOURCE_DIR}
${VISIT_COMMON_INCLUDES}
${VISIT_INCLUDE_DIR}/avt/DBAtts/MetaData
${VISIT_INCLUDE_DIR}/avt/DBAtts/SIL
${VISIT_INCLUDE_DIR}/avt/Database/Database
${VISIT_INCLUDE_DIR}/avt/Expressions/Abstract
${VISIT_INCLUDE_DIR}/avt/Expressions/CMFE
${VISIT_INCLUDE_DIR}/avt/Expressions/Conditional
${VISIT_INCLUDE_DIR}/avt/Expressions/Derivations
${VISIT_INCLUDE_DIR}/avt/Expressions/General
${VISIT_INCLUDE_DIR}/avt/Expressions/ImageProcessing
${VISIT_INCLUDE_DIR}/avt/Expressions/Management
${VISIT_INCLUDE_DIR}/avt/Expressions/Math
${VISIT_INCLUDE_DIR}/avt/Expressions/MeshQuality
${VISIT_INCLUDE_DIR}/avt/Expressions/TimeIterators
${VISIT_INCLUDE_DIR}/avt/FileWriter
${VISIT_INCLUDE_DIR}/avt/Filters
${VISIT_INCLUDE_DIR}/avt/IVP
${VISIT_INCLUDE_DIR}/avt/Math
${VISIT_INCLUDE_DIR}/avt/Pipeline/AbstractFilters
${VISIT_INCLUDE_DIR}/avt/Pipeline/Data
${VISIT_INCLUDE_DIR}/avt/Pipeline/Pipeline
${VISIT_INCLUDE_DIR}/avt/Pipeline/Sinks
${VISIT_INCLUDE_DIR}/avt/Pipeline/Sources
${VISIT_INCLUDE_DIR}/avt/Plotter
${VISIT_INCLUDE_DIR}/avt/QtVisWindow
${VISIT_INCLUDE_DIR}/avt/View
${VISIT_INCLUDE_DIR}/avt/VisWindow/Colleagues
${VISIT_INCLUDE_DIR}/avt/VisWindow/Interactors
${VISIT_INCLUDE_DIR}/avt/VisWindow/Proxies
${VISIT_INCLUDE_DIR}/avt/VisWindow/Tools
${VISIT_INCLUDE_DIR}/avt/VisWindow/VisWindow
${VISIT_INCLUDE_DIR}/gui
${VISIT_INCLUDE_DIR}/mdserver/proxy
${VISIT_INCLUDE_DIR}/mdserver/rpc
${VISIT_INCLUDE_DIR}/viewer/core
${VISIT_INCLUDE_DIR}/viewer/main
${VISIT_INCLUDE_DIR}/viewer/main/ui
${VISIT_INCLUDE_DIR}/viewer/proxy
${VISIT_INCLUDE_DIR}/viewer/rpc
${VISIT_INCLUDE_DIR}/winutil
${VISIT_INCLUDE_DIR}/visit_vtk/full
${VISIT_INCLUDE_DIR}/visit_vtk/lightweight
${QT_INCLUDE_DIR}
${QT_QTCORE_INCLUDE_DIR}
${QT_QTGUI_INCLUDE_DIR}
${EAVL_INCLUDE_DIR} 
${VTK_INCLUDE_DIRS} 
${PYINCLUDES}
)

IF(HAVE_LIBNETCDF)
    INCLUDE_DIRECTORIES(${NETCDF_INCLUDE_DIR})
ENDIF(HAVE_LIBNETCDF)

IF(HAVE_LIBNETCDF)
    ADD_DEFINITIONS(-DHAVE_NETCDF)
ENDIF(HAVE_LIBNETCDF)

IF(HAVE_LIBNETCDF)
    LINK_DIRECTORIES(${VISIT_LIBRARY_DIR} ${QT_LIBRARY_DIR} ${GLEW_LIBRARY_DIR} ${EAVL_LIBRARY_DIR} ${VTK_LIBRARY_DIRS} ${NETCDF_LIBRARY_DIR})
ELSE(HAVE_LIBNETCDF)
    LINK_DIRECTORIES(${VISIT_LIBRARY_DIR} ${QT_LIBRARY_DIR} ${GLEW_LIBRARY_DIR} ${EAVL_LIBRARY_DIR} ${VTK_LIBRARY_DIRS} )
ENDIF(HAVE_LIBNETCDF)

ADD_LIBRARY(IPeaksOverThresholdOperator ${LIBI_SOURCES})
TARGET_LINK_LIBRARIES(IPeaksOverThresholdOperator visitcommon)
SET(INSTALLTARGETS IPeaksOverThresholdOperator)

IF(NOT VISIT_SERVER_COMPONENTS_ONLY AND NOT VISIT_ENGINE_ONLY AND NOT VISIT_DBIO_ONLY)
    QT_WRAP_CPP(GPeaksOverThresholdOperator LIBG_SOURCES ${LIBG_MOC_SOURCES})
    ADD_LIBRARY(GPeaksOverThresholdOperator ${LIBG_SOURCES})
    TARGET_LINK_LIBRARIES(GPeaksOverThresholdOperator visitcommon gui )

    ADD_LIBRARY(VPeaksOverThresholdOperator ${LIBV_SOURCES})
    TARGET_LINK_LIBRARIES(VPeaksOverThresholdOperator visitcommon viewer )
    SET(INSTALLTARGETS ${INSTALLTARGETS} GPeaksOverThresholdOperator VPeaksOverThresholdOperator)

    IF(VISIT_PYTHON_SCRIPTING)
        SET(LIBS_SOURCES
            PeaksOverThresholdScriptingPluginInfo.C
            PyPeaksOverThresholdAttributes.C
            ${COMMON_SOURCES}
        )
        ADD_LIBRARY(SPeaksOverThresholdOperator ${LIBS_SOURCES})
        TARGET_LINK_LIBRARIES(SPeaksOverThresholdOperator visitcommon visitpy ${PYTHON_LIBRARY})
        SET(INSTALLTARGETS ${INSTALLTARGETS} SPeaksOverThresholdOperator)
    ENDIF(VISIT_PYTHON_SCRIPTING)

    IF(VISIT_JAVA)
        FILE(COPY PeaksOverThresholdAttributes.java DESTINATION ${JavaClient_BINARY_DIR}/src/operators)
        ADD_CUSTOM_TARGET(JavaPeaksOverThreshold ALL ${Java_JAVAC_EXECUTABLE} ${VISIT_Java_FLAGS} -d ${JavaClient_BINARY_DIR} -classpath ${JavaClient_BINARY_DIR} -sourcepath ${JavaClient_BINARY_DIR} PeaksOverThresholdAttributes.java
            DEPENDS JavaClient
            WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
    ENDIF(VISIT_JAVA)
ENDIF(NOT VISIT_SERVER_COMPONENTS_ONLY AND NOT VISIT_ENGINE_ONLY AND NOT VISIT_DBIO_ONLY)

ADD_LIBRARY(EPeaksOverThresholdOperator_ser ${LIBE_SOURCES})
TARGET_LINK_LIBRARIES(EPeaksOverThresholdOperator_ser visitcommon avtexpressions_ser avtfilters_ser avtpipeline_ser )
IF(HAVE_LIBNETCDF)
    TARGET_LINK_LIBRARIES(EPeaksOverThresholdOperator_ser ${NETCDF_LIB})
ENDIF(HAVE_LIBNETCDF)
SET(INSTALLTARGETS ${INSTALLTARGETS} EPeaksOverThresholdOperator_ser)

IF(VISIT_PARALLEL)
    ADD_PARALLEL_LIBRARY(EPeaksOverThresholdOperator_par ${LIBE_SOURCES})
    TARGET_LINK_LIBRARIES(EPeaksOverThresholdOperator_par visitcommon avtexpressions_par avtfilters_par avtpipeline_par )
    IF(HAVE_LIBNETCDF)
        TARGET_LINK_LIBRARIES(EPeaksOverThresholdOperator_par ${NETCDF_LIB})
    ENDIF(HAVE_LIBNETCDF)
    SET(INSTALLTARGETS ${INSTALLTARGETS} EPeaksOverThresholdOperator_par)
ENDIF(VISIT_PARALLEL)

VISIT_INSTALL_OPERATOR_PLUGINS(${INSTALLTARGETS})
VISIT_PLUGIN_TARGET_RTOD(operators ${INSTALLTARGETS})
VISIT_PLUGIN_TARGET_FOLDER(operators PeaksOverThreshold ${INSTALLTARGETS})

ADD_SUBDIRECTORY(r_src)

