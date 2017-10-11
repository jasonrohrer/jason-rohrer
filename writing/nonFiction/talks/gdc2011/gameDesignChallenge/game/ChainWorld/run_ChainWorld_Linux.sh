#!/bin/sh



if [ ! -d ~/.minecraft ];
then
   mkdir ~/.minecraft
fi


if [ ! -d ~/.minecraft/saves ];
then
   mkdir ~/.minecraft/saves
fi

cp -r data/minecraft/saves/Chain\ World   ~/.minecraft/saves



echo "Launching Minecraft...."

echo "Leave this window open until Minecraft exits."


# don't know whether this will block or not
# the -Xmx option seems to (strangly) prevent this call from forking javaw
# BUT, catch below and wait for user input, just in case!

java -Xmx1024M -jar bin/MinecraftLauncher.jar




echo ""

echo "Press ENTER *after* Minecraft exits:"

read dummyArg


echo "Copying Chain World back to USB..."

if [ -d ~/.minecraft/saves/Chain\ World ];
then
    echo "Moving local chain world back onto USB drive..."

    rm -r data/minecraft/saves/Chain\ World
    cp -r ~/.minecraft/saves/Chain\ World   data/minecraft/saves/
    rm -r ~/.minecraft/saves/Chain\ World

    echo "...done."
else
    echo "Chain World save directory not found on local drive!"
fi