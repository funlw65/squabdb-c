#!/bin/bash
cc  -o squabdb squabdb.c -lraylib -lGL -lm -lpthread -ldl -lrt -lX11 -ltyphoon
