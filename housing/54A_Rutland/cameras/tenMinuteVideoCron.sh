# specify camera name (no spaces) as first argument
cameraName=$1
# specify camera IP as second argument
cameraIP=$2
# specify camera password as third argument (in quotes)
pass=$3

 
audio=0

if [ $# -eq 4 ]
then
	audio=$4
fi




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

if [ $audio -eq 1 ]
then
	# include audio stream, re-encode as aac
	ffmpeg -rtsp_transport tcp -i rtsp://admin:$pass@$cameraIP:88/videoMain -c:v copy -c:a aac -t 600 $path$fileName
else
	# skip audio
	ffmpeg -rtsp_transport tcp -i rtsp://admin:$pass@$cameraIP:88/videoMain -c copy -an -t 600 $path$fileName
fi



# NOTE
# camera video streams SHOULD include audio, but this seems to be stalled
# on many of the cameras (both streams are announced, but only the video
# stream is transmitted by the camera).

# audio streams seem function ONLY for these old cameras (original to house)
#
# frontDoor 192.168.88.230
# backDoor 192.168.88.231
# garage 192.168.88.232
#
# AND this one new camera:
#
# sideYardRear 192.168.88.182
#
# the -an param above skips the audio stream, allowing the video to record
#
# HOWEVER
# For a camera that IS streaming audio, the following ffmpeg command works
# to record both audio and video:
#
# ffmpeg -rtsp_transport tcp -i rtsp://admin:@192.168.88.231:88/videoMain -c:v copy -c:a aac -t 5 out.mp4
#
# Audio alone can be recorded thus:
#
# ffmpeg -rtsp_transport tcp -i rtsp://admin:@192.168.88.231:88/audio -c copy -t 5 out.wav

# In future, when we install an outdoor microphone, we can connect the audio
# to one of the cameras above that supports streaming audio and video.
#
# Then, we can update the userCronTab to take 1 as a 4th parameter for
# the camera that has the microphone.
