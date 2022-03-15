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
hourNum=`date +"%I"`
hourMeridian=`date +"%p"`

minuteNum=`date +"%M"`


path="$basePath/$year/${monthNum}_$monthName/${dayNum}_$dayName/${hourMeridian}_$hourNum/$cameraName/"



fileName="${dayName}_${hourMeridian}_${hourNum}_${minuteNum}.mp4"

mkdir -p $path

ffmpeg -rtsp_transport tcp -i rtsp://admin:@$cameraIP:88/videoMain -c copy -an -t 600 $path$fileName
