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
#include <Python/Python.h>


namespace UUUU
{
    /**
     * Define whether two images describe the same person or not.
     * 
     * @param img1 - First image to be examined.
     * @param img2 - Second image to be examined.
     * @return 1 - if images describe the **same** person
     *         0 - if images describe two **different** people
     *         2 - if error `ERR_CMD_LINE_ARGS` occured.
     */
    int isEqualFace(std::string const & img1, std::string const & img2);
    
    using Coords = std::array<int, 5>;
    /**
     * Get coordinates of face rectangle for each face on the image.
     * 
     * @param img - Image on which faces are located.
     * @return vector of arrays with coordinates for each face, 
     *         where array starts with face unique index.
     */
     std::vector< Coords > findFaceRect(std::string const & img);
} // namespace UUUU
