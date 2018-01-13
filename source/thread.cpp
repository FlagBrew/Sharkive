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

#include "thread.h"

static std::vector<Thread> threads;

// these are not needed because there's no way to trigger
// the loadTitle thread another time, but I'll keep them
// in case it gets added in the future
static bool forceRefresh = false;
static volatile bool isLoadingTitles = false;

void createThread(ThreadFunc entrypoint)
{
	s32 prio = 0;
	svcGetThreadPriority(&prio, CUR_THREAD_HANDLE);
	Thread thread = threadCreate((ThreadFunc)entrypoint, NULL, 4*1024, prio-1, -2, false);
	threads.push_back(thread);
}

void destroyThreads(void)
{
	for (u32 i = 0; i < threads.size(); i++)
	{
		threadJoin(threads.at(i), U64_MAX);
		threadFree(threads.at(i));
	}
}

void threadLoadTitles(void)
{
	// don't load titles while they're loading
	if (isLoadingTitles)
	{
		return;
	}

	isLoadingTitles = true;
	loadTitles(forceRefresh);
	forceRefresh = true;
	isLoadingTitles = false;
}

void threadDownloadCodeList(void)
{
	std::string url = "https://api.github.com/repos/BernardoGiordano/Sharkive/contents/db";
	std::u16string path = u8tou16("/3ds/Sharkive/cache.json");
	
	createInfo("Loading...", "Downloading most recent cache from github..");
	Result res = httpDownloadFile(url, path);
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
			setAvailableOnDB(object["name"], object["download_url"]);
		}
		createInfo("Success!", "Data loaded correctly.");
	}
}