cmake_minimum_required(VERSION 3.5)

project(TFGGcodeApp VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Encontrar los paquetes necesarios de Qt y OpenGL
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets OpenGLWidgets LinguistTools)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets OpenGLWidgets LinguistTools)
find_package(OpenGL REQUIRED)
include_directories(${OPENGL_INCLUDE_DIR})

# Archivos de traducción
set(TS_FILES translations/myapp_en.ts translations/myapp_es.ts)

# Archivos fuente y encabezados
set(PROJECT_SOURCES
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
    SaveSection.h
    SaveSection.cpp
    DimensionSection.h
    DimensionSection.cpp
    ShapeSection.h
    ShapeSection.cpp
    InfillSection.h
    InfillSection.cpp
    MaterialSection.h
    MaterialSection.cpp
    GenerateSection.h
    GenerateSection.cpp
    opengl.h
    opengl.cpp
    startpage.h
    startpage.cpp
)

# Añadir ejecutable según la versión de Qt
if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(TFGGcodeApp
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
else()
    if(ANDROID)
        add_library(TFGGcodeApp SHARED
            ${PROJECT_SOURCES}
        )
    else()
        add_executable(TFGGcodeApp
            ${PROJECT_SOURCES}
        )
    endif()
endif()

# Enlazar bibliotecas Qt y OpenGL
target_link_libraries(TFGGcodeApp PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)
target_link_libraries(TFGGcodeApp PRIVATE Qt${QT_VERSION_MAJOR}::OpenGLWidgets)
target_link_libraries(TFGGcodeApp PRIVATE ${OPENGL_gl_LIBRARY})

# Propiedades del ejecutable para macOS/iOS
if(${QT_VERSION} VERSION_LESS 6.1.0)
    set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.TFGGcodeApp)
endif()
set_target_properties(TFGGcodeApp PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

# Incluir y configurar la instalación
include(GNUInstallDirs)
install(TARGETS TFGGcodeApp
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

# Finalizar el ejecutable para Qt 6
if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(TFGGcodeApp)
endif()
