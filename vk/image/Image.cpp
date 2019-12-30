//
// Created by kaede on 2019/12/30.
//

#include "Image.hpp"
#include "../Vulkan.hpp"

namespace vk {

Image::~Image() noexcept {
  std::shared_ptr<Vulkan> vulkan =  vulkan_.lock();
  if(vulkan) {
    vkDestroyImageView(vulkan->vkDevice(), vkImageView_, nullptr);
    vkDestroyImage(vulkan->vkDevice(), vkImage_, nullptr);
  }
}

}
