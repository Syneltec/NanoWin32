#include "NWUnitTest.h"

#ifdef __linux
 #include "NanoWinMFCAfxFile.h"
 #include <sys/types.h>
 #include <sys/stat.h>
 #include <unistd.h>
#else
 //#include <windows.h>
 #include <afx.h>
#endif

#define MAX_LENGTH     (1024)

#ifdef __linux
// #define PATH_DIR_SEPARATOR_CHAR  ('/')
static char PATH_DIR_SEPARATOR_STR[] = "/";
#else
// #define PATH_DIR_SEPARATOR_CHAR  ('\\')
static char PATH_DIR_SEPARATOR_STR[] = "\\";
#endif

static void CreateFile(const int size, const char *fileName, const char symbol)
{
	FILE *file;

	file = fopen(fileName, "w");

	if (file != NULL)
	{
		for (int i = 0; i < size; i++)
		{
			fputc(symbol, file);
		}
	}

	fclose(file);
}

static void RemoveTestDir()
{
	#ifdef __GNUC__

	unlink("testDir/testSubDir/testFile512.txt");
	rmdir("testDir/testSubDir");

	unlink("testDir/testFile512.txt");
	unlink("testDir/testFile256.txt");
	rmdir("testDir");

	#else

	DeleteFileA("testDir/testSubDir/testFile512.txt");
	RemoveDirectoryA("testDir/testSubDir");

	DeleteFileA("testDir/testFile512.txt");
	DeleteFileA("testDir/testFile256.txt");
	RemoveDirectoryA("testDir");

	#endif
}

static void CreateTestDir()
{
	#ifdef __GNUC__

	mkdir("testDir", S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH);
	mkdir("testDir/testSubDir", S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH);

	#else

	CreateDirectoryA("testDir", NULL);
	CreateDirectoryA("testDir/testSubDir", NULL);

	#endif

	CreateFile(512, "testDir/testSubDir/testFile512.txt", 'a');
	CreateFile(512, "testDir/testFile512.txt", 'a');
	CreateFile(256, "testDir/testFile256.txt", 'a');

}

NW_BEGIN_TEST_GROUP(NanoWinMFCAfxFileTestGroup)

NW_BEGIN_SETUP_TEARDOWN

NW_SETUP_METHOD()
{
	CreateTestDir();
}

NW_TEARDOWN_METHOD()
{
	RemoveTestDir();
}

NW_END_SETUP_TEARDOWN

NW_TEST(NanoWinMFCAfxFileTestGroup, CFileFindUnexistTest)
{
	CFileFind find;
	BOOL      res;

	CString str("testDir/unexist.txt");

	res = find.FindFile(str);

	NW_CHECK_FALSE(res);
}
/*
NW_TEST(NanoWinMFCAfxFileTestGroup, FindFileTest)
{
	CFileFind find;
	BOOL      res;

	CString str("testDir/testFile5*");

	res = find.FindFile(str);
	NW_CHECK_TRUE(res);

	res = find.FindNextFile();
	NW_CHECK_FALSE(res);

	CString resStr = find.GetFileName();
	NW_CHECK_EQUAL_STRCMP("testFile512.txt", resStr.GetString());
}
*//*
NW_TEST(NanoWinMFCAfxFileTestGroup, FindNextFileTest)
{
	CFileFind find;
	BOOL      res;

	CString resStr("testDir/testFile*");

	res = find.FindFile(resStr);
	NW_CHECK_TRUE(res);

	res = find.FindNextFile();
	NW_CHECK_TRUE(res);

	resStr = find.GetFileName();


	if (strcmp("testFile512.txt", resStr.GetString()) == 0)
	{
		NW_CHECK_EQUAL_STRCMP("testFile512.txt", resStr.GetString());
	}
	else
	{
		NW_CHECK_EQUAL_STRCMP("testFile256.txt", resStr.GetString());
	}

	res = find.FindNextFile();
	NW_CHECK_FALSE(res);

	resStr = find.GetFileName();


	if (strcmp("testFile512.txt", resStr.GetString()) == 0)
	{
		NW_CHECK_EQUAL_STRCMP("testFile512.txt", resStr.GetString());
	}
	else
	{
		NW_CHECK_EQUAL_STRCMP("testFile256.txt", resStr.GetString());
	}
}
*/
NW_TEST(NanoWinMFCAfxFileTestGroup, FindCloseTest)
{
	CFileFind find;
	BOOL      res;

	CString str1("testDir/testFile5*");
	CString str2("testDir/testFile2*");

	res = find.FindFile(str1);
	NW_CHECK_TRUE(res);

	res = find.FindNextFile();
	NW_CHECK_FALSE(res);

	find.Close();

	CString resStr = find.GetFileName();
	NW_CHECK_EQUAL_STRCMP("", resStr.GetString());

	res = find.FindFile(str2);
	NW_CHECK_TRUE(res);
}
/*
NW_TEST(NanoWinMFCAfxFileTestGroup, IsDirectoryTest)
{
	CFileFind find;
	BOOL      res;

	CString str1("testDir/testFile5*");
	CString str2("testDir/testSub*");

	res = find.FindFile(str1);
	NW_CHECK_TRUE(res);

	res = find.FindNextFile();
	NW_CHECK_FALSE(res);

	CString resStr = find.GetFileName();

	NW_CHECK_EQUAL_STRCMP("testFile512.txt", resStr.GetString());
	NW_CHECK_TRUE(find.IsDirectory() == 0);

	find.Close();

	res = find.FindFile(str2);
	NW_CHECK_TRUE(res);

	res = find.FindNextFile();
	NW_CHECK_FALSE(res);

	resStr = find.GetFileName();

	NW_CHECK_EQUAL_STRCMP("testSubDir", resStr.GetString());
	NW_CHECK_TRUE(find.IsDirectory() != 0);
}
*/
NW_TEST(NanoWinMFCAfxFileTestGroup, IsDotsTest)
{
	CFileFind find;
	BOOL      res;

	CString str("*.*");

	res = find.FindFile(str);
	NW_CHECK_TRUE(res);

	while (res)
	{
		res = find.FindNextFile();

		CString resStr = find.GetFileName();

		if ((strcmp(".", resStr.GetString()) == 0) || (strcmp("..", resStr.GetString()) == 0))
		{
			NW_CHECK_TRUE(find.IsDots());
		}
	}
}
/*
NW_TEST(NanoWinMFCAfxFileTestGroup, GetLengthTest)
{
	CFileFind find;
	BOOL      res;

	CString resStr("testDir/testFile*");

	res = find.FindFile(resStr);
	NW_CHECK_TRUE(res);

	res = find.FindNextFile();
	NW_CHECK_TRUE(res);

	resStr = find.GetFileName();


	if (strcmp("testFile512.txt", resStr.GetString()) == 0)
	{
		NW_CHECK_EQUAL_LONGS(512, find.GetLength());
	}
	else
	{
		NW_CHECK_EQUAL_LONGS(256, find.GetLength());
	}

	res = find.FindNextFile();
	NW_CHECK_FALSE(res);

	resStr = find.GetFileName();


	if (strcmp("testFile512.txt", resStr.GetString()) == 0)
	{
		NW_CHECK_EQUAL_LONGS(512, find.GetLength());
	}
	else
	{
		NW_CHECK_EQUAL_LONGS(256, find.GetLength());
	}
}
*//*
NW_TEST(NanoWinMFCAfxFileTestGroup, GetFilePathTest)
{
	CFileFind find;
	BOOL      res;
	char      fullFileName[MAX_LENGTH];

	CString str("testDir/testFile5*");

	res = find.FindFile(str);
	NW_CHECK_TRUE(res);

	res = find.FindNextFile();
	NW_CHECK_FALSE(res);

	CString resStr = find.GetFilePath();

	GetCurrentDirectoryA(sizeof(fullFileName), fullFileName);

	strcat(fullFileName, PATH_DIR_SEPARATOR_STR);
	strcat(fullFileName, "testDir");
	strcat(fullFileName, PATH_DIR_SEPARATOR_STR);
	strcat(fullFileName, "testFile512.txt");

	NW_CHECK(strcmp(fullFileName, resStr.GetString()) == 0);
}
*/
NW_END_TEST_GROUP()