wget $1 -o log.txt -O - | grep "/app/" | sed -e "s/.*\/app\///" | sed -e "s/\">//" | while read appID
do
   wget -O $appID.jpg https://steamcdn-a.akamaihd.net/steam/apps/$appID/$2
done




#sed -e "s#\([0-9]*\)#https://steamcdn-a.akamaihd.net/steam/apps/\1/$2#"


# https://steamcdn-a.akamaihd.net/steam/apps/429050/capsule_184x69.jpg?t=1541703627
# https://steamcdn-a.akamaihd.net/steam/apps/429050/header.jpg?t=1541703627
# https://steamcdn-a.akamaihd.net/steam/apps/429050/capsule_616x353.jpg?t=1541703627
