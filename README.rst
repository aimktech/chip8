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

    $ bin/c8run ../../roms/BLITZ

The following actions can be performed during runtime:

- ``<ESC>`` : exit the emulator
- ``<F1>`` : reduce emulator speed
- ``<F2>`` : reset the emulator speed to its initial value (1)
- ``<F3>`` : increase the emulator speed
- ``<F10>`` : restart the emulator
- ``P`` : pause the emulator

The **disassembler** will try to extract the assembly source from the bytes code.

.. code:: bash

    $ bin/c8dasm ../../roms/BLITZ > blitz.asm

The output of the disassembler shows labels, data section and the initial opcode:

.. code::

    ;----------------------------------------------------
    ; ROM Name: BLITZ
    ; ROM Size: 393 Bytes
    ;----------------------------------------------------
        JP   L217                     ; 1217
        DB   #42, #4C, #49, #54
        DB   #5A, #20, #42, #79
        DB   #20, #44, #61, #76
        DB   #69, #64, #20, #57
        DB   #49, #4E, #54, #45
        DB   #52
    L217:
        LD   I, L341                  ; A341
        LD   V0, #04                  ; 6004
        LD   V1, #09                  ; 6109
        LD   V2, #0E                  ; 620E
        LD   V7, #04                  ; 6704
    L221:
        DRW  V0, V1, #0E              ; D01E
        ADD  I, V2                    ; F21E
        ADD  V0, #0C                  ; 700C
        SE   V0, #40                  ; 3040
        JP   L221                     ; 1221
        LD   V0, K                    ; F00A
    L22D:
        CLS                           ; 00E0
        CALL L2D9                     ; 22D9
        LD   V0, K                    ; F00A
        CLS                           ; 00E0
        LD   VE, V7                   ; 8E70
        LD   I, L31E                  ; A31E

The **assembler** takes the output of the disassembler, or any Chip8 assembly source
and build the corresponding ROM.

.. code:: bash

    $ bin/c8asm blitz.asm blitz.rom
    190 lines parsed.
    393 bytes generated.


ROMS
----

By default the emulator is provided with no ROMS.
You can find ROMs in the public domain `here`_.

Links
-----

Here is a curated list of links that might be useful when building a Chip8 emulator

- Chip8 `documentation`_ by David WINTER [PDF]
- Chip8 `Technical reference`_
- `Building`_ the Dream 6800 (Electronics Australia 1979)
- `Programming`_ in Chip8 (Electronics Today International 1981)

License
-------

This program is released under the Apache License 2.0. See the bundled `LICENSE`_ file for details.

.. _cmake: https://cmake.org

.. _SDL2: https://www.libsdl.org

.. _here: https://www.zophar.net/pdroms/chip8.html

.. _documentation: http://vanbeveren.byethost13.com/stuff/CHIP8.pdf

.. _Technical reference: http://devernay.free.fr/hacks/chip8/C8TECH10.HTM

.. _Building: https://archive.org/stream/EA1979/EA%201979-05%20May#page/n85/mode/2up

.. _Programming: https://archive.org/stream/ETIA1981/ETI%201981-11%20November#page/n113/mode/2up

.. _LICENSE: https://github.com/aimktech/chip8/blob/master/LICENSE.txt

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
