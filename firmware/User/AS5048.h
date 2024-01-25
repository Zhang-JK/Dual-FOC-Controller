#include "spi.h"

#define AS5048A_CLEAR_ERROR_FLAG                0x0001
#define AS5048A_PROGRAMMING_CONTROL             0x0003
#define AS5048A_OTP_REGISTER_ZERO_POS_HIGH      0x0016
#define AS5048A_OTP_REGISTER_ZERO_POS_LOW       0x0017
#define AS5048A_DIAG_AGC                        0x3FFD
#define AS5048A_MAGNITUDE                       0x3FFE
#define AS5048A_ANGLE                           0x3FFF

class AS5048
{
private:
    void init();
    uint16_t read(uint16_t address);
    // void write(uint16_t address, uint16_t data);

    SPI_HandleTypeDef *spi;
    uint16_t zeroPosition;
    uint16_t position;
    uint32_t receiveTimestamp;
    bool connectionLoss;
    
public:
    AS5048(SPI_HandleTypeDef *spi);
    ~AS5048();

    void update();
    uint16_t getPosition();
    uint16_t getZeroPosition();
    bool setCurrentZeroPosition();
};
