

mkdir capsule29_22

cd capsule29_22

pwd


for i in {22..29}; do
../steamImagePull.sh https://steamcharts.com/top/p.$i capsule_184x69.jpg
done


montage -tile x16 -background none -geometry +5+5 *.jpg montage.jpg