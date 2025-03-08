# Dominoes

This repository contains the source code of a Dominoes terminal game, for the laboratory final project of the Programming I course of the Computer Science career at the Rafael Urdaneta University, located in Maracaibo, Venezuela.

## Developers

- [ralvarezdev](https://github.com/ralvarezdev) - Ramón Álvarez

## Programming Languages

- C++

## Installation

<ol>
<li>Clone this repository to your local machine using the following command:

```git clone https://github.com/ralvarezdev/uru-programming-1-dominoes```
</li>

<li>Change your Current Working Directory to where the repository was cloned</li>

<li>There are two ways to compile the program:</li>

<ol>
<li>With <strong>G++</strong> (you must have installed <strong>MinGW Compiler</strong>). Use the following command:

<details>
<summary>Command for Windows</summary>

```g++ -o bin\Dominoes.exe src\main.cpp src\lib\cards.cpp src\lib\cards.h src\lib\input.cpp src\lib\input.h src\lib\terminal.cpp src\lib\terminal.h```

</details>

<details>
<summary>Command for Linux</summary>

```g++ -o bin/Dominoes.exe src/main.cpp src/lib/cards.cpp src/lib/cards.h src/lib/input.cpp src/lib/input.h src/lib/terminal.cpp src/lib/terminal.h```

</details></li>

<li>With <strong>CMake</strong> (you must have installed <strong>CMake and Make</strong>). First, get to the <code>bin</code> folder inside the repository. Use the following command:

<details>
<summary>Command for Windows</summary>

```cmake -S ..\ -G "MinGW Makefiles" -B .\ && make```

</details>

<details>
<summary>Command for Linux</summary>

```cmake -S ../ -G "MinGW Makefiles" -B ./ && make```

</details></li></ol>

<li>Run the program by clicking the <code>Dominoes.exe</code>.</li>
</ol>

<strong>For a better User Experience, resize the terminal window to fullscreen mode</strong>