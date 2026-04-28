/***********************************************************************
 * Class:: CSC-615-01 Spring 2026 
 * Name::  Anthony Yee, Emerson Berido, Joshua Muhammad, 
 *         Joshua `Josh` Juan
 * Student ID:: 921946165, 923160976, 924074746, 923539770
 * Github-Name:: jjuan-SFSU
 * Project:: Assignment 5 - RGB Sensor
 * File:: Hexcode.h
 * 
 * Description:: Header file for Hexcode.c, containing functions for 
 *               converting RGB values to Hex values and Color Names
 *               
 **********************************************************************/

#ifndef HEXCODE_H
#define HEXCODE_H

#define MAX_HEX_VALUE 0xFF
#define MIDDLE_HEX_VALUE 0x80
#define MIN_HEX_VALUE 0x00

#include <stdint.h>

uint16_t RoundHexValue(int value);
void PrintColorFromHex(uint16_t red, uint16_t green, uint16_t blue);
char* GetColorNameFromHex(int hexValue);

#endif