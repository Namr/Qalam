# Qalam: A Quantum Shader Language
Quantum computers are a powerful new computational tool that are vastly different from classical computers. Qalam is a programming language that recognizes these differences. Programs more closely resemble quantum circuit diagrams rather than classical computer programs. This allows the programmer to spend less time converting between the circuit diagram syntax and the language syntax. It also helps to avoid conflating classical computing approaches with quantum computing approaches.

## Usage

`qalamc` is the compiler for the language, it will take a qalam source file (which usually have the extension `.qlm`) and outputs a Qiskit powered Python file. In order to run these files you should have Qiskit installed on your system. By default, the output file is called `qalam_output.py`. This can be changed using the `-o` command line argument. Here an example usage of `qalamc`:

```
qalamc -o my_output_file.py ./examples/BV.qlm
```

### Usage with Quirk JSON export
`qalamc` also supports input from [Quirk's](https://algassert.com/quirk) JSON files. After creating a circuit in Quirk go to `Export` in the top left and then click `Copy to Clipboard` under the heading `Circuit JSON`. You will need to take the copied JSON and save that into a file. Then you can use the `-q` option to turn this JSON into Qiskit Python code. An example command would be:

```
qalamc -q ./examples/BV-Quirk.json -o quirkTest.py
```

## Building
Qalam can easily be built using CMake. It has no external dependencies. The following set of commands can be used to build:
```
mkdir build
cd build
cmake ..
make
```

This creates the executable `qalamc` in the build directory. You can add this to your `/bin` or just use it from the build directory (keep in mind when using the example commands above that filepaths are relative to where you are in your filesystem).

## Examples & Syntax

In the examples folder some famous quantum algorithms have been implemented in Qalam. This is a good way to get familiar with the syntax of the language. Here is a quick overview of the syntax:

There are two sections of the file, a `given:` section and a `circuit:` section.

In the given section specify your quantum variables
```
[VAR_NAME]: [NUMBER_OF_QBITS];
```
i.e
```
input: 12;
```
And your quantum gates
```
[GATE_NAME] <- [NUMBER_OF_INPUT_QBITS] =
{
    //BODY
};
```
i.e
```
XORString <- 13 =
{
    0, 12 -> CX;
    3, 12 -> CX;
    8, 12 -> CX;
};
```
In a quantum gate definition, the input variables are accessed by their index number as seen above. Variables inputted on the left hand side have lower index values.

The circuit section is a series of gate operations:
```
[VARIABLE_NAME] -> [GATE_NAME];
```
They can also be chained together in a single line:
```
[VARIABLE_NAME] -> [FIRST_GATE] -> [SECOND_GATE];
```
