/*
 * SPIDriver.cpp --- AP_HAL_VRBRAIN SPI driver.
 *
 * Copyright (C) 2013, Virtualrobotix.com Roberto Navoni , Emile 
 * All Rights Reserved.
 *
 * This software is released under the "BSD3" license.  Read the file
 * "LICENSE" for more information.
 */

#include "SPIDriver.h"
//#include <FreeRTOS.h>
#include <wirish.h>

using namespace VRBRAIN;

extern const AP_HAL::HAL& hal;

//////////////////////////////////////////////////////////////////////
// SPI Device Driver

VRBRAINSPIDeviceDriver::VRBRAINSPIDeviceDriver(spi_bus *bus, spi_device *device)
  : _bus(bus), _device(device)
{
}

void VRBRAINSPIDeviceDriver::init()
{
  _semaphore.init();
}

AP_HAL::Semaphore* VRBRAINSPIDeviceDriver::get_semaphore()
{
  return &_semaphore;
}

void VRBRAINSPIDeviceDriver::transaction(const uint8_t *tx, uint8_t *rx, uint16_t len)
{
  if (spi_transfer(_bus, _device, 1000, tx, rx, len) < 0) {
    hal.scheduler->panic("PANIC: SPI transaction timeout.");
  }
}

// XXX these methods are not implemented
void VRBRAINSPIDeviceDriver::cs_assert()
{
}

void VRBRAINSPIDeviceDriver::cs_release()
{
}

uint8_t VRBRAINSPIDeviceDriver::transfer (uint8_t data)
{
  return 0;
}

//////////////////////////////////////////////////////////////////////
// SPI Device Instances

// SPIDevice_Dataflash (I2C device in PX4FMU)

// SPIDevice_ADS7844 (not present in PX4FMU)

// SPIDevice_MS5611 (I2C device in PX4FMU)

// SPIDevice_MPU6000 (on SPI1)
static spi_device g_mpu6000_spi_dev = {
  pin_b0,                       // chip_select
  false,                        // chip_select_active
  SPI_BAUD_DIV_128,             // baud XXX check frequency
  SPI_CLOCK_POLARITY_LOW,       // clock_polarity
  SPI_CLOCK_PHASE_1,            // clock_phase
  SPI_BIT_ORDER_MSB_FIRST       // bit_order
};

static VRBRAINSPIDeviceDriver g_mpu6000_dev(spi1, &g_mpu6000_spi_dev);

// SPIDevice_ADNS3080_SPI0 (not present in PX4FMU)

// SPIDevice_ADNS3080_SPI3 (not present in PX4FMU)

//////////////////////////////////////////////////////////////////////
// SPI Device Manager

VRBRAINSPIDeviceManager::VRBRAINSPIDeviceManager()
{
}

// Initialize all SPI busses and devices.
void VRBRAINSPIDeviceManager::init(void *)
{
  spi_init(spi1);

  spi_device_init(&g_mpu6000_spi_dev);
  g_mpu6000_dev.init();
}

AP_HAL::SPIDeviceDriver* VRBRAINSPIDeviceManager::device(AP_HAL::SPIDevice dev)
{
  switch (dev) {
    case AP_HAL::SPIDevice_MPU6000:
      return &g_mpu6000_dev;

    default:
      return NULL;
  }
}
