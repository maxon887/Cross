/*
	Jonathan Dummer
	2007-07-26-10.36

	Simple OpenGL Image Library

	Public Domain
	using Sean Barret's stb_image as a base

	Thanks to:
	* Sean Barret - for the awesome stb_image
	* Dan Venkitachalam - for finding some non-compliant DDS files, and patching some explicit casts
	* everybody at gamedev.net
*/

#define SOIL_CHECK_FOR_GL_ERRORS 0

#ifdef _WIN32
	#define WIN32_LEAN_AND_MEAN
	#include <windows.h>
	#include <wingdi.h>
	#include <GL/gl.h>
#elif defined(__APPLE__) || defined(__APPLE_CC__)
	/*	I can't test this Apple stuff!	*/
    #include <OpenGLES/ES1/gl.h>
	//#include <Carbon/Carbon.h>
	#define APIENTRY
#elif defined(ANDROID) || defined(__ANDROID__)
	#include <GLES/gl.h>
#else
	#include <GL/gl.h>
	#include <GL/glx.h>
#endif

#include "SOIL.h"
#include "stb_image_aug.h"
#include "image_helper.h"
#include <stdlib.h>
#include <string.h>

/*	error reporting	*/
char *result_string_pointer = "SOIL initialized";

/*	for loading cube maps	*/
enum{
	SOIL_CAPABILITY_UNKNOWN = -1,
	SOIL_CAPABILITY_NONE = 0,
	SOIL_CAPABILITY_PRESENT = 1
};
static int has_cubemap_capability = SOIL_CAPABILITY_UNKNOWN;
int query_cubemap_capability( void );
#define SOIL_TEXTURE_WRAP_R					0x8072
#define SOIL_CLAMP_TO_EDGE					0x812F
#define SOIL_NORMAL_MAP						0x8511
#define SOIL_REFLECTION_MAP					0x8512
#define SOIL_TEXTURE_CUBE_MAP				0x8513
#define SOIL_TEXTURE_BINDING_CUBE_MAP		0x8514
#define SOIL_TEXTURE_CUBE_MAP_POSITIVE_X	0x8515
#define SOIL_TEXTURE_CUBE_MAP_NEGATIVE_X	0x8516
#define SOIL_TEXTURE_CUBE_MAP_POSITIVE_Y	0x8517
#define SOIL_TEXTURE_CUBE_MAP_NEGATIVE_Y	0x8518
#define SOIL_TEXTURE_CUBE_MAP_POSITIVE_Z	0x8519
#define SOIL_TEXTURE_CUBE_MAP_NEGATIVE_Z	0x851A
#define SOIL_PROXY_TEXTURE_CUBE_MAP			0x851B
#define SOIL_MAX_CUBE_MAP_TEXTURE_SIZE		0x851C
/*	for non-power-of-two texture	*/
static int has_NPOT_capability = SOIL_CAPABILITY_UNKNOWN;
int query_NPOT_capability( void );
/*	for texture rectangles	*/
static int has_tex_rectangle_capability = SOIL_CAPABILITY_UNKNOWN;
int query_tex_rectangle_capability( void );
#define SOIL_TEXTURE_RECTANGLE_ARB				0x84F5
#define SOIL_MAX_RECTANGLE_TEXTURE_SIZE_ARB		0x84F8
/*	for using DXT compression	*/
int query_DXT_capability( void );
#define SOIL_RGB_S3TC_DXT1		0x83F0
#define SOIL_RGBA_S3TC_DXT1		0x83F1
#define SOIL_RGBA_S3TC_DXT3		0x83F2
#define SOIL_RGBA_S3TC_DXT5		0x83F3

/*	other functions	*/
unsigned int
	SOIL_internal_create_OGL_texture
	(
		const unsigned char *const data,
		int width, int height, int channels,
		unsigned int reuse_texture_ID,
		unsigned int flags,
		unsigned int opengl_texture_type,
		unsigned int opengl_texture_target,
		unsigned int texture_check_size_enum
	);

/*	and the code magic begins here [8^)	*/
unsigned int SOIL_load_OGL_texture(	const char *filename,
									int force_channels,
									unsigned int reuse_texture_ID,
									unsigned int flags )
{
	/*	variables	*/
	unsigned char* img;
	int width, height, channels;
	unsigned int tex_id;

	/*	try to load the image	*/
	img = SOIL_load_image( filename, &width, &height, &channels, force_channels );
	/*	channels holds the original number of channels, which may have been forced	*/
	if( (force_channels >= 1) && (force_channels <= 4) )
	{
		channels = force_channels;
	}
	if( NULL == img )
	{
		/*	image loading failed	*/
		result_string_pointer = stbi_failure_reason();
		return 0;
	}
	/*	OK, make it a texture!	*/
	tex_id = SOIL_internal_create_OGL_texture(
			img, width, height, channels,
			reuse_texture_ID, flags,
			GL_TEXTURE_2D, GL_TEXTURE_2D,
			GL_MAX_TEXTURE_SIZE );
	/*	and nuke the image data	*/
	SOIL_free_image_data( img );
	/*	and return the handle, such as it is	*/
	return tex_id;
}

unsigned int SOIL_create_OGL_texture(	const unsigned char *const data,
										int width, int height, int channels,
										unsigned int reuse_texture_ID,
										unsigned int flags){
	/*	wrapper function for 2D textures	*/
	return SOIL_internal_create_OGL_texture(
				data, width, height, channels,
				reuse_texture_ID, flags,
				GL_TEXTURE_2D, GL_TEXTURE_2D,
				GL_MAX_TEXTURE_SIZE );
}

#if SOIL_CHECK_FOR_GL_ERRORS
void check_for_GL_errors( const char *calling_location )
{
	/*	check for errors	*/
	GLenum err_code = glGetError();
	while( GL_NO_ERROR != err_code )
	{
		printf( "OpenGL Error @ %s: %i", calling_location, err_code );
		err_code = glGetError();
	}
}
#else
void check_for_GL_errors( const char *calling_location )
{
	/*	no check for errors	*/
}
#endif

unsigned int SOIL_internal_create_OGL_texture(	const unsigned char *const data,
												int width, int height, int channels,
												unsigned int reuse_texture_ID,
												unsigned int flags,
												unsigned int opengl_texture_type,
												unsigned int opengl_texture_target,
												unsigned int texture_check_size_enum)
{
	/*	variables	*/
	unsigned char* img;
	unsigned int tex_id;
	unsigned int internal_texture_format = 0, original_texture_format = 0;
	int DXT_mode = SOIL_CAPABILITY_UNKNOWN;
	int max_supported_size;
	/*	If the user wants to use the texture rectangle I kill a few flags	*/
	if( flags & SOIL_FLAG_TEXTURE_RECTANGLE )
	{
		/*	well, the user asked for it, can we do that?	*/
		if( query_tex_rectangle_capability() == SOIL_CAPABILITY_PRESENT )
		{
			/*	only allow this if the user in _NOT_ trying to do a cubemap!	*/
			if( opengl_texture_type == GL_TEXTURE_2D )
			{
				/*	clean out the flags that cannot be used with texture rectangles	*/
				flags &= ~(
						SOIL_FLAG_POWER_OF_TWO | SOIL_FLAG_MIPMAPS |
						SOIL_FLAG_TEXTURE_REPEATS
					);
				/*	and change my target	*/
				opengl_texture_target = SOIL_TEXTURE_RECTANGLE_ARB;
				opengl_texture_type = SOIL_TEXTURE_RECTANGLE_ARB;
			} else
			{
				/*	not allowed for any other uses (yes, I'm looking at you, cubemaps!)	*/
				flags &= ~SOIL_FLAG_TEXTURE_RECTANGLE;
			}

		} else
		{
			/*	can't do it, and that is a breakable offense (uv coords use pixels instead of [0,1]!)	*/
			result_string_pointer = "Texture Rectangle extension unsupported";
			return 0;
		}
	}
	/*	create a copy the image data	*/
	img = (unsigned char*)malloc( width*height*channels );
	memcpy( img, data, width*height*channels );
	/*	does the user want me to invert the image?	*/
	if( flags & SOIL_FLAG_INVERT_Y )
	{
		int i, j;
		for( j = 0; j*2 < height; ++j )
		{
			int index1 = j * width * channels;
			int index2 = (height - 1 - j) * width * channels;
			for( i = width * channels; i > 0; --i )
			{
				unsigned char temp = img[index1];
				img[index1] = img[index2];
				img[index2] = temp;
				++index1;
				++index2;
			}
		}
	}
	/*	does the user want me to convert from straight to pre-multiplied alpha?
		(and do we even _have_ alpha?)	*/
	if( flags & SOIL_FLAG_MULTIPLY_ALPHA )
	{
		int i;
		switch( channels )
		{
		case 2:
			for( i = 0; i < 2*width*height; i += 2 )
			{
				img[i] = (img[i] * img[i+1] + 128) >> 8;
			}
			break;
		case 4:
			for( i = 0; i < 4*width*height; i += 4 )
			{
				img[i+0] = (img[i+0] * img[i+3] + 128) >> 8;
				img[i+1] = (img[i+1] * img[i+3] + 128) >> 8;
				img[i+2] = (img[i+2] * img[i+3] + 128) >> 8;
			}
			break;
		default:
			/*	no other number of channels contains alpha data	*/
			break;
		}
	}
	/*	if the user can't support NPOT textures, make sure we force the POT option	*/
	if( (query_NPOT_capability() == SOIL_CAPABILITY_NONE) &&
		!(flags & SOIL_FLAG_TEXTURE_RECTANGLE) )
	{
		/*	add in the POT flag */
		flags |= SOIL_FLAG_POWER_OF_TWO;
	}
	/*	how large of a texture can this OpenGL implementation handle?	*/
	/*	texture_check_size_enum will be GL_MAX_TEXTURE_SIZE or SOIL_MAX_CUBE_MAP_TEXTURE_SIZE	*/
	glGetIntegerv( texture_check_size_enum, &max_supported_size );
	/*	do I need to make it a power of 2?	*/
	if(
		(flags & SOIL_FLAG_POWER_OF_TWO) ||	/*	user asked for it	*/
		(flags & SOIL_FLAG_MIPMAPS) ||		/*	need it for the MIP-maps	*/
		(width > max_supported_size) ||		/*	it's too big, (make sure it's	*/
		(height > max_supported_size) )		/*	2^n for later down-sampling)	*/
	{
		int new_width = 1;
		int new_height = 1;
		while( new_width < width )
		{
			new_width *= 2;
		}
		while( new_height < height )
		{
			new_height *= 2;
		}
		/*	still?	*/
		if( (new_width != width) || (new_height != height) )
		{
			/*	yep, resize	*/
			unsigned char *resampled = (unsigned char*)malloc( channels*new_width*new_height );
			up_scale_image(
					img, width, height, channels,
					resampled, new_width, new_height );
			/*	OJO	this is for debug only!	*/
			/*
			SOIL_save_image( "\\showme.bmp", SOIL_SAVE_TYPE_BMP,
							new_width, new_height, channels,
							resampled );
			*/
			/*	nuke the old guy, then point it at the new guy	*/
			SOIL_free_image_data( img );
			img = resampled;
			width = new_width;
			height = new_height;
		}
	}
	/*	now, if it is too large...	*/
	if( (width > max_supported_size) || (height > max_supported_size) )
	{
		/*	I've already made it a power of two, so simply use the MIPmapping
			code to reduce its size to the allowable maximum.	*/
		unsigned char *resampled;
		int reduce_block_x = 1, reduce_block_y = 1;
		int new_width, new_height;
		if( width > max_supported_size )
		{
			reduce_block_x = width / max_supported_size;
		}
		if( height > max_supported_size )
		{
			reduce_block_y = height / max_supported_size;
		}
		new_width = width / reduce_block_x;
		new_height = height / reduce_block_y;
		resampled = (unsigned char*)malloc( channels*new_width*new_height );
		/*	perform the actual reduction	*/
		mipmap_image(	img, width, height, channels,
						resampled, reduce_block_x, reduce_block_y );
		/*	nuke the old guy, then point it at the new guy	*/
		SOIL_free_image_data( img );
		img = resampled;
		width = new_width;
		height = new_height;
	}
	/*	create the OpenGL texture ID handle
    	(note: allowing a forced texture ID lets me reload a texture)	*/
    tex_id = reuse_texture_ID;
    if( tex_id == 0 )
    {
		glGenTextures( 1, &tex_id );
    }
	check_for_GL_errors( "glGenTextures" );
	/* Note: sometimes glGenTextures fails (usually no OpenGL context)	*/
	if( tex_id )
	{
		/*	and what type am I using as the internal texture format?	*/
		switch( channels )
		{
		case 1:
			original_texture_format = GL_LUMINANCE;
			break;
		case 2:
			original_texture_format = GL_LUMINANCE_ALPHA;
			break;
		case 3:
			original_texture_format = GL_RGB;
			break;
		case 4:
			original_texture_format = GL_RGBA;
			break;
		}
		internal_texture_format = original_texture_format;
		/*	does the user want me to, and can I, save as DXT?	*/
		if( flags & SOIL_FLAG_COMPRESS_TO_DXT )
		{
			DXT_mode = query_DXT_capability();
			if( DXT_mode == SOIL_CAPABILITY_PRESENT )
			{
				/*	I can use DXT, whether I compress it or OpenGL does	*/
				if( (channels & 1) == 1 )
				{
					/*	1 or 3 channels = DXT1	*/
					internal_texture_format = SOIL_RGB_S3TC_DXT1;
				} else
				{
					/*	2 or 4 channels = DXT5	*/
					internal_texture_format = SOIL_RGBA_S3TC_DXT5;
				}
			}
		}
		/*  bind an OpenGL texture ID	*/
		glBindTexture( opengl_texture_type, tex_id );
		check_for_GL_errors( "glBindTexture" );
		/*  upload the main image	*/

			/*	user want OpenGL to do all the work!	*/
			glTexImage2D(
				opengl_texture_target, 0,
				internal_texture_format, width, height, 0,
				original_texture_format, GL_UNSIGNED_BYTE, img );
			check_for_GL_errors( "glTexImage2D" );
			/*printf( "OpenGL DXT compressor\n" );	*/
		/*	are any MIPmaps desired?	*/
		if( flags & SOIL_FLAG_MIPMAPS )
		{
			int MIPlevel = 1;
			int MIPwidth = (width+1) / 2;
			int MIPheight = (height+1) / 2;
			unsigned char *resampled = (unsigned char*)malloc( channels*MIPwidth*MIPheight );
			while( ((1<<MIPlevel) <= width) || ((1<<MIPlevel) <= height) )
			{
				/*	do this MIPmap level	*/
				mipmap_image(
						img, width, height, channels,
						resampled,
						(1 << MIPlevel), (1 << MIPlevel) );

					/*	user want OpenGL to do all the work!	*/
					glTexImage2D(
						opengl_texture_target, MIPlevel,
						internal_texture_format, MIPwidth, MIPheight, 0,
						original_texture_format, GL_UNSIGNED_BYTE, resampled );
					check_for_GL_errors( "glTexImage2D" );
				
				/*	prep for the next level	*/
				++MIPlevel;
				MIPwidth = (MIPwidth + 1) / 2;
				MIPheight = (MIPheight + 1) / 2;
			}
			SOIL_free_image_data( resampled );
			/*	instruct OpenGL to use the MIPmaps	*/
			glTexParameteri( opengl_texture_type, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
			glTexParameteri( opengl_texture_type, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR );
			check_for_GL_errors( "GL_TEXTURE_MIN/MAG_FILTER" );
		} else
		{
			/*	instruct OpenGL _NOT_ to use the MIPmaps	*/
			glTexParameteri( opengl_texture_type, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
			glTexParameteri( opengl_texture_type, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
			check_for_GL_errors( "GL_TEXTURE_MIN/MAG_FILTER" );
		}
		/*	does the user want clamping, or wrapping?	*/
		if( flags & SOIL_FLAG_TEXTURE_REPEATS )
		{
			glTexParameteri( opengl_texture_type, GL_TEXTURE_WRAP_S, GL_REPEAT );
			glTexParameteri( opengl_texture_type, GL_TEXTURE_WRAP_T, GL_REPEAT );
			if( opengl_texture_type == SOIL_TEXTURE_CUBE_MAP )
			{
				/*	SOIL_TEXTURE_WRAP_R is invalid if cubemaps aren't supported	*/
				glTexParameteri( opengl_texture_type, SOIL_TEXTURE_WRAP_R, GL_REPEAT );
			}
			check_for_GL_errors( "GL_TEXTURE_WRAP_*" );
		} else
		{
			unsigned int clamp_mode = SOIL_CLAMP_TO_EDGE;
			//unsigned int clamp_mode = GL_CLAMP;
			glTexParameteri( opengl_texture_type, GL_TEXTURE_WRAP_S, clamp_mode );
			glTexParameteri( opengl_texture_type, GL_TEXTURE_WRAP_T, clamp_mode );
			if( opengl_texture_type == SOIL_TEXTURE_CUBE_MAP )
			{
				/*	SOIL_TEXTURE_WRAP_R is invalid if cubemaps aren't supported	*/
				glTexParameteri( opengl_texture_type, SOIL_TEXTURE_WRAP_R, clamp_mode );
			}
			check_for_GL_errors( "GL_TEXTURE_WRAP_*" );
		}
		/*	done	*/
		result_string_pointer = "Image loaded as an OpenGL texture";
	} else
	{
		/*	failed	*/
		result_string_pointer = "Failed to generate an OpenGL texture name; missing OpenGL context?";
	}
	SOIL_free_image_data( img );
	return tex_id;
}


unsigned char* SOIL_load_image (const char *filename,
                                int *width, int *height, int *channels,
                                int force_channels ) {
	unsigned char *result = stbi_load( filename, width, height, channels, force_channels );
	if( result == NULL )
	{
		result_string_pointer = stbi_failure_reason();
	} else
	{
		result_string_pointer = "Image loaded";
	}
	return result;
}

unsigned char* SOIL_load_image_from_memory(	const unsigned char *const buffer,
											int buffer_length,
											int *width, int *height, int *channels,
											int force_channels)
{
	unsigned char *result = stbi_load_from_memory(	buffer, buffer_length,
													width, height, channels,
													force_channels );
	if( result == NULL )
	{
		result_string_pointer = stbi_failure_reason();
	} else
	{
		result_string_pointer = "Image loaded from memory";
	}
	return result;
}

void SOIL_free_image_data
	(
		unsigned char *img_data
	)
{
	free( (void*)img_data );
}

const char*
	SOIL_last_result
	(
		void
	)
{
	return result_string_pointer;
}

int query_NPOT_capability( void )
{
	/*	check for the capability	*/
	if( has_NPOT_capability == SOIL_CAPABILITY_UNKNOWN )
	{
		/*	we haven't yet checked for the capability, do so	*/
		if(
			(NULL == strstr( (char const*)glGetString( GL_EXTENSIONS ),
				"GL_ARB_texture_non_power_of_two" ) )
			)
		{
			/*	not there, flag the failure	*/
			has_NPOT_capability = SOIL_CAPABILITY_NONE;
		} else
		{
			/*	it's there!	*/
			has_NPOT_capability = SOIL_CAPABILITY_PRESENT;
		}
	}
	/*	let the user know if we can do non-power-of-two textures or not	*/
	return has_NPOT_capability;
}

int query_tex_rectangle_capability( void )
{
	/*	check for the capability	*/
	if( has_tex_rectangle_capability == SOIL_CAPABILITY_UNKNOWN )
	{
		/*	we haven't yet checked for the capability, do so	*/
		if(
			(NULL == strstr( (char const*)glGetString( GL_EXTENSIONS ),
				"GL_ARB_texture_rectangle" ) )
		&&
			(NULL == strstr( (char const*)glGetString( GL_EXTENSIONS ),
				"GL_EXT_texture_rectangle" ) )
		&&
			(NULL == strstr( (char const*)glGetString( GL_EXTENSIONS ),
				"GL_NV_texture_rectangle" ) )
			)
		{
			/*	not there, flag the failure	*/
			has_tex_rectangle_capability = SOIL_CAPABILITY_NONE;
		} else
		{
			/*	it's there!	*/
			has_tex_rectangle_capability = SOIL_CAPABILITY_PRESENT;
		}
	}
	/*	let the user know if we can do texture rectangles or not	*/
	return has_tex_rectangle_capability;
}

int query_cubemap_capability( void )
{
	/*	check for the capability	*/
	if( has_cubemap_capability == SOIL_CAPABILITY_UNKNOWN )
	{
		/*	we haven't yet checked for the capability, do so	*/
		if(
			(NULL == strstr( (char const*)glGetString( GL_EXTENSIONS ),
				"GL_ARB_texture_cube_map" ) )
		&&
			(NULL == strstr( (char const*)glGetString( GL_EXTENSIONS ),
				"GL_EXT_texture_cube_map" ) )
			)
		{
			/*	not there, flag the failure	*/
			has_cubemap_capability = SOIL_CAPABILITY_NONE;
		} else
		{
			/*	it's there!	*/
			has_cubemap_capability = SOIL_CAPABILITY_PRESENT;
		}
	}
	/*	let the user know if we can do cubemaps or not	*/
	return has_cubemap_capability;
}

int query_DXT_capability(){
	return SOIL_CAPABILITY_NONE;
}
