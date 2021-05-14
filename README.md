# Graph Calculator
## MATAM final project – Course Number: 234124
[![Build Status](https://travis-ci.org/joemccann/dillinger.svg?branch=master)](https://travis-ci.org/joemccann/dillinger)

Implementation of a "Graphs Calculator".

The calculator is able to handle whitespaces between every element of the input expression or function.

For a detailed explanation about the project check out matam_final_ins.pdf and design.pdf.

## Operations
You can define/redefine a graph variable e.g. G1={a,b|<a,b>}.

You can assign a graph e.g. G1 = G2.

You can use functions and operation listed below.

## Operators
The calculator supports the below operators:
- union (G+G):

	𝑉(𝐺1+𝐺2)=𝑉(𝐺1)∪𝑉(𝐺2) ; 𝐸(𝐺1+𝐺2)=𝐸(𝐺1)∪𝐸(𝐺2) 
- intersection (G^G): 

	𝑉(𝐺1^𝐺2)=𝑉(𝐺1)∩𝑉(𝐺2) ; 𝐸(𝐺1^𝐺2)=𝐸(𝐺1)∩𝐸(𝐺2) 
- difference (G-G): 

	𝑉(𝐺1−𝐺2)={𝑣∈𝑉(𝐺1) | 𝑣∉𝑉(𝐺2)} ;
	𝐸(𝐺1−𝐺2)={<𝑣,𝑤> ∈𝐸(𝐺1) | 𝑣∈𝑉(𝐺1−𝐺2)∧𝑤∈𝑉(𝐺1−𝐺2)}
- product  (G*G):

	𝑉(𝐺1∗𝐺2)={[𝑣;𝑤] | 𝑣∈𝑉(𝐺1)∧ 𝑤∈𝑉(𝐺2)} ;
	𝐸(𝐺1∗𝐺2)={<[𝑣1;𝑤1],[𝑣2;𝑤2]> |<𝑣1,𝑣2> ∈𝐸(𝐺1)∧ <𝑤1,𝑤2> ∈𝐸(𝐺2)}
- complement  (G!):

	𝑉(!𝐺)=𝑉(𝐺) ;
	𝐸(!𝐺)={<𝑣,𝑤> | 𝑣∈𝑉(𝐺)∧𝑤∈𝑉(𝐺)∧ <𝑣,𝑤> ∉𝐸(𝐺)}

## Functions
The calculator supports the below functions:
- print(G) // adds graph to the calculator memory
- delete(G) // deletes graph from calculator memory
- who  // prints a lexical- sorted list of the defined variables
- reset // erase the calculator memory
- quit // finishes the program

## Advanced functionality
1) Complex expressions:

    a. The calculator supports advanced expressions as sequence of operators, with respect to priorities.
    G3 = G1 + !G2 * G2 // evaluated as (G1+(!G2))*G2

    b. Graph literals - allows using pre-defined variables and graph constants.
    G2 = G1 + {a,b|<a,b>}

    c. parentheses -  allows changing the evaluation order of the operators.
    G4 = G1 + (G2*G2)

    And of course a combination of all three:
    G4 = G3 + (G1*({a,b|<a,b>}+G2))

2) Saving and loading graphs:

    a. save(G, filename) - saves graph G into a file name filename.
    b. G=load(filename) - reads the saved grape from the file named filename to a variable named G.

3) Files are saved in a binary format:

   The files encoded in this format:
   <num_vertices><num_edges><vertex1><vertex2>...<vertexN><edge1><edge2>...<edgeM>

4) Functions with complex arguments:

   Will allow printing and saving functions to accept every legal expression and not only the variable name.

   print(G1+G2)

   print({a,b|<a,b>} * {c,d|<c,d>})

   save({a,b|<a,b>}, graph.gc).

5) Interfacing with python using SWIG:

   Allows using the calculator functionality with python as a library.

## Using the Calculator
The program has two running states: 
- auto mode (batch) - reading & writing to a file. After compilation run: gcalc input.txt output.txt
- interactive (shell-like).

## Execution
For your convenience there is makefile supported.

Run command "make".
 
Run the program using "./gcalc.exe".

## Extras
There is an explanation file named design.pdf, explaining the parsing process and the division to classes.

I followed OOP principles, also pay attention to the detailed erroring feedback to invalid input.
