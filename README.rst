Chip8 Emulator
==============

|license| |cpp|

----

*chip8* is a C++ emulator for the Chip8 Virtual Computer.

|invaders| |tetris|

----

Compilation
-----------

You will need `cmake`_ and `SDL2`_ libraries in order to compile the emulator.

.. code:: bash

    $ git clone https://github.com/aimktech/chip8.git chip8
    $ cd chip8/build
    $ cmake ..
    $ make

The emulator will be generated in the ``build/bin`` directory.

Running
-------

To load a ROM in the emulator, just pass it on the command line:

.. code:: bash

    $ bin/chip8 ../../roms/TETRIS

The following actions can be performed during runtime:

- ``<ESC>`` : exit the emulator
- ``<F1>`` : reduce emulator speed
- ``<F2>`` : reset the emulator speed to its initial value (1)
- ``<F3>`` : increase the emulator speed
- ``<F10>`` : restart the emulator
- ``P`` : pause the emulator

ROMS
----

By default the emulator is provided with no ROMS.
You can find ROMs in the public domain `here`_.


License
-------

This program is released under the Apache License 2.0. See the bundled `LICENSE`_ file for details.

.. _cmake: https://cmake.org

.. _SDL2: https://www.libsdl.org

.. _here: https://www.zophar.net/pdroms/chip8.html

.. _LICENSE: https://github.com/aimktech/chip8/blob/master/LICENSE.TXT

.. |cpp| image:: https://img.shields.io/static/v1?label=c%2b%2b&message=17&color=blue&style=flat-square
    :target: https://en.wikipedia.org/wiki/C%2B%2B17
    :alt: C++ 17

.. |license| image:: https://img.shields.io/badge/license-Apache--2.0-blue.svg?style=flat-square
    :target: https://raw.githubusercontent.com/aimktech/amtTimer/master/LICENSE.txt
    :alt: License

.. |invaders| image:: https://github.com/aimktech/chip8/blob/master/screenshots/invaders.png
    :alt: Space Invaders

.. |tetris| image:: https://github.com/aimktech/chip8/blob/master/screenshots/tetris.png
    :alt: Tetris