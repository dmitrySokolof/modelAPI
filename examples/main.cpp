
#include <iostream>
#include <modelAPI/vgg-face-api.h>

/// EXAMPLE of usage.
int main()
{
    /// MARK: last '/' is important and needed.
    std::string template_img_path = "/Users/dmitrysokolov/Documents/Diplom_dataset/template_dataset/";
    std::string img3 = "/Users/dmitrysokolov/Documents/Diplom_dataset/test_group_images/Derduga_Ryabov_Fedorov.jpg";
    auto resMap = UUUU::findLabeledFaceRect(img3, template_img_path);
    for (std::map< std::string, UUUU::Coords >::iterator i = resMap.begin(); i != resMap.end(); i++)
    {
        std::cout << i->first << '\t' << i->second[0] << ' ' << i->second[1] << ' ' << i->second[2] << ' ' <<  i->second[3] << std::endl;
    }

    return 0;
}