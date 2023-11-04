# Implementation of Procedure to Convert Pushdown Automata (PDA) to Context-Free Grammar (CFG) in C++

Automata Theory & Formal Language course - Spring 2020 (April 22, 2020)

Computer Science and Engineering

University of South Florida

## Files Structure

* "PDA to CFG Report - Mateus.pdf": this is the project report providing definitions, explanations of the procedures, simplification, implementations, limitations, and results. Read this first.
* "PDA ro CFG Presentation - Mateus.pptx": this is the project presentation slides. This contains a summary of the report with visuals for the final presentation.
* "pda_to_cfg.cpp": this is the project C++ code implementation. See next section on how to use.
* "example_inputs.txt": this is a text file with a list of multiple example inputs and expected outputs.

## How to use?

* To compile for macOS/Linux:
  > g++ -std=c++17 -Wall pda_to_cfg.cpp -o pda_to_cfg
note: it also works with higher c++ std versions. C++17 is the version this project is using.
  
* To run for macOS/Linux:
  > ./pda_to_cfg

* Then, the terminal will prompt you to input all the PDA transitions followed by the word "DONE" as in the pdf project report (Section V). You can also use any other example input from the "example_inputs.txt" or create your own following the rules and limitations described in the report.
  
* Next, the terminal will prompt you to enter the total number of PDA states. for example, if the PDA states are q0, q1, q2, and q3, then enter "4".

* Lastly, the terminal will display the simplified CFG following the procedures described in the report.

## Further Comments:

* The first limitation described in the project report ("Each PDA transition rule can only push up to two symbols \[k≤2\] on top of the stack at the time") was set because it would lead to an exponentially growing number of productions in order m^k; where m is the total number of states and k is the number symbols pushed on top of the stack at the same time. This limitation can be set higher if necessary, but be wary of its exponential time complexity.

* This project can be improved with a GUI for the input PDA.
  
* The code can be updated and cleaned up for better performance.
