//
//  vgg-face-api.cpp
//  
//
//  Created by Dmitry Sokolov on 13/04/2019.
//

#include "vgg-face-api.h"

#define ERR_CMD_LINE_ARGS 2 /// invalid number of arguments

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
    int isEqualFace(std::string const & img1, std::string const & img2)
    {
        std::string command = "python vgg-face.py " + img1 + " " + img2;

        std::array<char, 2> buffer;
        std::string result;

        FILE* pipe = popen(command.c_str(), "r");
        if (!pipe)
        {
            std::cerr << "isEqualFace: Couldn't start command." << std::endl;
            return 0;
        }

        while (fgets(buffer.data(), 2, pipe) != NULL) 
        {
            // check for error
            if (buffer[0] == '2')
            {
                std::cerr << "isEqualFace: error code: 2" << std::endl;
                return ERR_CMD_LINE_ARGS;
            }

            result += buffer.data();
        }
        pclose(pipe);

        return std::stoi( result );
    }
    
    /**
     * Get coordinates of face rectangle for each face on the image.
     * 
     * @param img - Image on which faces are located.
     * @return vector of arrays with coordinates for each face, 
     *         where array starts with face unique index.
     * 
     * Here is specification of coordinates on each array in result vector.
     * `resultArray[0]` - image id;
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
    std::vector< Coords > findFaceRect(std::string const & img)
    {
        std::string command = "python face_extractor.py " + img;

        const int buff_size = 64;
        std::vector< Coords > result;
        std::array<char, buff_size> buffer;

        // Need this to reduce noise at buffer. At least, artificial noise is better than random.
        for (int i = 0; i < buffer.size(); i++)
        {
            buffer[i] = '#';
        }

        Coords temp;

        FILE* pipe = popen(command.c_str(), "r");
        if (!pipe)
        {
            std::cerr << "findFaceRect: Couldn't start command." << std::endl;
            return result;
        }

        std::string currentNumber;
        int counter = 0;

        // MARK: buffer size is a weak place, where unobvious mistakes might occur if size is overflowing.
        // on each iteration buffer consists of one line of input data.
        while (fgets(buffer.data(), buff_size, pipe) != NULL) 
        {
            // check for error
            if (buffer[0] == 'e' && buffer[1] == 'r' && buffer[2] == 'r')
            {
                std::cerr << "findFaceRect: error code: 2" << std::endl;
                return result;
            }

            // if line starts from '.', finish reading.
            if (buffer[0] == '.')
            {
                break;
            }

            // if line starts from ',', read new number
            else if (buffer[0] == ',')
            {
                temp[counter] = std::stol(currentNumber);
                currentNumber = "";
                if(counter == 4)
                {
                    result.push_back(temp);
                    counter = 0;
                    continue;
                }
                counter++;
            }

            // if line starts from number, read the whole number
            else if (buffer[0] != ',')
            {
                for (int i = 0; i < buffer.size(); i++)
                {
                    // ignore noise
                    if (buffer[i] == '#')
                    {
                        continue;
                    }
                    else
                    {
                        currentNumber += buffer[i];
                    }
                }
            }
        }

        pclose(pipe);

        return result;
    }
} // namespace UUUU

// EXAMPLE of usage.
// int main()
// {
//     std::string img1 = "/Users/dmitrysokolov/datasets/faces/image_0001.jpg";
//     std::string img2 = "/Users/dmitrysokolov/datasets/faces/image_0003.jpg";
//     int res = UUUU::isEqualFace(img1, img2);
//     std::cout << res << std::endl << std::endl << std::endl;


//     std::string img3 = "/Users/dmitrysokolov/Documents/Diplom_dataset/test_group_images/Syomin_Utkin_Derduga.jpg";
//     std::vector< UUUU::Coords > resVector = UUUU::findFaceRect(img3);
//     for (int i = 0; i < resVector.size(); i++)
//     {
//         for (int j = 0; j < 5; j++)
//         {
//             std::cout << resVector[i][j] << std::endl;
//         }
//         std::cout << std::endl;
//     }
    
//     return 0;
// }