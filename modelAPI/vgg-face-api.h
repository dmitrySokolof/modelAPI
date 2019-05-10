//
//  vgg-face-api.h
//
//
//  Created by Dmitry Sokolov on 13/04/2019.
//

#include <iostream>
#include <string>
#include <vector>
#include <array>
#include <fstream>
#include <algorithm>
#include <sstream>
#include <cstdlib>
#include <cstdio>
#include <map>

/// available via link: https://sourceforge.net/projects/pstreams/
#include <pstreams-1.0.1/pstream.h>
#include <Python/Python.h>


namespace UUUU
{
    /**
     * Internal usage **only**.
     * Define id of person choosing from template image dataset.
     * 
     * @param template_img_path - Path to template images.
     * @return ERR_UNKNOWN_PERSON - if unknown person
     *         ERR_CMD_LINE_ARGS - if invalid number of arguments
     *         person id
     */
    std::vector< std::string > _whichFace(std::string const & template_img_path);
    
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
     std::map<std::string, Coords> findLabeledFaceRect(std::string const & img, std::string const & template_img_path);
} // namespace UUUU
