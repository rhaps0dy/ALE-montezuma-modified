/* *****************************************************************************
 * The lines 62, 115, 125 and 133 are based on Xitari's code, from Google Inc.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License version 2
 * as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 * *****************************************************************************
 * A.L.E (Arcade Learning Environment)
 * Copyright (c) 2009-2013 by Yavar Naddaf, Joel Veness, Marc G. Bellemare and 
 *   the Reinforcement Learning and Artificial Intelligence Laboratory
 * Released under the GNU General Public License; see License.txt for details. 
 *
 * Based on: Stella  --  "An Atari 2600 VCS Emulator"
 * Copyright (c) 1995-2007 by Bradford W. Mott and the Stella team
 *
 * *****************************************************************************
 */
#include "MontezumaRevengeBreadcrumbs.hpp"

#include "../RomUtils.hpp"


const int MontezumaRevengeBreadcrumbsSettings::m_trail[][2] = {
	{0x4d, 0xeb},
	{0x4d, 0xd3},
	{0x4d, 0xc0},
	{0x55, 0xc0},
	{0x68, 0xc0},
	{0x7a, 0xc0},
	{0x85, 0xc0},
	{0x85, 0xa5},
	{0x85, 0x94},
// Ground floor
	{0x85, 0x94},
	{0x7d, 0x94},
	{0x75, 0x94},
	{0x6d, 0x94},
	{0x65, 0x94},
	{0x5d, 0x94},
	{0x55, 0x94},
	{0x4d, 0x94},
	{0x45, 0x94},
	{0x3d, 0x94},
	{0x35, 0x94},
	{0x2d, 0x94},
	{0x25, 0x94},
	{0x1d, 0x94},
	{0x15, 0x94},

	{0x15, 0xa0},
	{0x15, 0xaf},
	{0x15, 0xc0},
	{0x15, 0xaf},
	{0x15, 0xa0},

	{0x15, 0x94},
	{0x1d, 0x94},
	{0x25, 0x94},
	{0x2d, 0x94},
	{0x35, 0x94},
	{0x3d, 0x94},
	{0x45, 0x94},
	{0x4d, 0x94},
	{0x55, 0x94},
	{0x5d, 0x94},
	{0x65, 0x94},
	{0x6d, 0x94},
	{0x75, 0x94},
	{0x7d, 0x94},
	{0x85, 0x94},

	{0x85, 0x94},
	{0x85, 0xa5},
	{0x85, 0xc0},
	{0x7a, 0xc0},
	{0x68, 0xc0},
	{0x55, 0xc0},
	{0x4d, 0xc0},
	{0x4d, 0xd3},
	{0x4d, 0xeb}
};


MontezumaRevengeBreadcrumbsSettings::MontezumaRevengeBreadcrumbsSettings() {
	m_lives = 5;
	m_trail_i = 0;
    reset();
}


/* create a new instance of the rom */
RomSettings* MontezumaRevengeBreadcrumbsSettings::clone() const { 
    
    RomSettings* rval = new MontezumaRevengeBreadcrumbsSettings();
    *rval = *this;
    return rval;
}

#include<iostream>


/* process the latest information from ALE */
void MontezumaRevengeBreadcrumbsSettings::step(const System& system) {

    // update the reward
//    int score = getDecimalScore(0x95, 0x94, 0x93, &system);
	int dx = readRam(&system, 0xAA) - 0x85;
	int dy = readRam(&system, 0xAB) - 0xc0;
    int new_lives = readRam(&system, 0xBA) + 1;

	if(dx*dx + dy*dy/4 <= 10) {
		m_reward = 50;
		m_terminal = true;
	} else if (new_lives < m_lives) {
		m_reward = -300;
		m_terminal = true;
	} else {
		m_reward = 0;
		m_terminal = false;
	}

    // update terminal status
	// Actually does not go up to 8, but that's alright
	//m_lives = (new_lives & 0x7) + 1;
	m_lives = new_lives;
//    m_reward = reward;
//    m_score = score;

//    int some_byte = readRam(&system, 0xFE);
//    m_terminal = m_trail_i == sizeof(m_trail)/sizeof(m_trail[0]) ||
//		(new_lives == 0 && some_byte == 0x60);
}


/* is end of game */
bool MontezumaRevengeBreadcrumbsSettings::isTerminal() const {

    return m_terminal;
};


/* get the most recently observed reward */
reward_t MontezumaRevengeBreadcrumbsSettings::getReward() const { 

    return m_reward; 
}


/* is an action part of the minimal set? */
bool MontezumaRevengeBreadcrumbsSettings::isMinimal(const Action &a) const {

    switch (a) {
        case PLAYER_A_NOOP:
        case PLAYER_A_FIRE:
        case PLAYER_A_UP:
        case PLAYER_A_RIGHT:
        case PLAYER_A_LEFT:
        case PLAYER_A_DOWN:
        case PLAYER_A_UPRIGHT:
        case PLAYER_A_UPLEFT:
        case PLAYER_A_DOWNRIGHT:
        case PLAYER_A_DOWNLEFT:
        case PLAYER_A_UPFIRE:
        case PLAYER_A_RIGHTFIRE:
        case PLAYER_A_LEFTFIRE:
        case PLAYER_A_DOWNFIRE:
        case PLAYER_A_UPRIGHTFIRE:
        case PLAYER_A_UPLEFTFIRE:
        case PLAYER_A_DOWNRIGHTFIRE:
        case PLAYER_A_DOWNLEFTFIRE:
            return true;
        default:
            return false;
    }   
}


/* reset the state of the game */
void MontezumaRevengeBreadcrumbsSettings::reset() {
    
    m_reward   = 0;
    m_score    = 0;
    m_terminal = false;
    m_lives    = 0; // avoid setting terminal at the beginning
}


        
/* saves the state of the rom settings */
void MontezumaRevengeBreadcrumbsSettings::saveState(Serializer & ser) {
  ser.putInt(m_reward);
  ser.putInt(m_score);
  ser.putBool(m_terminal);
  ser.putInt(m_lives);
}

// loads the state of the rom settings
void MontezumaRevengeBreadcrumbsSettings::loadState(Deserializer & ser) {
  m_reward = ser.getInt();
  m_score = ser.getInt();
  m_terminal = ser.getBool();
  m_lives = ser.getInt();
}

