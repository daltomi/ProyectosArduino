#!/bin/sh

if [[ $# -ne 1 ]]; then
    echo Error, falta nombre del proyecto.
    echo ./new [name project]
    exit
fi

PROJ="$1"
MAKEFILE=Makefile

if [[ -e $PROJ ]]; then
    echo Error, ya existe el directorio del proyecto: "$PROJ"
    exit
fi

if [[ ! -e $MAKEFILE ]]; then
    echo Error, no se encuentra el archivo $MAKEFILE
    exit
fi

arduino-cli -v sketch new "$PROJ"
cp -v $MAKEFILE "$PROJ"
exit









