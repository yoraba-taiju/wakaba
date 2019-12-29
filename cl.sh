# /bin/bash
find . \( -type f -name "*.cpp" -print -or -name "*.hpp" -print \) -or \( -type d -and \( -name "cmake-*" -or -name "external" \) -and -prune \) | xargs wc -l
