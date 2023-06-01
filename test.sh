make clean 
make all

valgrind ./40image -c flowers.ppm > flowers.out
valgrind ./40image -d flowers.out > flowersdecomp.ppm
valgrind ./ppmdiff flowers.ppm flowersdecomp.ppm
