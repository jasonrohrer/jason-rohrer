# specify camera name (no spaces) as first argument
cameraName=$1

# script should be called shortly after 1:00 AM of the following day



basePath="/var/www/html/camera"



# subtract 3 hours to get us squarely back in yesterday

year=`date -d '3 hours ago' +"%Y"`
monthNum=`date -d '3 hours ago' +"%m"`
monthName=`date -d '3 hours ago' +"%b"`
dayNum=`date -d '3 hours ago' +"%d"`
dayName=`date -d '3 hours ago' +"%a"`


pathSnaps="$basePath/$year/${monthNum}_$monthName/${dayNum}_$dayName/snapsWorking_$cameraName/"
pathMotion="$basePath/$year/${monthNum}_$monthName/${dayNum}_$dayName/motionSnaps_$cameraName/"

pathDest="$basePath/$year/${monthNum}_$monthName/${dayNum}_$dayName/"

mkdir -p $pathSnaps
mkdir -p $pathMotion
mkdir -p $pathDest


fileName="${cameraName}_fullDay_timelapse.mp4"

motionFileName="${cameraName}_fullDay_motion_timelapse.mp4"


cd $pathSnaps
ffmpeg -y -r 30 -pattern_type glob -i '*.jpg' -c:v libx264 -pix_fmt yuv420p $pathDest$fileName

cd $pathMotion
ffmpeg -y -r 5 -pattern_type glob -i '*.jpg' -c:v libx264 -pix_fmt yuv420p $pathDest$motionFileName



# now clear any files older than 30 days
cd $basePath

# make sure we don't delete the dashboard by accident
touch dashboard.html

find . -mtime +30 -delete 
