/***********************************************************************
 * Class:: CSC-615-01 Spring 2026 
 * Name::  Anthony Yee, Emerson Berido, Joshua Muhammad, 
 *         Joshua `Josh` Juan
 * Student ID:: 921946165, 923160976, 924074746, 923539770
 * Github-Name:: jjuan-SFSU
 * Project:: Assignment 5 - RGB Sensor
 * File:: tcs34725_library.h
 * 
 * Description:: Header file for RGB.c, containing functions for opening 
 *               connection to RGB sensor, initializing it, reading RGB 
 *               values, and closing connection. Contains TCS34725 
 *               register addresses and command register's fields.
 *               
 **********************************************************************/

#ifndef RGB_H
#define RGB_H

#include <stdint.h>

#define RGB_ADDR 0x29 // Find this later

// TCS34725 Registers
#define ENABLE_REG 0x00
#define ATIME_REG 0x01
#define WTIME_REG 0x03
#define AILTL_REG 0x04
#define AILTH_REG 0x05
#define AIHTL_REG 0x06
#define AIHTH_REG 0x07
#define PERS_REG 0x0C
#define CONFIG_REG 0x0D
#define CONTROL_REG 0x0F
#define ID_REG 0x12
#define STATUS_REG 0x13
#define CDATAL 0x14
#define CDATAH 0x15
#define RDATAL 0x16
#define RDATAH 0x17
#define GDATAL 0x18
#define GDATAH 0x19
#define BDATAL 0x1A
#define BDATAH 0x1B

// Command Fields
#define COMMAND_BYTE 0x80
#define COMMAND_AUTO_INCREMENT 0x20

// Enable Register Defines
#define PON 0x01 // powers on the RGB sensor
#define AEN 0x02 // enables RGBC

// Main Functions
int OpenRGB(); 
int InitializeRGB(int handle, uint8_t integration, uint8_t gain);
uint16_t ReadDataRegisterRGB(int handle, uint8_t register);
int CloseRGB(int handle);

// I2C Wrappers
int RGBWriteByte(int handle, int reg, uint8_t value);
uint16_t RGBReadBytes(int handle, int reg, int amount);

struct colors {
  char *name;
  int hex;
};

extern struct colors predefinedColors[5];

#endif
