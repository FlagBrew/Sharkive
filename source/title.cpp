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

#include "title.h"

static std::vector<Title> titles;

static bool checkHigh(u64 id);
static void loadTextureIcon(smdh_s *smdh, size_t i);

static void exportTitleListCache(void);
static void importTitleListCache(void);

void Title::load(void)
{
	id = 0xFFFFFFFFFFFFFFFF;
	media = MEDIATYPE_SD;
	card = CARD_CTR;
	memset(productCode, 0, 16);
	shortDescription = u8tou16(" ");
	longDescription = u8tou16(" ");
	textureId = TEXTURE_NOICON;
	availableOnSD = false;
	availableOnDB = false;
	dbSize = 0;
	sdSize = 0;
}

bool Title::load(u64 _id, FS_MediaType _media, FS_CardType _card)
{
	bool loadTitle = false;
	static size_t index = TEXTURE_FIRST_FREE_INDEX;
	id = _id;
	media = _media;
	card = _card;
	
	availableOnSD = false;
	availableOnDB = false;

	smdh_s *smdh = loadSMDH(getLowId(), getHighId(), media);
	if (smdh == NULL)
	{
		return false;
	}
	
	char unique[12] = {0};
	sprintf(unique, "0x%05X ", (unsigned int)getUniqueId());
	
	shortDescription = (char16_t*)smdh->applicationTitles[1].shortDescription;
	longDescription = (char16_t*)smdh->applicationTitles[1].longDescription;
	AM_GetTitleProductCode(media, id, productCode);
	
	dbName = string_format("%016llX.txt", id);
	std::u16string folderPath = u8tou16("/luma/titles/") + u8tou16(string_format("%016llX", id).c_str());
	codePath = folderPath + u8tou16("/cheats.txt");

	// having 2 separate checks let us perform just 
	// one IO bound operation rather than 2
	if (isSaveAccessible(media, getLowId(), getHighId()))
	{
		loadTitle = true;
	}
	else if (isExtdataAccessible(getExtdataId()))
	{
		loadTitle = true;
	}
	
	if (loadTitle)
	{
		if (!directoryExist(getArchiveSDMC(), folderPath))
		{
			Result res = createDirectory(getArchiveSDMC(), folderPath);
			if (R_FAILED(res))
			{
				createError(res, "Failed to create backup directory.");
			}
		}
		else
		{
			FSStream stream(getArchiveSDMC(), codePath, FS_OPEN_READ);
			stream.close();
			availableOnSD = stream.getLoaded();
			sdSize = stream.getSize();
		}

		loadTextureIcon(smdh, index);
		textureId = index;
		index++;
	}
	
	delete smdh;
	return loadTitle;
}

std::string Title::getMediatypeString(void)
{
	switch(media)
	{
		case MEDIATYPE_SD: return "SD Card";
		case MEDIATYPE_GAME_CARD: return "Cartridge";
		case MEDIATYPE_NAND: return "NAND";
		default: return " ";
	}
	
	return " ";
}

std::string Title::getShortDescription(void)
{
	return u16tou8(shortDescription);
}

std::string Title::getLongDescription(void)
{
	return u16tou8(longDescription);
}

u32 Title::getHighId(void)
{
	return (u32)(id >> 32);
}

u32 Title::getLowId(void)
{
	return (u32)id;
}

u32 Title::getUniqueId(void)
{
	return (getLowId() >> 8);
}

u32 Title::getExtdataId(void)
{
	u32 low = getLowId();
	switch(low)
	{
		case 0x00055E00: return 0x0000055D; // Pokémon Y
		case 0x0011C400: return 0x000011C5; // Pokémon Omega Ruby
		case 0x00175E00: return 0x00001648; // Pokémon Moon
		case 0x00179600:
		case 0x00179800: return 0x00001794; // Fire Emblem Conquest SE NA
		case 0x00179700:
		case 0x0017A800: return 0x00001795; // Fire Emblem Conquest SE EU
		case 0x0012DD00:
		case 0x0012DE00: return 0x000012DC; // Fire Emblem If JP
		case 0x001B5100: return 0x001B5000; // Pokémon Ultramoon
	}
	
	return low >> 8;
}

static bool checkHigh(u64 id)
{
	u32 high = id >> 32;
	return (high == 0x00040000 || high == 0x00040002);
}

void loadTitles(bool forceRefresh)
{
	std::u16string savecachePath = u8tou16("/3ds/Sharkive/cache");
	
	// on refreshing
	titles.clear();

	bool optimizedLoad = false;
	
	u8 hash[SHA256_BLOCK_SIZE];
	calculateTitleDBHash(hash);

	std::u16string titlesHashPath = u8tou16("/3ds/Sharkive/titles.sha");
	if (!fileExist(getArchiveSDMC(), titlesHashPath) || !fileExist(getArchiveSDMC(), savecachePath))
	{
		// create title list sha256 hash file if it doesn't exist in the working directory
		FSStream output(getArchiveSDMC(), titlesHashPath, FS_OPEN_WRITE, SHA256_BLOCK_SIZE);
		output.write(hash, SHA256_BLOCK_SIZE);
		output.close();
	}
	else
	{
		// compare current hash with the previous hash
		FSStream input(getArchiveSDMC(), titlesHashPath, FS_OPEN_READ);
		if (input.getLoaded() && input.getSize() == SHA256_BLOCK_SIZE)
		{
			u8* buf = new u8[input.getSize()];
			input.read(buf, input.getSize());
			input.close();
			
			if (memcmp(hash, buf, SHA256_BLOCK_SIZE) == 0)
			{
				// hash matches
				optimizedLoad = true;
			}
			else
			{
				FSUSER_DeleteFile(getArchiveSDMC(), fsMakePath(PATH_UTF16, titlesHashPath.data()));
				FSStream output(getArchiveSDMC(), titlesHashPath, FS_OPEN_WRITE, SHA256_BLOCK_SIZE);
				output.write(hash, SHA256_BLOCK_SIZE);
				output.close();
			}
			
			delete[] buf;
		}
	}
	
	if (optimizedLoad && !forceRefresh)
	{
		// deserialize data
		importTitleListCache();
	}
	else
	{
		u32 count = 0;
		AM_GetTitleCount(MEDIATYPE_SD, &count);
		titles.reserve(count + 1);

		u64 ids[count];
		AM_GetTitleList(NULL, MEDIATYPE_SD, count, ids);

		for (u32 i = 0; i < count; i++)
		{
			if (checkHigh(ids[i]))
			{
				Title title;
				if (title.load(ids[i], MEDIATYPE_SD, CARD_CTR))
				{
					titles.push_back(title);
				}
			}
		}
		
		std::sort(titles.begin(), titles.end(), [](Title l, Title r) {
			return l.getShortDescription() < r.getShortDescription();
		});
		
		FS_CardType cardType;
		Result res = FSUSER_GetCardType(&cardType);
		if (R_SUCCEEDED(res))
		{
			if (cardType == CARD_CTR)
			{
				AM_GetTitleCount(MEDIATYPE_GAME_CARD, &count);
				if (count > 0)
				{
					AM_GetTitleList(NULL, MEDIATYPE_GAME_CARD, count, ids);	
					if (checkHigh(ids[0]))
					{
						Title title;
						if (title.load(ids[0], MEDIATYPE_GAME_CARD, cardType))
						{
							titles.insert(titles.begin(), title);
						}
					}
				}
			}
		}
	}
	
	// serialize data
	exportTitleListCache();
	
	// load cache
	std::string url = "https://api.github.com/repos/BernardoGiordano/Sharkive/contents/db";
	std::u16string path = u8tou16("/3ds/Sharkive/cache.json");
	
	u32 sz = 0;
	createInfo("Loading...", "Downloading most recent cache from github..");
	Result res = httpDownloadFile(url, path, &sz);
	if (R_FAILED(res))
	{
		createError(res, "Failed to retrieve cache from github.");
	}
	
	createInfo("Loading...", "Reading most recent cache from disk..");
	if (fileExist(getArchiveSDMC(), path))
	{
		std::ifstream i(u16tou8(path));
		nlohmann::json j;
		i >> j;

		for (auto& object : j)
		{
			setAvailableOnDB(object["name"], object["download_url"], object["size"]);
		}
		createInfo("Success!", "Data loaded correctly.");
	}
}

void getTitle(Title &dst, int i)
{
	if (i < getTitlesCount())
	{
		dst = titles.at(i);
	}
}

int getTitlesCount(void)
{
	return titles.size();
}

size_t getTextureId(int i)
{
	return titles.at(i).textureId;
}

static void loadTextureIcon(smdh_s *smdh, size_t i) {
	static const u32 width = 48, height = 48;
	u32 *image = (u32*)malloc(width*height*sizeof(u32));

	for (u32 x = 0; x < width; x++)
	{
		for (u32 y = 0; y < height; y++)
		{
			unsigned int dest_pixel = (x + y*width);
			unsigned int source_pixel = (((y >> 3) * (width >> 3) + (x >> 3)) << 6) + ((x & 1) | ((y & 1) << 1) | ((x & 2) << 1) | ((y & 2) << 2) | ((x & 4) << 2) | ((y & 4) << 3));

			u8 r = ((smdh->bigIconData[source_pixel] >> 11) & 0b11111) << 3;
			u8 g = ((smdh->bigIconData[source_pixel] >> 5) & 0b111111) << 2;
			u8 b = (smdh->bigIconData[source_pixel] & 0b11111) << 3;
			u8 a = 0xFF;

			image[dest_pixel] = (r << 24) | (g << 16) | (b << 8) | a;
		}
	}

	pp2d_load_texture_memory(i, (u8*)image, (u32)width, (u32)height);
	free(image);
}

static void exportTitleListCache(void)
{
	u8* cache = new u8[titles.size() * 10];
	for (size_t i = 0; i < titles.size(); i++)
	{
		u64 id = titles.at(i).id;
		FS_MediaType media = titles.at(i).media;
		FS_CardType card = titles.at(i).card;
		memcpy(cache + i*10 + 0, &id, sizeof(u64));
		memcpy(cache + i*10 + 8, &media, sizeof(u8));
		memcpy(cache + i*10 + 9, &card, sizeof(u8));
	}
	
	std::u16string path = u8tou16("/3ds/Sharkive/cache");
	FSUSER_DeleteFile(getArchiveSDMC(), fsMakePath(PATH_UTF16, path.data()));
	FSStream output(getArchiveSDMC(), path, FS_OPEN_WRITE, titles.size() * 10);
	output.write(cache, titles.size() * 10);
	output.close();
	delete[] cache;
}

static void importTitleListCache(void)
{
	FSStream inputsaves(getArchiveSDMC(), u8tou16("/3ds/Sharkive/cache"), FS_OPEN_READ);
	u32 sizesaves = inputsaves.getSize() / 10;
	u8* cachesaves = new u8[inputsaves.getSize()];
	inputsaves.read(cachesaves, inputsaves.getSize());
	inputsaves.close();
	
	titles.clear();
	
	// if you're here to judge the code: yes, this is weird
	// and shouldn't need to be there, but people like it, so...
	for (size_t i = 0; i < sizesaves; i++)
	{
		Title title;
		title.load();
		titles.push_back(title);
	}
	
	for (size_t i = 0; i < sizesaves; i++)
	{
		u64 id;
		FS_MediaType media;
		FS_CardType card;
		memcpy(&id, cachesaves + i*10, sizeof(u64));
		memcpy(&media, cachesaves + i*10 + 8, sizeof(u8));
		memcpy(&card, cachesaves + i*10 + 9, sizeof(u8));
		Title title;
		title.load(id, media, card);
		titles.at(i) = title;
	}
	
	delete[] cachesaves;
}

void setAvailableOnDB(std::string name, std::string url, u32 size)
{
	auto it = find_if(titles.begin(), titles.end(), [&name](const Title& obj) {return obj.dbName == name;});
	if (it != titles.end())
	{
		auto index = std::distance(titles.begin(), it);
		titles.at(index).availableOnDB = true;
		titles.at(index).url = url;
		titles.at(index).dbSize = size;
	}
}

void setAvailableOnSD(size_t index, u32 size)
{
	titles.at(index).sdSize = size;
	titles.at(index).availableOnSD = true;
}