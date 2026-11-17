#include "DebugLogger.hpp"

#include <cstdint>
#include <cstring>

#include "usbd_cdc_if.h"

void DebugLogger::log(const char* message) const
{
    CDC_Transmit_FS(reinterpret_cast<std::uint8_t*>(const_cast<char*>(message)),
                    static_cast<std::uint16_t>(std::strlen(message)));
}
