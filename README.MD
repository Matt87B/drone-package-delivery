501313741
Matthew Bowman
Assignment 1 - CPS511

## SETUP
Run and debug using "Run Raylib Program" through vscode, or compile and run the main.exe executable.
If it makes it easier, you can pull the entire repository by cloning from this link: https://github.com/Matt87B/drone-package-delivery
The .vscode config files make building and running this project much easier so I highly recommend cloning the repository rather than manually setting it up yourself. 

# KNOWN LIMITATIONS
I ran out of time and wasn't able to make nice looking landing gear (It is currently just a cube).
The tilt mechanics for the drone only tilt relative to the drone's Yaw when moving side-to-side (For some reason it doesn't work forward to back).

Also, if you are able to figure out why it is that the forward to backward movement doesn't tilt correctly while the side to side does, please let me know since I have been unable to fix this issue for quite a while. 
In future submissions, I'll do my best to organize the layouts of functions and classes to be more readable.

# WORKSPACE
I have seperated different portions of the code into seperate files. mesh_utils.cpp holds all the mesh related helper functions (Normal generation, indices, etc. ).
mat_utils.cpp holds all of the matrix operation helpers provided by the labs (I also added some of my own and tweaked the rotation operations to allow for three axis rotation with a Vector3).

The drone's hierarchy can be observed in the main.cpp file below the KeyListener section. The resources.cpp holds the mesh vertex coordinates as well as the two basic lighting shaders provided on d2l.

The game's Draw loop begins around line 504 and all drone parameters can be adjusted at the top of the main.cpp file.

# CONTROLS
The controls are the same as specified in the rubric. All motion is frame-rate dependant using GetFrameTime().

W A S D - Basic front back side to side movement relative to the drone's current facing direction
R F - Ascend and Descend
Q E - Rotate the drone (Yaw)
SPACEBAR - Start and Stop propellers
HOME - Reset everything back to start position (Drone, propellers, camera)
