/*
** EPITECH PROJECT, 2022
** game_controler
** File description:
** joystick.h
*/
#ifndef GAME_CONTROLER_JOYSTICK_H
#define GAME_CONTROLER_JOYSTICK_H

#include <stdbool.h>
#include <stdint.h>

#define ERROR_CODE 84
#define TO_200_RANGE 325

typedef struct joystick_s {
	bool button[12];
	int16_t joystick[8];
} joystick_t;

joystick_t *get_joystick(void);
int start_reading(void);
int stop_reading(void);
void display_controler_data(void);

#endif //GAME_CONTROLER_JOYSTICK_H
