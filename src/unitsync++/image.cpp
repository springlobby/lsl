#include "image.h"

#include <cimg/CImg.h>
#include <utils/misc.h>

namespace LSL {

#define NEW_PTR new PrivateImageType( width, height, 1, 3 )
#define DEFINE_PTR(name) PrivateImageType* name = NEW_PTR

UnitsyncImage::UnitsyncImage( int width, int height )
	: m_data_ptr( NEW_PTR )
{
}

UnitsyncImage::UnitsyncImage(PrivateImagePtrType ptr)
	: m_data_ptr( ptr )
{
}

UnitsyncImage UnitsyncImage::FromMetalmapData(const Util::uninitialized_array<unsigned char>& data, int width, int height)
{
	DEFINE_PTR(img_p);
	PrivateImageType& img = *img_p;
	cimg_forXY(img,x,y) {
		img(x,y,0,0) = 0;
		img(x,y,0,1) = data[x+(y*width)];
		img(x,y,0,2) = 0;
	}
	PrivateImagePtrType ptr( img_p );
	return UnitsyncImage( ptr );
}

void UnitsyncImage::Save(const std::string& path) const
{
	assert( m_data_ptr );
	m_data_ptr->save( path.c_str() );
}

UnitsyncImage UnitsyncImage::FromMinimapData(const UnitsyncImage::RawDataType *colours, int width, int height)
{
	DEFINE_PTR(img_p);
	PrivateImageType& img = *img_p;
	cimg_forXY(img,x,y) {
		int at = x+(y*width);
		img(x,y,0,0) = (unsigned char)( (( colours[at] >> 11 )/31.0)*255.0 );
		img(x,y,0,1) = (unsigned char)( (( (colours[at] >> 5) & 63 )/63.0)*255.0 );
		img(x,y,0,2) = (unsigned char)( (( colours[at] & 31 )/31.0)*255.0 );
	}
	PrivateImagePtrType ptr( img_p );
	return UnitsyncImage( ptr );
}

UnitsyncImage UnitsyncImage::FromHeightmapData(const Util::uninitialized_array<unsigned short>& grayscale, int width, int height)
{
	DEFINE_PTR(img_p);
	PrivateImageType& img = *img_p;

	// the height is mapped to this "palette" of colors
	// the colors are linearly interpolated
	const unsigned char points[][3] = {
		{   0,   0,   0 },
		{   0,   0, 255 },
		{   0, 255, 255 },
		{   0, 255,   0 },
		{ 255, 255,   0 },
		{ 255,   0,   0 },
	};
	const int numPoints = sizeof(points) / sizeof(points[0]);

	// find range of values present in the height data returned by unitsync
	int min = 65536;
	int max = 0;

	for ( int i = 0; i < width*height; i++ ) {
		if (grayscale[i] < min) min = grayscale[i];
		if (grayscale[i] > max) max = grayscale[i];
	}

	// prevent division by zero -- heightmap wouldn't contain any information anyway
	if (min == max)
		return UnitsyncImage( 1, 1 );

	// perform the mapping From 16 bit grayscale to 24 bit true colour
	const double range = max - min + 1;
//	for ( int i = 0; i < width*height; i++ ) {
	cimg_forXY(img,x,y) {
		const int i = x + (y*width);
		const double value = (grayscale[i] - min) / (range / (numPoints - 1));
		const int idx1 = int(value);
		const int idx2 = idx1 + 1;
		const int t = int(256.0 * (value - std::floor(value)));

		//assert(idx1 >= 0 && idx1 < numPoints-1);
		//assert(idx2 >= 1 && idx2 < numPoints);
		//assert(t >= 0 && t <= 255);
		cimg_forC(img,j) {
			img(x,y,0,j) = (points[idx1][j] * (255 - t) + points[idx2][j] * t) / 255;
		}
	}

	PrivateImagePtrType ptr( img_p );
	return UnitsyncImage( ptr );
}


} // namespace LSL
