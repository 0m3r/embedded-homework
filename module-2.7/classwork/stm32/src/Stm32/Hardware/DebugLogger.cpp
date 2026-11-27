#include "Hardware/DebugLogger.hpp"

#include <cstdint>
#include <cstring>

#include "usb_device.h"
#include "usbd_cdc.h"
#include "usbd_cdc_if.h"

extern "C" USBD_HandleTypeDef hUsbDeviceFS;

void DebugLogger::log(const char* message) const
{
    if (count_ >= QueueCapacity) {
        return;
    }

    std::strncpy(queue_[head_], message, MessageSize - 1);
    queue_[head_][MessageSize - 1] = '\0';
    head_ = (head_ + 1) % QueueCapacity;
    ++count_;
}

void DebugLogger::update() const
{
    if (hUsbDeviceFS.dev_state != USBD_STATE_CONFIGURED) {
        transmitting_ = false;
        return;
    }

    auto* cdc = static_cast<USBD_CDC_HandleTypeDef*>(hUsbDeviceFS.pClassData);

    if (transmitting_) {
        if (cdc == nullptr || cdc->TxState != 0) {
            return;
        }

        tail_ = (tail_ + 1) % QueueCapacity;
        --count_;
        transmitting_ = false;
    }

    if (count_ == 0) {
        return;
    }

    const auto length = static_cast<std::uint16_t>(std::strlen(queue_[tail_]));
    if (CDC_Transmit_FS(reinterpret_cast<std::uint8_t*>(queue_[tail_]), length) ==
        USBD_OK) {
        transmitting_ = true;
    }
}
