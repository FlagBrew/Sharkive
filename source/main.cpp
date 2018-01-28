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

#include "common.h"

static Gui* menu;

void createInfo(std::string title, std::string message)
{
	menu->createInfo(title, message);
}

void createError(Result res, std::string message)
{
	menu->createError(res, message);
}

int main() {
	servicesInit();
	menu = new Gui();
	createThread((ThreadFunc)threadLoadTitles);

	while (aptMainLoop() && !(hidKeysDown() & KEY_START)) {
		hidScanInput();
		
		if (hidKeysDown() & KEY_A)
		{
			Title title;
			getTitle(title, menu->getNormalizedIndex());
			if (title.availableOnDB)
			{
				u32 size = 0;
				Result res = httpDownloadFile(title.url, title.codePath, &size);
				if (R_SUCCEEDED(res))
				{
					setAvailableOnSD(menu->getNormalizedIndex(), size);
					createInfo("Success!", "Codes downloaded successfully.");
				}
			}
		}
		
		menu->updateSelector();
		menu->draw();
	}
	
	delete menu;
	destroyThreads();
	servicesExit();
}