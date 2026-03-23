# Vine Generator Addon for Blender

Made by Mia Letendre
For COMP 438 - Computer Graphics
Tiberiu Popa - Instructor
Due: March 26, 2026 

# IMPORTANT
In vineGen_addon.py, change the folders refrencing the models and main to where they are on your local system
Install a version of python that is before your version of blender, in this case, 3.11.9
In CMakeLists.txt, change the folders to your local machine's python folders
THIS PROJECT WILL NOT WORK WITHOUT THESE CHANGES

# HOW THIS WAS MADE AND TESTED
Created a symbiotic link between github directory and extensions folder in blender:
New-Item -ItemType SymbolicLink -Path ``"C:\Users\miale\AppData\Roaming\Blender Foundation\Blender\5.0\extensions\user_default\vineGen" -Target "C:\Users\miale\Source\Repos\COMP_438_Project\vineGen"``
CTRL + SHIFT + P -> Cmake: Configure
`CTRL + SHIFT + B -> Cmake: Build`

# HOW TO RUN THIS
To run, go to blender, properties, then instal `vineGen_addon.py`
	NOTE the extension needs to be in blenders extension folder, and the vineGen_addon.py must point to the `out/build/x64-Debug folder`
then in the viewport, hit N and open tools tab, select VineGen, and a bar should pop up, add ur rules and other related inputs and then generate 

# RULES 
For the L-system, the rules are as follows:
F, G, H, I, J -> Move forward and draw a line, all go forward same amount
X, Y, Z -> A placeholder variable
+ -> Turn right by a specified angle
- -> Turn left by a specified angle
[ -> Push the current position and angle onto a stack
] -> Pop the position and angle from the stack
& -> Pitch down by a specified angle
^ -> Pitch up by a specified angle
> -> Roll right by a specified angle
< -> Roll left by a specified angle


# EXAMPLES
Kock Curve Axiom: `F` Rules: `F->F+F-F-F+F` Angle: `90`
Sierpinski Triangle: Axiom: `F-G-G` Rules: `F->F-G+F+G-F,G->GG` Angle: `120`
Dragon Curve: Axiom: `F` Rules: `F->F+G,G->F-G` Angle: `90`
Fractal Plant: Axiom: `-X` Rules: `X->F+[[X]-X]-F[-FX]+X,F->FF` Angle: `25`
Wizard Spell A: Axiom: `X` Rules: `X->F[&-FF][^+FF]+>X,F->FF` Angle: `25`
Wizard Spell B: Axiom: `X` Rules: `F->[F+F^F>F]F-F&F<F,` Angle: `25`
Wizard Spell C: Axion: `[+F][-F]` Rules `F->F>F[^G],G->G<F` Angle: `40`

# HOW TO BUILD A SET OF RULES
The vine starts off (0, 1, 0)
	For a spell, if you want to build something that spins around the Y axis, you need to start with a + or a - 
	Many times you don't need to use every possibel rule, for example, using ^ will turn you up, but > will change the direction of that up


# WORKS CITED 


