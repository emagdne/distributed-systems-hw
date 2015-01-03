//
// neighbors_vector.h
//
// Author: Jordan Brown
// Date: Dec 31, 2014
//
// A thread-safe vector for storing known neighbors

#include <pthread.h>
#include <vector>
#include <string>
#include <algorithm>
#include "neighbors_vector.h"

namespace brown {
    neighbors_vector::neighbors_vector() {
        init();
    }

    neighbors_vector::neighbors_vector(std::vector<std::string> vector):neighbors(vector) {
        init();
    }

    void neighbors_vector::init() {
        pthread_mutex_init(&neighborsMutex, NULL);
    }

    std::string neighbors_vector::at(int index) {
        std::string neighbor;
        pthread_mutex_lock(&neighborsMutex);
            neighbor = neighbors.at(index);
        pthread_mutex_unlock(&neighborsMutex);
        return neighbor;
    }

    void neighbors_vector::append(std::string neighbor) {
        pthread_mutex_lock(&neighborsMutex);
            neighbors.push_back(neighbor);
        pthread_mutex_unlock(&neighborsMutex);
    }

    bool neighbors_vector::contains(std::string neighbor) {
        bool output;
        pthread_mutex_lock(&neighborsMutex);
            output = std::find(neighbors.begin(), neighbors.end(), neighbor) == neighbors.end();
        pthread_mutex_unlock(&neighborsMutex);
        return output;
    }

    int neighbors_vector::size() {
        int output;
        pthread_mutex_lock(&neighborsMutex);
           output = neighbors.size(); 
        pthread_mutex_unlock(&neighborsMutex);
        return output;
    }

    void neighbors_vector::copy(std::vector<std::string>& vector) {
        pthread_mutex_lock(&neighborsMutex);
            for (std::vector<std::string>::iterator it = neighbors.begin(); it != neighbors.end(); ++it) {
                vector.push_back(*it);
            }
        pthread_mutex_unlock(&neighborsMutex);
    }
}
