$(shell mkdir -p ./build/bin ./build/src)
$(shell cp ./src/.muttrc build)

all: cat ls xargs mkdir touch muttshell ln rm cp mv bad_apple

cat: ./src/cat.c
	cc -o ./build/bin/cat ./src/cat.c -g
ls: ./src/ls.c
	cc -o ./build/bin/ls ./src/ls.c -g
xargs: ./src/xargs.c
	cc -o ./build/bin/xargs ./src/xargs.c -g
mkdir: ./src/mkdir.c
	cc -o ./build/bin/mkdir ./src/mkdir.c -g
touch: ./src/touch.c
	cc -o ./build/bin/touch ./src/touch.c -g
ln: ./src/ln.c
	cc -o ./build/bin/ln ./src/ln.c -g
rm: ./src/rm.c
	cc -o ./build/bin/rm ./src/rm.c -g
cp: ./src/cp.c
	cc -o ./build/bin/cp ./src/cp.c -g
mv: ./src/mv.c
	cc -o ./build/bin/mv ./src/mv.c -g
bad_apple: ./src/bad_apple.c
	cc -o ./build/bin/bad_apple ./src/bad_apple.c -lao -lmpg123
	$(shell cp ./src/badapple.mp3 ./src/badapple.txt ./build/src)
muttshell: ./src/muttshell.c
	cc -o ./build/muttshell ./src/muttshell.c -g -lreadline
	$(shell cp ./src/hello.txt ./build/src)

clean: 
	rm -rf build