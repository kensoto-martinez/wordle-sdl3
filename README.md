# Wordle

<p>
  <br>
  This project is a Wordle clone, is coded in C, and uses the SDL3 library. Fun fact: This is my first coding project in C. It is also my biggest coding project ever (at the time of its creation).
</p>

<br>

<h1>How it looks</h1>

<img src="screenshots/wordle.png" alt="Game Screenshot" width="600">
<img src="screenshots/wordle_stats.png" alt="Stats Menu Screenshot" width="600">
<img src="screenshots/wordle_settings.png" alt="Settings Menu Screenshot" width="600">
<img src="screenshots/wordle_help.png" alt="Help Menu Screenshot" width="600">
<img src="screenshots/wordle_guessed.png" alt="Finished Game Screenshot" width="600">

<br>

<h1>Installation requirements</h1>

<ul>
  <li>OS: Windows</li>
  <li>Specs: a computer</li>
  <li>Libraries: None (even when building; SDL3 included)</li>
</ul>

<br>

<h1>How to install</h1>

<p>If you want to play without having to build the project, click <a href="https://github.com/kensoto-martinez/wordle-sdl3/tags">here</a>, choose the latest version, and download wordle-windows.zip. Run the executable inside the .zip file.</p>

<br>

<h1>How to build</h1>

<p>Clone the entire repository in a directory of choice. Again, SDL3 is already included in the project; you can always delete it and include your SDL version. Use the batch script to compile the project. There is also a makefile if you prefer it.</p>

<br>

<h1>How my game works</h1>

<h2>Controls</h2>
<ul>
  <li>Keyboard letter: Type letter</li>
  <li>Enter: Submit guess</li>
  <li>Backspace: Delete letter</li>
  <li>Arrow Keys: Change focus square</li>
</ul>

<h2>How I compare the inputted word to the answer</h2>
<p>If you don't know how to play Wordle, I put a help section in my game or you can look it up online. As you now know, there is a grid of squares where letters appear to form the word you input. I stored each square, the color of each, and the letters on each square if there is one in their respective arrays. Putting them in their own arrays makes it easier to loop through them all, since an index from each array corresponds to a particular square's existence, color, and letter. Because of how easy it is to loop through every square, it wasn't too complicated to not only check if a letter is present in a word, but to also account for double letters. If someone guesses "PIZZA" and the answer is "ZONED", then the colors would be {GRAY, GRAY, YELLOW, GRAY, GRAY} rather than {GRAY, GRAY, YELLOW, YELLOW, GRAY}.</p>

<h2>Issues/Known Bugs</h2>

<h3>Word Bank</h3>
<p>You cannot guess plural words because I didn't differentiate between guessable words and guessable words that have the possibility of being the answer. Some guessable words are nonsensical or are names which makes the game harder than it should be.</p>

<h3>"Hard Mode"</h3>
<p>I added a "Hard Mode" toggle in the settings menu but forgot to actually implement the feature. A viable strategy in Wordle is to guess filler words that have letters you're thinking could be in the word. A filler word doesn't necessarily need to include green or yellow letters. Any guess can use letters that are grayed out, grayed out meaning you guessed the letter in a previous guess and it turns out that letter isn't in the answer. This strategy is useful if you know the answer has a common prefix or suffix. "Hard Mode" makes this strategy obsolete as you wouldn't be able to use grayed out letters and you would need to use revealed green and yellow letters in your next guess.</p>

<h3>Can open multiple menus</h3>
<p>It is possible to toggle multiple menus at once, making them render on top of each other.</p>

<h2>Features</h2>

<h3>Exclusive Features</h3>
<p><strong>v0.0.0-alpha</strong> creates a QoL feature exclusive to my game. In the official Wordle game, to change a letter in the middle of your guess, you will need to backspace the trailing letters until you reach said letter, change it, then type the removed letters again. This is a pain if said letter is the first letter. This pain is gone in my game. I added a focus square that highlights the focused square in white. With the arrow keys, you can move the focused square to any letter in the current guess. With this, you can easily change any letter in your guess rapidly. This makes backspacing letters slightly more complicated, just know that backspacing removes the <strong>preceding</strong> letter rather than the one highlighted. If you highlight the last square and there is a letter, it removes that letter.</p>

<h3>Features already in the official Wordle</h3>
<p>There is a help, settings, and stats button. The help button does a decent job at teaching you the game and it also explains the arrow key feature. The settings button technically has no purpose because "hard mode" is not implemented. Statistics are stored in stats.csv. stats.csv is updated only when exiting the game, although statistics are updated per game in the actual program.

<br>
