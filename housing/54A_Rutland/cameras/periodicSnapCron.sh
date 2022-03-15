# specify camera name (no spaces) as first argument
cameraName=$1
# specify camera IP as second argument
cameraIP=$2


basePath="/var/www/html/camera"



year=`date +"%Y"`
monthNum=`date +"%m"`
monthName=`date +"%b"`
dayNum=`date +"%d"`
dayName=`date +"%a"`
hourNum=`date +"%H"`
minuteNum=`date +"%M"`
secondsNum=`date +"%S"`


path="$basePath/$year/${monthNum}_$monthName/${dayNum}_$dayName/snapsWorking_$cameraName/"

mkdir -p $path


fileName="${hourNum}_${minuteNum}_$secondsNum.jpg"

wget "http://$cameraIP:88/cgi-bin/CGIProxy.fcgi?cmd=snapPicture2&usr=admin&pwd=" -O $path$fileName
