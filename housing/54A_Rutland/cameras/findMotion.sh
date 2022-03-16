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

pathDestMotion="$basePath/$year/${monthNum}_$monthName/${dayNum}_$dayName/motion_$cameraName/"

pathDest="$basePath/$year/${monthNum}_$monthName/${dayNum}_$dayName/"

mkdir -p $pathSnaps
mkdir -p $pathDestMotion
mkdir -p $pathDest


fileName="${cameraName}_fullDay_motion_timelapse.mp4"


n="0"
prevFile=""
for f in ${pathSnaps}*.jpg;
do
	if [ $n -gt 0 ];
	then
		diffCount=`compare -metric AE -fuzz 20%  $prevFile $f NULL 2>&1`

		if [ "$diffCount" -gt 1500 ];
		then
			name=$(basename $f)
			echo "motion in $name"
			cp $f $pathDestMotion
		fi
	fi
	prevFile=$f
	((n++))
done


cd $pathDestMotion
ffmpeg -r 5 -pattern_type glob -i '*.jpg' -c:v libx264 -pix_fmt yuv420p $pathDest$fileName
