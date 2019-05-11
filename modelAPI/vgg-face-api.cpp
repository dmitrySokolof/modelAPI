//
//  vgg-face-api.cpp
//  
//
//  Created by Dmitry Sokolov on 13/04/2019.
//

#include "vgg-face-api.h"

#include <iostream>
#include <fstream>
#include <algorithm>
#include <sstream>
#include <cstdlib>
#include <cstdio>

/// available via link: https://sourceforge.net/projects/pstreams/
#include <pstreams-1.0.1/pstream.h>

#include <Python/Python.h>

#define ERR_CMD_LINE_ARGS 2 /// invalid number of arguments
#define ERR_UNKNOWN_PERSON 1 /// unknown person

namespace
{
    const auto MakeScriptPath = [](const auto& dir, const auto& scriptName) -> std::string
    {
        const std::string dirNormalised = [&dir]()
        {
            std::string dirNormalised = dir;
            if (!dirNormalised.empty() && *dirNormalised.rbegin() != '/')
                dirNormalised += '/';
            return std::move(dirNormalised);
        }();
        
        return dirNormalised + scriptName;
    };
    
/**
 * Internal usage **only**.
 * Define id of person choosing from template image dataset.
 *
 * @param template_img_path - Path to template images.
 * @return ERR_UNKNOWN_PERSON - if unknown person
 *         ERR_CMD_LINE_ARGS - if invalid number of arguments
 *         person id
 */
std::vector<std::string> _whichFace(const std::string& template_img_path, const std::string& scriptsFolder, const int& thread_id)
{
    const std::string scriptToExecute = MakeScriptPath(scriptsFolder, "vgg-face.py");

    const std::string command = "python " + scriptToExecute + " " + template_img_path + " " + std::to_string(thread_id);

    std::vector<std::string> result;

    redi::ipstream in(command);
    std::string buffer;
    while (in >> buffer)
    {
        if (buffer == "1")
        {
            result.push_back("Unknown");
        }
        else if (buffer == "2")
        {
            std::cerr << "_whichFace: error code: 2" << std::endl;
            return result;
        }
        else
        {
            result.push_back(buffer);
        }
    }

    return result;
}

} // namespace

namespace UUUU
{
    /**
     * Get coordinates of face rectangle for each face on the image.
     * 
     * @param img - Image on which faces are located.
     * @param template_img_path - Path to template images.
     * @return dictionary with person id as string and face coordinates as array of integer values.
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
    std::map<std::string, Coords> findLabeledFaceRect(const std::string& img, const std::string& template_img_path, const std::string& scriptsFolder)
    {
        const std::string scriptToExecute = MakeScriptPath(scriptsFolder, "face_extractor.py");
        
        const std::string command = "python " + scriptToExecute + " " + img;

        redi::ipstream in(command);
        std::string buffer;

        std::map<std::string, Coords> result;
        Coords temp;
        int counter = 0;
        std::string currentNumber = "";
        std::string currentID = "";

        bool isFisrt = true;
        int thread_id = 0;

        while (in >> buffer) 
        {
            if (buffer == "err")
            {
                std::cerr << "findFaceRect: error code: 2" << std::endl;
                return result;
            }

            // if line starts from '.', finish reading.
            else if (buffer == ".")
            {
                break;
            }

            // if line starts from ',', read new number
            else if (buffer == ",")
            {
                if (counter == 0)
                {
                    currentID = currentNumber;
                    counter++;
                    continue;
                }
                temp[counter - 1] = std::stol(currentNumber);
                if(counter == 4)
                {
                    result.insert(std::pair<std::string, Coords>(currentID, temp));
                    counter = 0;
                    currentID = "";
                    currentNumber = "";
                    continue;
                }
                counter++;
            }

            // if line starts from number, read the whole number
            else if (buffer != ",")
            {
                if(isFisrt)
                {
                    isFisrt = false;
                    thread_id = std::stoi(buffer);
                    continue;
                }
                currentNumber = buffer;
            }
        }

        std::vector< std::string > labels = _whichFace(template_img_path, scriptsFolder, thread_id);

        /// MARK: it should be guaranteed that labels and result size are equal
        int j = 0;
        auto tempMap = result;
        for (std::map< std::string, Coords >::iterator i = result.begin(); i != result.end(); i++)
        {
            // TODO: Fix insertion of equal names.
            auto key = i->first;
            if (j >= labels.size())
            {
                break;
            }
            // C++17 needed.
            auto nodeHandler = tempMap.extract(key);
            nodeHandler.key() = labels[j];
            tempMap.insert(std::move(nodeHandler));
            j++;
        }
        
        result = tempMap;

        return result;
    }
} // namespace UUUU
