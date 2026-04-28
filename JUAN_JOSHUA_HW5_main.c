/***********************************************************************
 * Class:: CSC-615-01 Spring 2026 
 * Name::  Anthony Yee, Emerson Berido, Joshua Muhammad, 
 *         Joshua `Josh` Juan
 * Student ID:: 921946165, 923160976, 924074746, 923539770
 * Github-Name:: jjuan-SFSU
 * Project:: Assignment 5 - RGB Sensor
 * File:: JUAN_JOSHUA_HW5_main.c
 * 
 * Description:: Main file for assignment 5, opens connection to RGB
 *               sensor, initializes it, continuously reads RGB values,
 *               and prints the hex value and color name. 
 *               
 **********************************************************************/

#include "tcs34725_library.h"
#include "Hexcode.h"
#include <pigpio.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>

int handle = -1;

struct colors predefinedColors[27] = {
    {"Black", 0x000000},       {"Navy", 0x000080},
    {"Blue", 0x0000ff},        {"Green", 0x008000},
    {"Teal", 0x008080},        {"Dodger Blue-ish", 0x0080ff},
    {"Lime", 0x00ff00},        {"Spring Green", 0x00ff80},
    {"Cyan", 0x00ffff},        {"Maroon", 0x800000},
    {"Purple", 0x800080},      {"Violet", 0x8000ff},
    {"Olive", 0x808000},       {"Gray", 0x808080},
    {"Light Blue", 0x8080ff},  {"Yellow-Green", 0x80ff00},
    {"Light Green", 0x80ff80}, {"Aquamarine", 0x80ffff},
    {"Red", 0xff0000},         {"Deep Pink-ish", 0xff0080},
    {"Magenta", 0xff00ff},     {"Orange", 0xff8000},
    {"Salmon", 0xff8080},      {"Pink", 0xff80ff},
    {"Yellow", 0xffff00},      {"Light Yellow", 0xffff80},
    {"White", 0xffffff}};

// Helper Functions
void TurnOffRGB(int signal);
uint16_t GetColorNumberFromRGB(int handle, uint8_t reg, uint16_t clearData);

int main() {

  if (gpioInitialise() < 0)
    return 1;
  
  // Starts I2C Connection with RGB Sensor
  handle = OpenRGB();
  if (handle == -1) {
    printf("Failed to open RGB sensor");
    return 1;
  }

  // Initializes RGB sensor
  if (InitializeRGB(handle, 0x00, 3) != 0) {
    printf("Failed to open RGB sensor");
    TurnOffRGB(-1);
    return 1;
  }

  // Continuously reads & prints RGB values
  signal(SIGINT, TurnOffRGB);
  while (1) {
    uint16_t clearData = ReadDataRegisterRGB(handle, CDATAL);

    uint16_t redData = GetColorNumberFromRGB(handle, RDATAL, clearData);
    uint16_t blueData = GetColorNumberFromRGB(handle, BDATAL, clearData);
    uint16_t greenData = GetColorNumberFromRGB(handle, GDATAL, clearData);

    PrintColorFromHex(redData, greenData, blueData);
  }

  TurnOffRGB(-1);
  return 0;
}

// Helper function, Catches Ctrl C
void TurnOffRGB(int signal) {

  if (handle == -1) {
    if (CloseRGB(handle) != 0) {
      printf("Failed to close RGB sensor");
    }
  }

  gpioTerminate();
  exit(0);
}

/*
  Helper function

  Takes in color register and reads data
  Removes any distance or brightness

  Returns numeric color value in range [0, 255]
*/
uint16_t GetColorNumberFromRGB(int handle, uint8_t reg, uint16_t clearData) 
{
  if (reg < RDATAL || reg > BDATAL) {
    printf("Register is not color data register\n");
    return 0;
  }

  // Removes any distance or brightness from reading
  uint16_t colorData = ReadDataRegisterRGB(handle, reg);
  float colorPercentage = (float)colorData / clearData;

  return (uint16_t)(colorPercentage * 255);
}
