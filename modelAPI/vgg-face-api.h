//
//  vgg-face-api.h
//
//
//  Created by Dmitry Sokolov on 13/04/2019.
//

#include <string>
#include <array>
#include <map>

namespace UUUU
{

    using Coords = std::array<int, 4>;
    /**
     * Get coordinates of face rectangle for each face on the image.
     * 
     * @param img - Image on which faces are located.
     * @param template_img_path - Path to template images.
     * @return dictionary with person id (string) as key and face coordinates (array of integer values) as value.
     * 
     * Here is specification of coordinates on each array in result vector.
     * `resultArray[0]` - Person id;
     * `resultArray[1]` - startX;
     * `resultArray[2]` - endX;
     * `resultArray[3]` - startY;
     * `resultArray[4]` - endY;
     *
     * startX ---------------------------
     * |                                |
     * |                                |
     * |                                |
     * |                                |
     * endX -----------------------------
     *
     * startY -----------------------endY
     * |                                |
     * |                                |
     * |                                |
     * |                                |
     * ----------------------------------
     */
     std::map<std::string, Coords> findLabeledFaceRect(const std::string& img, const std::string& template_img_path, const std::string& scriptsFolder = "");

} // namespace UUUU
