<!--Popescu Petrut - Alin-->

# Tema 3 PCLP
    
    In this README I will clearly and concisely describe the code flow, as well as certain tricks or formulas that I deduced during the development of this assignment.
    The format of this README is the following:

    - [Prparation]
    - [Tasks]
        -[UNDO AND REDO]
        -[L-system]
        -[Turtle]

## Preparation

    First of all, we need to identify the inputs and outputs. From the assignment statement, we observe that we are given multiple commands that we need to process, and at some point, upon encountering a specific command, we must stop. First, we allocate the buffers needed to implement these functionalities, then we start reading commands until we encounter the stop command. After reading a command, we process it because the function (fgets) always appends a newline character at the end of each read string. Then we call the command management function. After calling the main management function, it identifies the command and calls the function required to solve it. After all commands have been read, we free the memory of any unnecessary information and the program terminates.

## Tasks

    Within this assignment there are several functions, usually one main function, one loading function, and one display function. After finishing reading the command and after the management function has called the function needed to solve the command, we can start detailing the important functions for solving each command individually.

### UNDO AND REDO

    These two functions are very clearly described in the assignment statement. In order to implement them, we use a structure in which we store a snapshot of the rogram state. Depending on whether the UNDO or REDO command is encountered, we perform the corresponding operation. To simplify the implementation, we created a push function whose purpose is to add a new element to a stack and reallocate memory if necessary. When the UNDO function is called, it adds the current program state to the REDO stack, frees the current memory, and loads the last saved version of the program from the UNDO stack. When the REDO function is called, it simply adds the current program state to the UNDO stack and restores the memory using the last version stored in the REDO stack.

### L-system

    If the LSYSTEM function is called, then we must load a new L-system into memory. First, we determine the file path where the L-system is stored, then we call the load_from_file function, which reads the new L-system from a file and returns 0 if loading failed or 1 if the L-system was loaded successfully. After loading the new L-system, the old L-system must be saved in the manager in order to be able to perform the undo function successfully. After loading the old L-system into the undo manager, we must clear the redo manager. Once the redo manager is cleared, we can finally free the old L-system and replace it with the new one.

    If the DERIVE function is called, we must check several things. The DERIVE function requires an L-system to run, so this is the first thing we check. If an L-system is found loaded in memory, we can proceed. The DERIVE function is a simple one: it derives the last loaded L-system in memory n times and displays the nth derivation on the screen. The derivation operation is straightforward; the strategy is to iterate through the axiom of the L-system character by character. For each character, we look up its rule in the set of rules and replace the character in the previous derivation with its corresponding rule. After deriving n times, we return the resulting derivation string.

### Turtle

    The Turtle task is more complex and requires more knowledge to solve. First of all, the assignment clearly highlights the necessity of the DERIVE function, which was implemented as part of the L-system task. First, we need to implement two important functions to solve this task: LOAD and SAVE. The LOAD function, as described in the statement, loads a PPM image into memory, which we will use as a canvas for drawing. The SAVE function saves the drawing into another file.
   
    LOAD function (load_PPM_file): This function opens a binary file and starts reading characters from it, saving a PPM image into a structure called ppm, which has multiple fields where we store the width, height, maximum pixel value, and the pixel image itself. The pixel image is a structure with multiple channels, as mentioned in the statement: R, G, and B. These are read in binary format.
   
    SAVE function: This function simply writes the standard PPM header into a file, then iterates through each line of the image and writes it using the fwrite command.
   
    The Turtle function is the main function of this task. Its purpose is to draw an image based on a starting position, orientation, step size, derivation number, and color. The TURTLE function first processes the command and stores all the data necessary to start drawing. After processing the command and reading all the required data, we need to determine the nth derivation of the L-system loaded in memory. Then we iterate through each character of the derivation and perform the corresponding operation specified in the assignment. If the character is 'F', we start drawing. To do this, we adopt the following strategy: first, we determine the future position of the turtle after it takes a step, then we call the draw_line function, where Bresenham’s algorithm is implemented to draw a line between two points. To understand how we determine the next position of the turtle, we need some basic mathematical knowledge. The calculation of the turtle’s new coordinates is based on two simple mathematical formulas that are easy to derive. The new X coordinate is equal to the old X plus distance times cos(orientation), and the Y coordinate is similar but uses the sine function. These formulas are easy to visualize by imagining a right triangle where the turtle’s positions represent the endpoints of the hypotenuse. By projecting onto the axes, the previously mentioned formulas become apparent. After determining the new position, we call the draw function. If the character is '+', we rotate the turtle’s orientation counterclockwise by a certain number of degrees, and if it is '-', we rotate the orientation clockwise by a certain number of degrees. In this task, the turtle can store certain positions or return to previously saved positions. To achieve this, we need to implement a stack-like structure to store states and return to them. If we encounter the '[' character, we push the current state onto the stack, and if the ']' character appears, we return to the most recently saved state.
