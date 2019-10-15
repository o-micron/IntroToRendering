# IntroToRendering
Rendering using different methods, Raycasting, Raytracing, Pathtracing<br>
All of the examples are intended to run anywhere, on the web for now<br>
So using emscripten sdk you can build the examples and run them inside your browser<br>
[See live version here](http://o-micron.github.io/IntroToRendering)

![Alt text](/screenshot.png "Screenshot")

# Philosophy
Build on any platform and run the examples on Chrome or Firefox

# Build IntroToRendering on MacOS
- Download and install emscripten sdk tools
```
cd dependencies
git clone https://github.com/emscripten-core/emsdk.git
cd emsdk
./emsdk install latest
```
- Build IntroToRendering using the emscripten SDK
```
./emsdk activate latest
source ./emsdk_env.sh
cd ../../
make -j4
```
- Run local server
```
python -m SimpleHTTPServer 8000
```
- Open Google Chrome on `http://127.0.0.1:8000`, et voiala !

# Build IntroToRendering on Linux
- Make sure you have installed `make` and `python`
if not then
```
sudo apt install python
sudo apt install make
```
- Download and install emscripten sdk tools
```
cd dependencies
git clone https://github.com/emscripten-core/emsdk.git
cd emsdk
./emsdk install latest
```
- Build IntroToRendering using the emscripten SDK
```
./emsdk activate latest
source ./emsdk_env.sh
cd ../../
make -j4
```
- Run local server
```
python -m SimpleHTTPServer 8000
```
- Open Google Chrome on `http://127.0.0.1:8000`, et voiala !

# Build IntroToRendering on Windows
Use Windows subsystem for linux and follow the linux build guides above


# LICENSE
[MIT LICENSE](LICENSE)
