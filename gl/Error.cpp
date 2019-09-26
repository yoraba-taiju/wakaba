//
// Created by kaede on 2019/09/26.
//

#include "Error.hpp"

namespace gl {

Error::Error(std::string const& err)
:std::runtime_error(err)
{

}

}
