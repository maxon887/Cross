/*
    Jonathan Dummer

    Image helper functions

    MIT license
*/

#ifndef HEADER_IMAGE_HELPER
#define HEADER_IMAGE_HELPER



/**
	This function upscales an image.
	Not to be used to create MIPmaps,
	but to make it square,
	or to make it a power-of-two sized.
**/
int	up_scale_image
	(
		const unsigned char* const orig,
		int width, int height, int channels,
		unsigned char* resampled,
		int resampled_width, int resampled_height
	);

/**
	This function downscales an image.
	Used for creating MIPmaps,
	the incoming image should be a
	power-of-two sized.
**/
int	mipmap_image
	(
		const unsigned char* const orig,
		int width, int height, int channels,
		unsigned char* resampled,
		int block_size_x, int block_size_y
	);


#endif /* HEADER_IMAGE_HELPER	*/
