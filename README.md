# Wordle

<p>This project is a Wordle clone, is coded in C, and uses the SDL3 library. Fun fact: This is my first coding project in C.</p>

<h2>How to install</h2>

<p>If you want to play without having to build the project, click <a href="https://github.com/kensoto-martinez/wordle-sdl3/tags">here</a>, choose the latest version, and download either wordle.zip or wordle-windows.zip. If you want to build/edit the project, download the source code.</p>

<h2>How to build</h2>

<p>If you are building, you have two options: a Makefile and a Batchscript. I suggest the Makefile. To build, just run either one inside your terminal. You will need a Unix terminal (e.g., MSYS) to use the make command on Windows. You do not need to download SDL3 to compile the project as the project contains the entire library. You should add the "include" directory into your PATH. If you prefer to use your own SDL3 installation, you can always include the PATH to your SDL3 installation rather than mine.</p>

<h2>How my game works</h2>

<p>If you don't know how to play Wordle, I put a help section in my game or you can look it up online. As you now know, in Wordle, there is a grid of squares that the letters appear on that make up a word that you inputted. I stored each square, the color of each, and the letters on each square if there is one in their respective arrays. Putting them in their own arrays makes it easier to loop through them all since an index from all arrays corresponds to a particular square's existence, color, and letter. Because of how easy it is to loop through every square, it wasn't too complicated to not only check if a letter is present in a word, but to also account for double letters. If someone guesses "PIZZA" and the answer is "ZONED", then the colors would be {GRAY, GRAY, YELLOW, GRAY, GRAY} rather than {GRAY, GRAY, YELLOW, YELLOW, GRAY}.</p>
