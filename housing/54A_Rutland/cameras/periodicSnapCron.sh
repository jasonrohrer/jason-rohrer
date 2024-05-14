# specify camera name (no spaces) as first argument
cameraName=$1
# specify camera IP as second argument
cameraIP=$2
# specify camera password as third argument (in quotes)
pass=$3


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

# if file contains motion compared to last file, save copy here
pathDestMotion="$basePath/$year/${monthNum}_$monthName/${dayNum}_$dayName/motionSnaps_$cameraName/"


lastFile=`ls -Art $path | tail -n 1`


mkdir -p $path
mkdir -p $pathDestMotion



fileName="${hourNum}_${minuteNum}_$secondsNum.jpg"

wget "http://$cameraIP:88/cgi-bin/CGIProxy.fcgi?cmd=snapPicture2&usr=admin&pwd=$pass" -O $path$fileName


if [ -n "$lastFile" ];
then
	# last file exits

	diffCount=`compare -metric AE -fuzz 20%  $path$lastFile $path$fileName NULL 2>&1`

	if [ "$diffCount" -gt 700 ];
	then
		# new file contains motion compared to last one
		cp $path$fileName $pathDestMotion
	fi
fi
