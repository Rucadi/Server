#include "stdafx.h"
#include "MarkManager.h"

#ifdef _WIN32
#include <direct.h>
#endif

#define OLD_MARK_INDEX_FILENAME "guild_mark.idx"
#define OLD_MARK_DATA_FILENAME "guild_mark.tga"

static Pixel * LoadOldGuildMarkImageFile()
{
	FILE * fp = fopen(OLD_MARK_DATA_FILENAME, "rb");

	if (!fp)
	{
		sys_err("cannot open %s", OLD_MARK_INDEX_FILENAME);
		return NULL;
	}

	int dataSize = 512 * 512 * sizeof(Pixel);
	Pixel * dataPtr = (Pixel *) malloc(dataSize);

	fread(dataPtr, dataSize, 1, fp);

	fclose(fp);

	return dataPtr;
}

bool GuildMarkConvert(const std::vector<DWORD> & vecGuildID)
{
	// Жъґх »эјє
#ifndef _WIN32
	mkdir("mark", S_IRWXU);
#else
	_mkdir("mark");
#endif

	// АОµ¦Ѕє ЖДАПАМ АЦіЄ? 
#ifndef _WIN32
	if (0 != access(OLD_MARK_INDEX_FILENAME, F_OK))
#else
	if (0 != _access(OLD_MARK_INDEX_FILENAME, 0))
#endif
		return true;

	// АОµ¦Ѕє ЖДАП ї­±в
	FILE* fp = fopen(OLD_MARK_INDEX_FILENAME, "r");

	if (NULL == fp)
		return false;

	// АМ№МБц ЖДАП ї­±в
	Pixel * oldImagePtr = LoadOldGuildMarkImageFile();

	if (NULL == oldImagePtr)
	{
		fclose(fp);
		return false;
	}

	/*
	// guild_mark.tga°Ў ЅЗБ¦ targa ЖДАПАМ ѕЖґП°н, 512 * 512 * 4 Е©±вАЗ raw ЖДАПАМґЩ.
	// ґ«Аё·О И®АОЗП±в А§ЗШ ЅЗБ¦ targa ЖДАП·О ёёµзґЩ.
	CGuildMarkImage * pkImage = new CGuildMarkImage;
	pkImage->Build("guild_mark_real.tga");
	pkImage->Load("guild_mark_real.tga");
	pkImage->PutData(0, 0, 512, 512, oldImagePtr);
	pkImage->Save("guild_mark_real.tga");
	*/
	sys_log(0, "Guild Mark Converting Start.");

	char line[256];
	DWORD guild_id;
	DWORD mark_id;
	Pixel mark[SGuildMark::SIZE];

	while (fgets(line, sizeof(line)-1, fp))
	{
		sscanf(line, "%u %u", &guild_id, &mark_id);

		if (find(vecGuildID.begin(), vecGuildID.end(), guild_id) == vecGuildID.end())
		{
			sys_log(0, "  skipping guild ID %u", guild_id);
			continue;
		}

		// mark id -> АМ№МБцїЎј­АЗ А§ДЎ ГЈ±в
		uint row = mark_id / 32;
		uint col = mark_id % 32;

		if (row >= 42)
		{
			sys_err("invalid mark_id %u", mark_id);
			continue;
		}

		uint sx = col * 16;
		uint sy = row * 12;

		Pixel * src = oldImagePtr + sy * 512 + sx;
		Pixel * dst = mark;

		// їѕіЇ АМ№МБцїЎј­ ё¶Е© ЗС°і є№»з
		for (int y = 0; y != SGuildMark::HEIGHT; ++y)
		{
			for (int x = 0; x != SGuildMark::WIDTH; ++x)
				*(dst++) = *(src+x);

			src += 512;
		}

		// »х ±жµе ё¶Е© ЅГЅєЕЫїЎ іЦґВґЩ.
		CGuildMarkManager::instance().SaveMark(guild_id, (BYTE *) mark);
		line[0] = '\0';
	}

	free(oldImagePtr);
	fclose(fp);

	// ДБ№цЖ®ґВ ЗС№шёё ЗПёйµЗ№З·О ЖДАПА» їЕ°ЬБШґЩ.
#ifndef _WIN32
	system("mv -f guild_mark.idx guild_mark.idx.removable");
	system("mv -f guild_mark.tga guild_mark.tga.removable");
#else
	system("move /Y guild_mark.idx guild_mark.idx.removable");
	system("move /Y guild_mark.tga guild_mark.tga.removable");
#endif

	sys_log(0, "Guild Mark Converting Complete.");

	return true;
}

