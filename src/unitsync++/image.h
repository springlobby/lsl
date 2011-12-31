#ifndef LSL_IMAGE_H
#define LSL_IMAGE_H

#include <boost/shared_ptr.hpp>

//we really, really don't want to include the cimg
// header here, it's 2.1MB of template magic :)
namespace cimg_library {
template <class T>
class CImg;
}

namespace LSL {

namespace Util {
template <class T>
class uninitialized_array;
}
/** we use this class mostly to hide the cimg implementation details
 * \todo decide/implement COW
 */
class UnitsyncImage
{
private:
    typedef unsigned short
        RawDataType;
    typedef cimg_library::CImg<RawDataType>
        PrivateImageType;
	typedef boost::shared_ptr<PrivateImageType>
		PrivateImagePtrType;
public:
	UnitsyncImage();
	explicit UnitsyncImage( int width, int height );
	UnitsyncImage( const std::string& filename );

	void Save( const std::string& path ) const;
	void Load( const std::string& path ) const;

	static UnitsyncImage FromMinimapData( const RawDataType* data, int width, int height );
	static UnitsyncImage FromHeightmapData( const Util::uninitialized_array<unsigned short>& data, int width, int height );
	static UnitsyncImage FromMetalmapData( const Util::uninitialized_array<unsigned char>& data, int width, int height );
	static UnitsyncImage FromVfsFileData(  Util::uninitialized_array<char>& data, size_t size, const std::string& fn, bool useWhiteAsTransparent = true );

	int GetWidth() const;
	int GetHeight() const;
	void Rescale( const int new_width, const int new_height);
private:
	UnitsyncImage( PrivateImagePtrType ptr );
	static PrivateImageType* NewImagePtr( int width = 0, int height = 0 );
	PrivateImagePtrType m_data_ptr;
};

} //namespace LSL

#endif // LSL_IMAGE_H
