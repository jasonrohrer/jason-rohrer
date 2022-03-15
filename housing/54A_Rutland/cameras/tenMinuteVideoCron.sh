# specify camera name (no spaces) as first argument
cameraName=$1


basePath="/var/www/html/camera"



year=`date +"%Y"`
monthNum=`date +"%m"`
monthName=`date +"%b"`
dayNum=`date +"%d"`
dayName=`date +"%a"`
hourNum=`date +"%I"`
hourMeridian=`date +"%p"`

minuteNum=`date +"%M"`


path="$basePath/$cameraName/$year/${monthNum}_$monthName/${dayNum}_$dayName/${hourMeridian}_$hourNum/"



fileName="${dayName}_${hourMeridian}_${hourNum}_${minuteNum}.mp4"

mkdir -p $path

ffmpeg -rtsp_transport tcp -i rtsp://admin:@192.168.88.230:88/videoMain -c copy -an -t 600 $path$fileName
