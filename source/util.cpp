/*  This file is part of Sharkive
>	Copyright (C) 2018 Bernardo Giordano
>
>   This program is free software: you can redistribute it and/or modify
>   it under the terms of the GNU General Public License as published by
>   the Free Software Foundation, either version 3 of the License, or
>   (at your option) any later version.
>
>   This program is distributed in the hope that it will be useful,
>   but WITHOUT ANY WARRANTY; without even the implied warranty of
>   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
>   GNU General Public License for more details.
>
>   You should have received a copy of the GNU General Public License
>   along with this program.  If not, see <http://www.gnu.org/licenses/>.
>   See LICENSE for information.
*/

#include "util.h"

void *socubuf;

void servicesExit(void)
{
	socExit();
	free(socubuf);
	archiveExit();
	amExit();
	srvExit();
	hidExit();
	pp2d_exit();
	sdmcExit();
	romfsExit();
}

void servicesInit(void)
{
	Result res = 0;
	romfsInit();
	sdmcInit();
	pp2d_init();
	hidInit();
	srvInit();
	amInit();
	
	res = archiveInit();
	if (R_FAILED(res))
	{
		createError(res, "SDMC archive init failed.");
	}
	
	mkdir("sdmc:/3ds", 777);
	mkdir("sdmc:/3ds/Sharkive", 777);
	mkdir("sdmc:/luma", 777);
	mkdir("sdmc:/luma/titles", 777);
	
	pp2d_set_screen_color(GFX_TOP, COLOR_BACKGROUND);
	pp2d_set_screen_color(GFX_BOTTOM, COLOR_BACKGROUND);
	pp2d_load_texture_png(TEXTURE_CHECKBOX, "romfs:/checkbox.png");
	pp2d_load_texture_png(TEXTURE_ICON, "romfs:/shark.png");
	pp2d_load_texture_png(TEXTURE_NOICON, "romfs:/noicon.png");

	socubuf = memalign(0x1000, 0x100000);
	socInit((u32*)socubuf, 0x100000);
}

void calculateTitleDBHash(u8* hash)
{
	u32 titleCount, titlesRead;
	AM_GetTitleCount(MEDIATYPE_SD, &titleCount);
	u64 buf[titleCount + 1];
	AM_GetTitleList(&titlesRead, MEDIATYPE_SD, titleCount, buf);
	
	u64 cardID;
	FS_CardType cardType;
	Result res = FSUSER_GetCardType(&cardType);
	if (R_FAILED(res) || !(cardType == CARD_CTR))
	{
		buf[titleCount] = 0xFFFFFFFFFFFFFFFF;
	}
	else
	{
		AM_GetTitleList(NULL, MEDIATYPE_GAME_CARD, 1, &cardID);
		buf[titleCount] = cardID;
	}
	
	sha256(hash, (u8*)buf, (titleCount + 1) * sizeof(u64));
}

std::string getTime(void)
{
	char buf[80] = {0};
	time_t unixTime = time(NULL);
	struct tm* timeStruct = gmtime((const time_t*)&unixTime);
	
	sprintf(buf, "%02i:%02i:%02i", timeStruct->tm_hour, timeStruct->tm_min, timeStruct->tm_sec);
	
	std::string ret(buf);
	return ret;
}