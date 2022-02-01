#!/bin/bash
cc  -o squabdb squabdb.c -lraylib -lGL -lm -lpthread -ldl -lrt -lX11 -ltyphoon

# you have to compile typhoon as a dynamic library.
# default, it compiles omly as a static lib. you have to figure
# how to compile it and make a dynamic lib.

# or just compile with the static lib but copy the libtyphoon.a file 
# into the squabdb main folder and uncomment the following line:

#cc  -o squabdb squabdb.c -lraylib -lGL -lm -lpthread -ldl -lrt -lX11 libtyphoon.a
