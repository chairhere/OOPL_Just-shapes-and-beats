//
// Created by cheese on 2026/4/5.
//

#include "ImageObject.hpp"

void ImageObject::SetImage(const std::string &imagePath) {
    if (auto imageDrawable = std::dynamic_pointer_cast<Util::Image>(m_Drawable)) {
        imageDrawable->SetImage(imagePath);
    }
}
