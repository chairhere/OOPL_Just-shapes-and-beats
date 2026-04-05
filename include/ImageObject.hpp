//
// Created by cheese on 2026/4/5.
//

#ifndef JUST_SHAPES_AND_BEATS_IMAGEOBJECT_HPP
#define JUST_SHAPES_AND_BEATS_IMAGEOBJECT_HPP
#include "Util/GameObject.hpp"
#include "Util/Image.hpp"

class ImageObject : public Util::GameObject {
public:
    ImageObject(const std::string& imagePath) {
        m_Drawable = std::make_shared<Util::Image>(imagePath);
    }

    void SetImage(const std::string& imagePath);
};

#endif //JUST_SHAPES_AND_BEATS_IMAGEOBJECT_HPP