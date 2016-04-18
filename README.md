The original readme:


Implementation of the segmentation algorithm described in:

Efficient Graph-Based Image Segmentation
Pedro F. Felzenszwalb and Daniel P. Huttenlocher
International Journal of Computer Vision, 59(2) September 2004.

The program takes a color image (PPM format) and produces a segmentation
with a random color assigned to each region.

1) Type "make" to compile "segment".

2) Run "segment sigma k min input output".

The parameters are: (see the paper for details)

sigma: Used to smooth the input image before segmenting it.
k: Value for the threshold function.
min: Minimum component size enforced by post-processing.
input: Input image.
output: Output image.

Typical parameters are sigma = 0.5, k = 500, min = 20.
Larger values for k result in larger components in the result.

-----

I have changed the source code into the opencv version. You can useed it with the 
image format that the openCV support.

The source code can be found at: http://cs.brown.edu/~pff/segment/

The project can be used in both Visual Studio and Ubuntu. If you want to use in Ubuntu,
please step into segment directory and type "make" to compile "segment".