CC = ${EMSDK}/fastcomp/emscripten/em++ -std=c++17

SOURCE_FILES = $(shell find src -name '*.cpp')

EXECUTABLE = index.html

CFLAGS = -Idependencies/all_include
LDFLAGS = -Ldependencies/all_libs -static -lBulletDynamics -static -lBulletSoftBody -static -lLinearMath -static -lBulletCollision -lpthread
OPTS =  -s LINKABLE=1 -s WASM=1 -s USE_WEBGL2=1 -s FULL_ES3=1 -s USE_GLFW=3 -s TOTAL_MEMORY=268435456 -s ASSERTIONS=1 -s SAFE_HEAP=1
FUNCS = -s EXPORTED_FUNCTIONS='["_main"]' -s EXTRA_EXPORTED_RUNTIME_METHODS='["ccall", "cwrap"]'

all:
	$(CC) $(SOURCE_FILES) $(CFLAGS) $(LDFLAGS) -DBUILD_WEB -o $(EXECUTABLE) $(OPTS) $(FUNCS)

clean:
	rm -rf $(EXECUTABLE) ./*.data ./*.js ./*.wasm ./*.mem