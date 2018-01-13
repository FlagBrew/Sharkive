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

#ifndef TITLE_H
#define TITLE_H

#include "common.h"

class Title
{
public:
	void load(void);
	bool load(u64 id, FS_MediaType mediaType, FS_CardType cardType);
	
	std::string getMediatypeString(void);
	std::string getShortDescription(void);
	std::string getLongDescription(void);
	
	u32 getHighId(void);
	u32 getLowId(void);
	u32 getUniqueId(void);
	u32 getExtdataId(void);
	
	char productCode[16];
	bool availableOnSD;
	bool availableOnDB;
	std::string dbName;
	std::string url;
	std::u16string codePath;
	
	u64 id;
	FS_MediaType media;
	FS_CardType card;
	size_t textureId;
	
private:
	std::u16string shortDescription;
	std::u16string longDescription;
};

void getTitle(Title &dst, int i);
int getTitlesCount(void);
size_t getTextureId(int i);

void loadTitles(bool forceRefresh);
void setAvailableOnDB(std::string name, std::string url);
void setAvailableOnSD(size_t index);

#endif