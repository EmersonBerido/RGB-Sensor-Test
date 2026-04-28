/***********************************************************************
 * Class:: CSC-615-01 Spring 2026 
 * Name::  Anthony Yee, Emerson Berido, Joshua Muhammad, 
 *         Joshua `Josh` Juan
 * Student ID:: 921946165, 923160976, 924074746, 923539770
 * Github-Name:: jjuan-SFSU
 * Project:: Assignment 5 - RGB Sensor
 * File:: RGB.c
 * 
 * Description:: Functions for opening connection to RGB sensor, 
 *               initializing it, reading RGB values, and closing 
 *               connection. Uses the I2C functions in the pigpio 
 *               library to create custom I2C wrapper functions.
 *               
 **********************************************************************/

#include "tcs34725_library.h"
#include <pigpio.h>
#include <stdint.h>
#include <stdio.h>
#include <unistd.h>

/*
  Initializes I2C Connection
  
  Returns handle
  Handle < 0 error, else success
*/
int OpenRGB() {
  int handle = i2cOpen(1, RGB_ADDR, 0);

  return (handle < 0 ? -1 : handle);
}

/**
 * Powers on and enables Sensor
 * Waits required minimum of 2.4 ms after powering on
 *
 * Parameters:
 *  integration - time to read data, [0xFF shortest, 0x00 longest]
 *  gain - affects white level, [0 is lowest, 3 is highest]
 * 
 * Returns 0 on success, -1 on failure
 */
int InitializeRGB(int handle, uint8_t integration, uint8_t gain) {

  // Enable register: Powers on & RGBC enabled
  if (RGBWriteByte(handle, ENABLE_REG, 0x01) == -1)
    return -1;  
  usleep(2400); 
  if (RGBWriteByte(handle, ENABLE_REG, 0x03 | 0x01) == -1)
    return -1; 

  // ATIME Register
  if (RGBWriteByte(handle, ATIME_REG, integration) == -1)
    return -1; 

  // Control Register
  if (gain > 3)
    gain = 3; 
  if (RGBWriteByte(handle, CONTROL_REG, gain) == -1)
    return -1;

  return 0;
}

/*
  Two-Byte read I2C transaction
  Reads lower register first for copy in shadow register
  Register: 0x14 -> 0x1B

  Returns 16 bit R/G/B/C value
*/
uint16_t ReadDataRegisterRGB(int handle, uint8_t reg) {

  // If reg not in range or passed in high reg
  if (reg < 0x14 || reg > 0x1B)
    return 0;

  // Sets reg to low address
  if (reg % 2 == 1)
    reg = reg - 1;

  uint16_t data = RGBReadBytes(handle, reg, 2);
  return data;
}

/*
  Closes I2C Connection with handle

  returns 0 on success
*/
int CloseRGB(int handle) { return i2cClose(handle); }


// I2C Wrapper Helper Functions

/**
 * Writes 1 Command Byte to establish register address to write to
 * Writes 1 Byte to a register with write access
 * 
 * Returns 0 on success, -1 on failure
 */
int RGBWriteByte(int handle, int reg, uint8_t value) {

  // Out of bounds or read-only register
  if (reg < 0x00 || reg > 0x0F) {
    printf("Register %d is out of bounds\n", reg);
    return 0;
  }

  uint8_t command = COMMAND_BYTE | (uint8_t)reg;

  uint8_t data[2];
  data[0] = command;
  data[1] = value;

  if (i2cWriteDevice(handle, (char *)data, 2) != 0) {
    printf("Failed to Write to device\n");
    return -1; // Sets read register access
  }

  return 0;
}

/*
  Reads 1-2 Bytes from the RGB Sensor
  Writes a command byte with optional auto-increment protocol
  Command byte sets which register to read from
  Auto increment allows reading for multiple bytes in a row
  Reads total of amount bytes from device
  Maximum bits to read in RGB register is 16; 2 Bytes

  Returns 0 -> 65536, 0 on error
*/
uint16_t RGBReadBytes(int handle, int reg, int amount) {

  if (reg < 0x00 || reg > 0x1B) {
    printf("Register out of bounds\n");
    return 0;
  }
  if (amount < 1 || amount > 2) {
    printf("Requested read amount out of bounds\n");
    return 0;
  }

  // create command byte
  uint8_t command = COMMAND_BYTE | (uint8_t)reg;
  if (amount > 1)
    command |= COMMAND_AUTO_INCREMENT;

  // Write to establish a register address to read from
  if (i2cWriteDevice(handle, (char *)&command, 1) != 0) {
    printf("Failed to Write to device\n");
    return 0; 
  }

  // Create a byte array (size is amount)
  uint8_t data[amount];

  // Read Device of count amount
  if (i2cReadDevice(handle, (char *)data, amount) <= 0) {
    printf("Failed to read data in register %d\n", reg);
    return 0;
  }

  // if amount is 1, return byte[0]
  if (amount == 1)
    return (uint16_t)data[0];

  uint16_t wholeData = ((uint16_t)data[1] << 8) | (uint8_t)data[0];
  return wholeData;
}