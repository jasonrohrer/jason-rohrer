

curl -o ~/tempWebPush/flag.txt http://45.56.104.43/flag.txt

if grep -q "1" ~/tempWebPush/flag.txt; then

    wget -O ~/tempWebPush/frontDoor.jpg "http://192.168.88.230:88/cgi-bin/CGIProxy.fcgi?cmd=snapPicture2&usr=admin&pwd="


    wget -O ~/tempWebPush/backDoor.jpg "http://192.168.88.231:88/cgi-bin/CGIProxy.fcgi?cmd=snapPicture2&usr=admin&pwd="


    wget -O ~/tempWebPush/garage.jpg "http://192.168.88.232:88/cgi-bin/CGIProxy.fcgi?cmd=snapPicture2&usr=admin&pwd="


    wget -O ~/tempWebPush/sideDrive.jpg "http://192.168.88.219:88/cgi-bin/CGIProxy.fcgi?cmd=snapPicture2&usr=admin&pwd=admin"


    wget -O ~/tempWebPush/frontDrive.jpg "http://192.168.88.199:88/cgi-bin/CGIProxy.fcgi?cmd=snapPicture2&usr=admin&pwd=admin"


    wget -O ~/tempWebPush/frontYard.jpg "http://192.168.88.197:88/cgi-bin/CGIProxy.fcgi?cmd=snapPicture2&usr=admin&pwd=admin"


    wget -O ~/tempWebPush/bulkhead.jpg "http://192.168.88.195:88/cgi-bin/CGIProxy.fcgi?cmd=snapPicture2&usr=admin&pwd=admin"

    wget -O ~/tempWebPush/rearDrive.jpg "http://192.168.88.194:88/cgi-bin/CGIProxy.fcgi?cmd=snapPicture2&usr=admin&pwd=admin"


    wget -O ~/tempWebPush/sideYardFront.jpg "http://192.168.88.183:88/cgi-bin/CGIProxy.fcgi?cmd=snapPicture2&usr=admin&pwd=admin"


    cd ~/tempWebPush

    scp *.jpg jcr13@45.56.104.43:public_html/pics
fi
