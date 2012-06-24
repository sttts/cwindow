project : DEMO.EXE .SYMBOLIC

DEMO.EXE : DEMO.CC
	wpp386 /i..\h demo.cc
	wlink FILE demo.obj LIBRARY ..\libs\wat386\cwindow.lib



