find ./include -iname '*.h' | xargs clang-format -i -style=file
find ./src -iname '*.cpp' | xargs clang-format -i -style=file