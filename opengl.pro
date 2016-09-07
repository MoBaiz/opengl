TEMPLATE = app
TARGET   = ObjReader

CONFIG += warn_off
QT += widgets axserver opengl
QT += opengl
HEADERS  = glbox.h \
           globjwin.h \
    glbox2.h \
    addobjfile.h \
    voxelization.h
SOURCES  = glbox.cpp \
           globjwin.cpp \
           main.cpp \
    addobjfile.cpp

RC_FILE = opengl.rc

# install
target.path = $$[QT_INSTALL_EXAMPLES]/activeqt/opengl
INSTALLS += target

 LIBS += -lopengl32 -lglu32
  LIBS += -LC:\glut
 LIBS += -lglut32

FORMS += \
    addobjfile.ui
