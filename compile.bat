cls

@cl /nologo /Zi /GR- /EHs /MDd /c src/main.cc -I ../../Prog/ESAT_rev109/include
@cl /nologo /Zi /GR- /EHs /MDd /c src/MathLib.cc -I ../../Prog/ESAT_rev109/include
@cl /nologo /Zi /GR- /EHs /MDd /c src/Vehicle.cc -I ../../Prog/ESAT_rev109/include
@cl /nologo /Zi /GR- /EHs /MDd /c src/Ship.cc -I ../../Prog/ESAT_rev109/include
@cl /nologo /Zi /GR- /EHs /MDd /c src/LRV.cc -I ../../Prog/ESAT_rev109/include
@cl /nologo /Zi /GR- /EHs /MDd /c src/Terrain.cc -I ../../Prog/ESAT_rev109/include
@cl /nologo /Zi /GR- /EHs /MDd /c src/Misc.cc -I ../../Prog/ESAT_rev109/include

@cl /nologo /Zi /GR- /EHs /MDd /Fe:lander.exe main.obj MathLib.obj Vehicle.obj Ship.obj LRV.obj Terrain.obj Misc.obj ..\..\Prog\ESAT_rev109\bin\ESAT_d.lib opengl32.lib user32.lib gdi32.lib shell32.lib
