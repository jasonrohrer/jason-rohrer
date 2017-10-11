 (define (eps2png filenameEPS
                  filenamePNG)
   (let* ((image (car (gimp-file-load RUN-NONINTERACTIVE filename filename)))
          (drawable (car (gimp-image-get-active-layer image))))
	  (plug-in-unsharp-mask RUN-NONINTERACTIVE
	                        image drawable radius amount threshold)
	  (gimp-file-save RUN-NONINTERACTIVE image drawable filename filename)
	  (gimp-image-delete image)))


; need to use file-ps-load and file-ps-load-setargs somehow
(file-ps-load-setargs resolution width height check_bbox pages coloring TextAlphaBits GraphicsAlphaBits)