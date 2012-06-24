project : LIBS\WAT386\CWINDOW.LIB .SYMBOLIC

LIBS\WAT386\CWINDOW.LIB : SRC\CWINDOW.CC SRC\DRIVER.CC  .AUTODEPEND
 wpp386 -i.\h src\cwindow.cc
 wpp386 -i.\h src\driver.cc
 wlib.exe -n -b -c LIBS\WAT386\CWINDOW.LIB +CWINDOW.OBJ +DRIVER.OBJ
 del *.obj



