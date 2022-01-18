#!/bin/bash

gcc -W -g -o Hideme src/Hideme.c src/functions/functions.c src/functions/functions.h

gcc -W -g -o target src/target_binary/target.c

cp pgp_file/file.pgp file.pgp
