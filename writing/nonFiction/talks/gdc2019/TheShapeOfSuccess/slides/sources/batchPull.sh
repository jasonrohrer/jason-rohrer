

mkdir capsule33_30

cd capsule33_30

for i in {30..33};
do
../steamImagePull.sh https://steamcharts.com/top/p.$i capsule_184x69.jpg
done


montage -tile x16 -background none -geometry +5+5 *.jpg montage.jpg