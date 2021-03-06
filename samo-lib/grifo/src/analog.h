/*
 * analog - driver for the analog inputs
 *
 * Copyright (c) 2009 Openmoko Inc.
 *
 * Authors   Christopher Hall <hsw@openmoko.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#if  !defined(_ANALOG_H_)
#define _ANALOG_H_ 1

typedef enum {
//+MakeSystemCalls: channels
	ANALOG_BATTERY_MILLIVOLTS,
	ANALOG_LCD_MILLIVOLTS,
	ANALOG_TEMPERATURE_CELCIUS,
	ANALOG_TEMPERATURE_CENTI_CELCIUS,
//-MakeSystemCalls: channels
} Analog_ChannelType;


void Analog_initialise(void);
long Analog_read(Analog_ChannelType channel);

#endif
