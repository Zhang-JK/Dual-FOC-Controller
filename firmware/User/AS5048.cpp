#include "AS5048.h"

AS5048::AS5048(SPI_HandleTypeDef *spi) {
    this->spi = spi;
    this->zeroPosition = 0;
    this->position = 0;
    this->receiveTimestamp = 0;
    this->connectionLoss = true;
}

AS5048::~AS5048() {

}

void AS5048::init() {
    HAL_SPI_Init(spi);
}

uint16_t AS5048::read(uint16_t address) {
    uint16_t data = 0;
    bool parity = false;
    for (int i = 0; i < 13; i++) {
        if (address & (1 << i)) {
            parity = !parity;
        }
    }
    uint16_t command = (parity ? 0xC000 : 0x4000) | (address & 0x3FFF);
    // uint16_t command = 0x4000 | (address & 0x3FFF);
    // HAL_SPI_TransmitReceive(spi, (uint8_t *)&command, (uint8_t *)&data, 1, 100);
    static volatile HAL_StatusTypeDef succ1 = HAL_OK, succ2 = HAL_OK, succ3 = HAL_OK;
    succ1 = HAL_SPI_Transmit(spi, (uint8_t *)&command, 1, 100);
    succ2 = HAL_SPI_Transmit(spi, (uint8_t *)&command, 1, 100);
    succ3 = HAL_SPI_Receive(spi, (uint8_t *)&data, 1, 100);
    return data;
}

// void AS5048::write(uint16_t address, uint16_t data) {
//     bool parityA = true;
//     bool parityD = true;
//     for (int i = 0; i < 13; i++) {
//         if (address & (1 << i)) {
//             parityA = !parityA;
//         }
//         if (data & (1 << i)) {
//             parityD = !parityD;
//         }
//     }
//     uint16_t command = (parityA ? 0x8000 : 0x0000) | (address & 0x3FFF);
//     uint16_t commandData = (parityD ? 0x8000 : 0x0000) | (data & 0x3FFF);
//     HAL_SPI_Transmit(spi, (uint8_t *)&command, 1, 100);
//     HAL_SPI_Transmit(spi, (uint8_t *)&commandData, 1, 100);
// }

void AS5048::update() {
    uint16_t data = read(AS5048A_ANGLE);
    if (data & 0x4000) {
        connectionLoss = true;
    } else {
        connectionLoss = false;
        position = data & 0x3FFF;
        receiveTimestamp = HAL_GetTick();
    }
}

uint16_t AS5048::getPosition() {
    return position - zeroPosition;
}

uint16_t AS5048::getZeroPosition() {
    return zeroPosition;
}

bool AS5048::setCurrentZeroPosition() {
    if (!connectionLoss){
        zeroPosition = position;
        return true;
    } else {
        return false;
    }
}
