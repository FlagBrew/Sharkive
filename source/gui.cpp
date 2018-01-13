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

#include "gui.h"

static Info info;
static MessageBox* messageBox;

static void drawBasicInterface(gfxScreen_t screen);

/// Multi selection

static std::vector<size_t> selectedEntries;

std::vector<size_t> getSelectedEntries(void)
{
	return selectedEntries;
}

bool multipleSelectionEnabled(void)
{
	return !selectedEntries.empty();
}

void clearSelectedEntries(void)
{
	selectedEntries.clear();
}

void addSelectedEntry(size_t index)
{
	int existing = -1;
	for (size_t i = 0, sz = selectedEntries.size(); i < sz && existing == -1; i++)
	{
		if (selectedEntries.at(i) == index)
		{
			existing = (int)i;
		}
	}
	
	if (existing == -1)
	{
		selectedEntries.push_back(index);
	}
	else
	{
		selectedEntries.erase(selectedEntries.begin() + existing);
	}
}

/// Gui implementation

static void drawBasicInterface(gfxScreen_t screen)
{
	if (screen == GFX_TOP)
	{
		pp2d_draw_rectangle(0, 0, 400, 19, COLOR_BARS);
		pp2d_draw_rectangle(0, 221, 400, 19, COLOR_BARS);
		
		char version[10];
		sprintf(version, "v%d.%d.%d", VERSION_MAJOR, VERSION_MINOR, VERSION_MICRO);
		static float versionLen = pp2d_get_text_width(version, 0.45f, 0.45f);
		static float namelen = pp2d_get_text_width("Sharkive", 0.50f, 0.50f);

		pp2d_draw_text(4, 3, 0.45f, 0.45f, GREYISH, getTime().c_str());
		pp2d_draw_text(TOP_WIDTH - 4 - versionLen, 3, 0.45f, 0.45f, GREYISH, version);
		pp2d_draw_texture(TEXTURE_ICON, TOP_WIDTH - 5 - versionLen - 19, 0);
		pp2d_draw_text(TOP_WIDTH - 6 - versionLen - namelen - 19, 2, 0.50f, 0.50f, WHITE, "Sharkive");		
	}
	else
	{
		pp2d_draw_rectangle(0, 0, 320, 19, COLOR_BARS);
		pp2d_draw_rectangle(0, 221, 320, 19, COLOR_BARS);		
	}
}

Gui::Gui(void)
{
	index = 0;
	page = 0;
	info.init("", "", 0, TYPE_INFO);
	messageBox = new MessageBox(COLOR_BARS, WHITE, GFX_TOP);
	messageBox->push_message("Press \uE000 to download target.");
	messageBox->push_message("Press \uE006 to move between titles.");
	messageBox->push_message("Press \uE004\uE005 to switch page.");
}

void Gui::createInfo(std::string title, std::string message)
{
	info.init(title, message, 500, TYPE_INFO);
}

void Gui::createError(Result res, std::string message)
{
	info.init(res, message, 500, TYPE_ERROR);
}

size_t Gui::getNormalizedIndex(void)
{
	return index + page*rowlen*collen;
}

void Gui::updateSelector(void)
{
	const size_t entries = rowlen * collen;
	const size_t maxentries = (getTitlesCount() - page*entries) > entries ? entries : getTitlesCount() - page*entries;
	const size_t maxpages = getTitlesCount() / entries + 1;
	calculateIndex(index, page, maxpages, maxentries, entries, collen);
}

void Gui::drawSelector(void)
{
	static const int w = 2;
	const int x = getSelectorX(index);
	const int y = getSelectorY(index);
	pp2d_draw_rectangle(         x,          y, 50,       50, RGBA8(255, 255, 255, 200)); 
	pp2d_draw_rectangle(         x,          y, 50,    w + 1, RED); //top
	pp2d_draw_rectangle(         x,      y + w,  w, 50 - 2*w, RED); //left
	pp2d_draw_rectangle(x + 50 - w,      y + w,  w, 50 - 2*w, RED); //right
	pp2d_draw_rectangle(         x, y + 50 - w, 50,        w, RED); //bottom
}

int Gui::getSelectorX(size_t index)
{
	return 50*((index % (rowlen*collen)) % collen);
}

int Gui::getSelectorY(size_t index)
{
	return 20 + 50*((index % (rowlen*collen)) / collen);
}

void Gui::draw(void)
{
	const size_t entries = rowlen * collen;
	const size_t max = (getTitlesCount() - page*entries) > entries ? entries : getTitlesCount() - page*entries;
	
	pp2d_begin_draw(GFX_TOP, GFX_LEFT);
		drawBasicInterface(GFX_TOP);
		
		for (size_t k = page*entries; k < page*entries + max; k++)
		{
			pp2d_draw_texture(getTextureId(k), getSelectorX(k) + 1, getSelectorY(k) + 1);
			if (!selectedEntries.empty() && std::find(selectedEntries.begin(), selectedEntries.end(), k) != selectedEntries.end())
			{
				pp2d_draw_rectangle(getSelectorX(k) + 31, getSelectorY(k) + 31, 16, 16, WHITE);
				pp2d_draw_texture_blend(TEXTURE_CHECKBOX, getSelectorX(k) + 27, getSelectorY(k) + 27, COLOR_BARS);
			}
		}
		
		if (getTitlesCount() > 0)
		{
			drawSelector();
		}
		
		pp2d_draw_text_center(GFX_TOP, 223, 0.47f, 0.47f, WHITE, "Hold SELECT to see commands.");
		
		info.draw();
		
		if (hidKeysHeld() & KEY_SELECT)
		{
			messageBox->draw();
		}
		
		pp2d_draw_on(GFX_BOTTOM, GFX_LEFT);
		drawBasicInterface(GFX_BOTTOM);
		
		if (getTitlesCount() > 0)
		{
			Title title;
			getTitle(title, index + page*entries);
			
			pp2d_draw_text(4, 0, 0.6f, 0.6f, WHITE, title.getShortDescription().c_str());
			pp2d_draw_text_wrap(4, 27, 0.55f, 0.55f, GREYISH, 240, title.getLongDescription().c_str());
			
			float longDescrHeight = pp2d_get_text_height_wrap(title.getLongDescription().c_str(), 0.55f, 0.55f, 240);
			pp2d_draw_text(4, 31 + longDescrHeight, 0.5f, 0.5f, GREYISH, "ID:");
			
			char lowid[9];
			snprintf(lowid, 9, "%08X", (int)title.getLowId());
			pp2d_draw_text(25, 31 + longDescrHeight, 0.5f, 0.5f, WHITE, lowid);
			pp2d_draw_textf(30 + pp2d_get_text_width(lowid, 0.5f, 0.5f), 32 + longDescrHeight, 0.42f, 0.42f, GREYISH, "(%s)", title.productCode);
			pp2d_draw_text(4, 47 + longDescrHeight, 0.5f, 0.5f, GREYISH, "Mediatype:");
			pp2d_draw_textf(75, 47 + longDescrHeight, 0.5f, 0.5f, WHITE, "%s", title.getMediatypeString().c_str());

			pp2d_draw_rectangle(260, 27, 52, 52, BLACK);
			pp2d_draw_texture(title.textureId, 262, 29);
			
			pp2d_draw_rectangle(28, 106, 264, 102, WHITE);
			pp2d_draw_rectangle(30, 108, 260, 98, PURPLE);
			pp2d_draw_rectangle(28, 156, 264, 2, WHITE);
			
			if (title.availableOnSD)
			{
				pp2d_draw_text_center(GFX_BOTTOM, 123, 0.6f, 0.6f, WHITE, "Codes available on SD");
			}
			else
			{
				pp2d_draw_text_center(GFX_BOTTOM, 123, 0.6f, 0.6f, GREYISH, "Codes not available on SD");
			}
			
			if (title.availableOnSD && !title.availableOnDB)
			{
				pp2d_draw_text_center(GFX_BOTTOM, 162, 0.6f, 0.6f, WHITE, "Codes not available in DB");
				pp2d_draw_text_center(GFX_BOTTOM, 182, 0.6f, 0.6f, WHITE, "Would you mind to contribute? \uE008");
			}
			else if (title.availableOnDB)
			{
				pp2d_draw_text_center(GFX_BOTTOM, 172, 0.6f, 0.6f, WHITE, "Codes available on DB.");
			}
			else
			{
				pp2d_draw_text_center(GFX_BOTTOM, 172, 0.6f, 0.6f, GREYISH, "Codes not available on DB.");
			}
		}
		
		pp2d_draw_text_center(GFX_BOTTOM, 223, 0.46f, 0.46f, WHITE, "Press \uE073 or START to exit.");
	pp2d_end_draw();
}

void Gui::resetIndex(void)
{
	index = 0;
	page = 0;
}