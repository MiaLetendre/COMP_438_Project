# COMP_438_Project

#Made by Mia Letendre

CTRL + SHIFT + P -> Cmake: Configure
CTRL + SHIFT + P -> Cmake: Build

Created a symbiotic link between github directory and extensions folder in blender:
New-Item -ItemType SymbolicLink -Path "C:\Users\miale\AppData\Roaming\Blender Foundation\Blender\5.0\extensions\user_default\vineGen" -Target "C:\Users\miale\Source\Repos\COMP_438_Project\vineGen"

To run, go to blender, properties, then instal vineGen_addon.py 
//NOTE the extension needs to be in blenders extension folder, and the vineGen_addon.py must point to the out/build/x64-Debug folder
then in the viewport, hit N and open tools tab, select VineGen, and a bar should pop up, add ur rules and stuff and then generate 
//NOTE must have a version of python installed, thatis before the version of blender (for 5.0.1, 3.11.13), on my local machine it is 3.11.9