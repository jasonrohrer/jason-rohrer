#!/bin/sh


if [ ! -d ~/Library/Application\ Support/minecraft ];
then
   mkdir ~/Library/Application\ Support/minecraft
fi


if [ ! -d ~/Library/Application\ Support/minecraft/saves ];
then
   mkdir ~/Library/Application\ Support/minecraft/saves
fi

cp -r data/minecraft/saves/Chain\ World   ~/Library/Application\ Support/minecraft/saves