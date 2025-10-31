# s12_datapath_sim

project respository for the Simple 12 datapath simulation program.

## Building

Run the following command in the root directory to build the program:
```
make
```

## Running

The compiled executable is located in the build directory.
To run it on a file you can call
```
./build/interpreter memfiles/<FILE NAME>
```
where FILE NAME is the name of the file you want to run. All files 
in the ``memfiles/`` directory with the ``*.txt`` extension can
be ran as is. A trace file will be generated with the ``.trace``
extension attached to the name of the file passed, meaning it
will be generated in the directory of the passed file :/.

## Debug Mode

To run a program in an iterative mode that prints the pipeline
state and allows the user to step forward, the ``-d`` flag
can be passed as follows:
```
./build/interpreter memfiles/<FILE NAME> -d
``` 

## Statistics

These are generated and printed automatically at the end of the run.
