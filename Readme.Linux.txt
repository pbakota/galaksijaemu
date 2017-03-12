Galaxy Linux - Galaksija Emulator for Linux
===========================================

v 0.1 - Very first test version. Simple SDL2 port. Basic support for GTP format.
      - The emulator's keys:
        * ESC           - Exit from emulator
        * F1            - A very simple help screen
        * F12           - NMI reset
        * F11 + SHIFT   - Normal reset
        * F8            - To toggle CPU speed (normal 3.072 MHz and fast 25 Mhz!). 
                          Just the CPU speed was increased the video refresh rate remains at 50 Hz. 
                          This extra speed comes very handy for example in the game "Ziul".
      - You can load GTP file from the command line with:
        ./galaxy Asteroidi.gtp -j0x2c74
        Optionally you can jump stright into the code with "-j" parameter. Eg: -j0x2c74 to auto start "Asteroidi".
      - To change the font color from the command line use "-c" option:
        * -c0 - The default gray font color.
        * -c1 - Green font color.
        * -c2 - Ember (?) font color.

      - Still missing features:
        =======================
        * LOAD/SAVE snapshot
        * Proper casette emulatio (Inspector Spiridon relays on it)
        * Cycle exact CPU emulation (This version still does not do that)
        * Video emulation (read actual data from latch). Currently the video output is 
          generated independently from Galaksija.

This is a Linux port of Galaxy - Galaksija emulator. Just a test version.

This project was made possible with the sources from these great authors

Miodrag Jevremovic  - Original GALAKSIJA emulator for DOS
Tomaž Šolc          - Windows port of the original emulator.
Marat Fayzullin     - Z80 Emulation core

Big thanks for Tomaž Šolc for his web blog for very detailed ROM1 source code,
and for all other tools/informations for/about Galaksija. 

His Galaksija dedicated web page is at https://www.tablix.org/~avian/blog/articles/galaksija/


Other valuable links:

http://retrospec.sgn.net/users/tomcat/yu/index.php Huge archive of the old Yu scene.
http://galaksija.org/ Which I have found incidentally and from where my journey was begun on the land of Galaksija.

