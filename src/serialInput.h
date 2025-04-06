#pragma once
#include "processors/processor.h"

class SerialInput {
private:
    
    static const byte numChars = 32;
    char m_receivedChars[numChars];
    byte m_ndx = 0; 
    bool m_verbose = true;
    bool recvWithEndMarker();

public:
    SerialInput(); 
    PIDParameters processInput(PIDParameters params);
    bool getVerbose();
};