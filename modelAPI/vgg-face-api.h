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
     * @param scriptToExecute - Path to script.
     * @return pair of dictionaries with person id as string and face coordinates as array of integer values.
     *         first  – dictionary of known people.
     *         second – dictionary of unknown people.
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
     std::pair< std::map<std::string, Coords>, std::map<std::string, Coords> > findLabeledFaceRect(const std::string& img, const std::string& template_img_path, const std::string& scriptsFolder = "");

} // namespace UUUU
