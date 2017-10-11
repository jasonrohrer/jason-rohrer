#!/bin/sh


if [ -d ~/Library/Application\ Support/minecraft/saves/Chain\ World ];
then
    echo "Moving local chain world back onto USB drive"

    rm -r data/minecraft/saves/Chain\ World
    cp -r ~/Library/Application\ Support/minecraft/saves/Chain\ World   data/minecraft/saves/
    rm -r ~/Library/Application\ Support/minecraft/saves/Chain\ World
fi