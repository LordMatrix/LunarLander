cls

@cl /nologo /Zi /GR- /EHs /MDd /c main.cc -I ../../Prog/ESAT_rev109/include
@cl /nologo /Zi /GR- /EHs /MDd /c MathLib.cc -I ../../Prog/ESAT_rev109/include
@cl /nologo /Zi /GR- /EHs /MDd /c Ship.cc -I ../../Prog/ESAT_rev109/include
@cl /nologo /Zi /GR- /EHs /MDd /c Terrain.cc -I ../../Prog/ESAT_rev109/include
@cl /nologo /Zi /GR- /EHs /MDd /c Misc.cc -I ../../Prog/ESAT_rev109/include

@cl /nologo /Zi /GR- /EHs /MDd /Fe:lander.exe main.obj MathLib.obj Ship.obj Terrain.obj Misc.obj ..\..\Prog\ESAT_rev109\bin\ESAT_d.lib opengl32.lib user32.lib gdi32.lib shell32.lib
