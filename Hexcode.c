/***********************************************************************
 * Class:: CSC-615-01 Spring 2026 
 * Name::  Anthony Yee, Emerson Berido, Joshua Muhammad, 
 *         Joshua `Josh` Juan
 * Student ID:: 921946165, 923160976, 924074746, 923539770
 * Github-Name:: jjuan-SFSU
 * Project:: Assignment 5 - RGB Sensor
 * File:: Hexcode.c
 * 
 * Description:: Functions for converting RGB values to Hex values 
 *               and Color Names
 *               
 **********************************************************************/

#include <stdint.h>
#include "tcs34725_library.h"
#include <stdio.h>
#include "Hexcode.h"

int HEX_VALUES[4] = {
  MIN_HEX_VALUE, MID1_HEX_VALUE, MID2_HEX_VALUE, MAX_HEX_VALUE
};

/*
  Rounds Color Value to 0 or 255
  Rounded values correspond to possible values for predefined colors

  Returns rounded value
*/
uint16_t RoundHexValue(int value) {

  uint16_t closestDiff = MAX_HEX_VALUE;
  uint16_t closestHexValue = 0x0;

  for (int i = 0; i < 4; i++) {
    int diff = (value > HEX_VALUES[i]) ? 
      value - HEX_VALUES[i] : 
      HEX_VALUES[i] - value;

    if (diff < closestDiff) {
      closestDiff = diff;
      closestHexValue = HEX_VALUES[i];
    }
  }

  return closestHexValue;
}

/*
  Takes RGB values, rounds to nearest predefine color,
  and prints the hex value and color name
*/
void PrintColorFromHex(uint16_t red, uint16_t green, uint16_t blue) {
  
  uint16_t roundedRed = RoundHexValue(red);
  uint16_t roundedGreen = RoundHexValue(green);
  uint16_t roundedBlue = RoundHexValue(blue);
  int hexValue = (roundedRed << 16) | (roundedGreen << 8) | roundedBlue;
  
  char* colorName = GetColorNameFromHex(hexValue); //placeholder
  
  printf("Hex: #%02x%02x%02x\t|\t", red, green, blue);
  printf("Color Name: %s\n", colorName);
}

/*
  Takes hex value & checks for matching predefined color

  Returns color name, "N/A Color" if no match
*/
char* GetColorNameFromHex(int hexValue) {
  
  for (int i = 0; i < 5; i++) {
    if (predefinedColors[i].hex == hexValue) {
      return predefinedColors[i].name;
    }
  }

  return "N/A Color"; // no match
}
