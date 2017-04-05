# tabletennis_GUI
GUI for the tabletennis program, written in Qt.

## Prerequisite

- `libbgsubtract.so` from [TableTennis](https://github.com/BrotherJing/TableTennis)
- `libtracker.so` from [CNN_tabletennis](https://github.com/BrotherJing/CNN_tabletennis)
- `libtrajpred.so` from [RNN_tabletennis](https://github.com/BrotherJing/RNN_tabletennis)

# Run

In `CMakeLists.txt`, change the include path and library path to where you install the libraries above. Then

```bash
mkdir build
cd build
cmake ..
make
```

Run `./tracking -h` for help.
