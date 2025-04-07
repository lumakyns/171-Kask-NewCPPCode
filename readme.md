# 171 Checkers Project - CPP
This repository is a refactoring of C++ code for UCI's CS 171 class.

## Improved performance
To see the refactoring improvement, go to ./metrics/Graphing.

## Building
1. Optional: Move the StudentAI[.cpp/.h] files from ./metrics/Simple_MCTS to the desired directory in ./src./[Original/Refactored].
2. Then, either with Original or Refactored...
```bash
cd src/Refactored/build
cmake -S .. -B . -G "Unix Makefiles"
cd ../bin
.\\Checkers.exe 8 8 3 s
```

## Acknowledgements
Thanks to Dalton Adcock for helping me write the MCTS code.