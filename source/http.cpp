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

#include "http.h"

Result httpDownloadFile(std::string url, std::u16string path)
{
	Result res = 0;
	httpcInit(0);
	httpcContext context;
	u32 statuscode = 0;
	u32 contentsize = 0;

	res = httpcOpenContext(&context, HTTPC_METHOD_GET, url.c_str(), 0);
	if (R_FAILED(res))
	{
		httpcCloseContext(&context);
		createError(res, "Failed to open http context.");
		return res;
	}

	res = httpcAddRequestHeaderField(&context, "User-Agent", "Sharkive");
	if (R_FAILED(res))
	{
		httpcCloseContext(&context);
		createError(res, "Failed to add a request header field.");
		return res;
	}

	res = httpcSetSSLOpt(&context, SSLCOPT_DisableVerify);
	if (R_FAILED(res))
	{
		httpcCloseContext(&context);
		createError(res, "Failed to set SSL opt.");
		return res;
	}

	res = httpcAddRequestHeaderField(&context, "Connection", "Keep-Alive");
	if (R_FAILED(res))
	{
		httpcCloseContext(&context);
		createError(res, "Failed to keep the connection alive.");
		return res;
	}

	res = httpcBeginRequest(&context);
	if (R_FAILED(res))
	{
		httpcCloseContext(&context);
		createError(res, "Failed to begin http request.");
		return res;
	}

	res = httpcGetResponseStatusCode(&context, &statuscode);
	if (R_FAILED(res))
	{
		httpcCloseContext(&context);
		createError(res, "Failed to get a response status code.");
		return res;
	}

	res = httpcGetDownloadSizeState(&context, NULL, &contentsize);
	if (R_FAILED(res))
	{
		httpcCloseContext(&context);
		createError(res, "Failed to get download size.");
		return res;
	}

	u8 *buf = new u8[contentsize]();
	if (R_FAILED(res))
	{
		delete[] buf;
		httpcCloseContext(&context);
		createError(res, "Failed to allocate buffer.");
		return res;
	}

	res = httpcDownloadData(&context, buf, contentsize, NULL);
	if (R_FAILED(res))
	{
		delete[] buf;
		httpcCloseContext(&context);
		createError(res, "Failed to download data.");
		return res;
	}

	FSStream output(getArchiveSDMC(), path, FS_OPEN_WRITE, contentsize);
	output.write(buf, contentsize);
	output.close();

	delete[] buf;

	httpcCloseContext(&context);
	httpcExit();
	return res;
}