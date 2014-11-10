doxytag -t qt.tag %QTDIR%/doc/html
doxygen doxyfile_qt.doxygen
cd web/doxygen
perl installdox -l  qt.tag@file://%QTDIR%/doc/html
cd ../../