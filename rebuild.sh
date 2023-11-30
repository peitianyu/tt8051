# 如果没有build文件夹, 新建一个
if [ ! -d "build" ]; then
    mkdir build
fi

cd build && cmake .. && make -j6

./tt_stc

# .././emu ../out/io.hex -clock=12000000

cd ..