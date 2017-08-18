
class NFCMifareWrapper{

// Use of MifareClassic Block
// Value max size (16 bytes)

void formatMifare();
void writeMifareBlock(int _numBlock, string _blockValue);

bool isTagPresent();
string readMifareBlock(int _numBlock);

}
