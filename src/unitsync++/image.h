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
	void Save( const std::string& path ) const;
	static UnitsyncImage FromMinimapData( const RawDataType* data, int width, int height );
	static UnitsyncImage FromHeightmapData( const Util::uninitialized_array<unsigned short>& data, int width, int height );
	static UnitsyncImage FromMetalmapData( const Util::uninitialized_array<unsigned char>& data, int width, int height );
private:
    explicit UnitsyncImage( int width, int height );
	UnitsyncImage( PrivateImagePtrType ptr );
	static PrivateImageType* NewImagePtr( int width = 0, int height = 0 );
	PrivateImagePtrType m_data_ptr;
};

} //namespace LSL

#endif // LSL_IMAGE_H
