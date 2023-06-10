for f in *.MTS; do
	name="${f%%.*}"

	echo "Making ${name}_deint.mp4"

	ffmpeg -i $f -filter:v bwdif=mode=send_field:parity=auto:deint=all -vcodec libx264 -tune film -acodec aac -preset slow ${name}_deint.mp4

	
	echo "Making ${name}.trf"
	ffmpeg -i ${name}_deint.mp4 -vf vidstabdetect=stepsize=32:shakiness=10:accuracy=10:result=${name}.trf -f null -

	echo "Making ${name}_stable.mp4"
	ffmpeg -y -i ${name}_deint.mp4 -vf vidstabtransform=input=${name}.trf:zoom=0:smoothing=10,unsharp=5:5:0.8:3:3:0.4 -vcodec libx264 -tune film -acodec aac -preset slow ${name}_stable.mp4
	
done
