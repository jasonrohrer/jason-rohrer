for f in *.MP4; do
	name="${f%%.*}"

	
	echo "Making ${name}.trf"
	ffmpeg -i ${name}.MP4 -vf vidstabdetect=stepsize=32:shakiness=10:accuracy=10:result=${name}.trf -f null -

	echo "Making ${name}_stable.mp4"
	ffmpeg -y -i ${name}.MP4 -vf vidstabtransform=input=${name}.trf:zoom=0:smoothing=10,unsharp=5:5:0.8:3:3:0.4 -vcodec libx264 -tune film -acodec aac -preset slow ${name}_stable.mp4
	
done
