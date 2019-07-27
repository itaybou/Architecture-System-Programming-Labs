cat ELFexec2 > ELFexec2cpy
chmod 777 ELFexec2cpy
make
./ELFexec2cpy
./printentry ELFexec2cpy
echo virus runs
./virus
echo program runs
./printentry ELFexec2cpy
cat ELFexec2cpy > ELFexec2cpy1
chmod 777 ELFexec2cpy1
cat ELFexec2 > ELFexec2cpy
chmod 777 ELFexec2cpy
./ELFexec2cpy1
