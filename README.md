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
Wizard Spell C: Axion: `[+F][-F]` Rules ` F->F>F[^G],G->G<F[^F]` Angle: `40`

# HOW TO BUILD A SET OF RULES
Only one letter can be replaced in each rule, for example:
	GOOD `F->F+F`
	BAD `FFG->FF+F`
The vine starts off (0, 1, 0)
	For a spell, if you want to build something that spins around the Y axis, you need to start with a + or a - 
	Many times you don't need to use every possible rule, for example, using ^ will turn you up, but > will change the direction of that up
Changing the axiom to fit your wanted vine is important
You must understand how to add smaller branches reasonably, as they grow exponentially


# WORKS CITED 
"Bark Willow." Poly Haven, polyhaven.com/a/bark_willow. Accessed 20 Mar. 2026.
Blender Foundation. Blender. Version 5.0.1, Blender Foundation, www.blender.org. Accessed 10 Mar. 2026.
Twigg, Christopher. "catmullRom.pdf." Carnegie Mellon University, www.cs.cmu.edu/~fp/courses/graphics/asst5/catmullRom.pdf. Accessed 13 Mar. 2026.
"Detailed Closeup Single Grape Leaf." Freepik, www.freepik.com/free-psd/detailed-closeup-single-grape-leaf_409868960.htm. Accessed 20 Mar. 2026.
"Curves and Splines" Computer Graphics (15-462), Spring 2010, Carnegie Mellon University, www.cs.cmu.edu/afs/cs/academic/class/15462-s10/www/lec-slides/lec06.pdf. Accessed 13 Mar. 2026.
"Pink Flower Petal Illustration Vector Set." Freepik, www.freepik.com/free-vector/pink-flower-petal-illustration-vector-set_16359014.htm. Accessed 20 Mar. 2026.
Futrick, S. Robert, and Jason Stowe. "LSystems Generator." Cornell University, people.ece.cornell.edu/land/OldStudentProjects/cs490-96to97/STOWE/P3D.html. Accessed 14 Mar. 2026.
