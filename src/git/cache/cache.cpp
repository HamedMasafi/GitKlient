//
// Created by hamed on 25.03.22.
//

#include "cache.h"

namespace Git {

    bool Cache::isLoaded() const {
        return _isLoaded;
    }

    void Cache::setLoaded(bool loadedValue) {
        if (_isLoaded == loadedValue)
            return;
        _isLoaded = loadedValue;
        emit loaded();
    }

}