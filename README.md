# Yet Another CHIP-8 Emulator (YACE)

My take on the ubiquitous CHIP-8 emulator in C as both an exercise in C and learning about emulators.

# Build instructions
```
cd ./buid/
cmake
make 
make run
```
The code should run the Tetris ROM in ./roms/

# Tests

```
cd ./build/
make tests && ./tests/tests
```

# To-dos

* [ ] Finish writing tests
* [ ] Port to ESP-32 
* [ ] Add sound
* [ ] Weird graphical glitch


## References

http://www.emulator101.com/chip-8-instruction-set.html

http://devernay.free.fr/hacks/chip8/C8TECH10.HTM#3.1
