#ifndef FILE_STATE_H
#define FILE_STATE_H

#include "FlatState.h"

class FileState : public FlatState {

public:

    /* Default Constructor */
    FileState() {}

    /* Destructor */
    ~FileState() {}

    /* Get the state size */
    const int getSize() const { return sizeof(FileState); }
};

#endif
