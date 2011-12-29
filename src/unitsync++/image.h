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

typedef cimg_library::CImg<unsigned short> RawImage;

/** we use this class mostly to hide the cimg implementation details
 *
 */
class UnitsyncImage
{
public:
    UnitsyncImage();
    UnitsyncImage( int width, int height, bool clear = true ) {}
    boost::shared_ptr<RawImage> m_data;
};

} //namespace LSL

#endif // LSL_IMAGE_H
