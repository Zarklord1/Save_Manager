import os

with open(os.path.abspath("src/resources/filelist.h"), "wb") as f:
	f.write("""#pragma once

typedef struct _RecourceFile
{
	const char *filename;
	const u8   *DefaultFile;
	const u32  &DefaultFileSize;
	u8		   *CustomFile;
	u32		    CustomFileSize;
} RecourceFile;

""")
	filePointerPrefix = "extern const u8 "
	filePointerSuffix = "[];\n"
	fileSizePrefix = "extern const u32 "
	fileSizeSuffix = "_size;\n\n"
	filePath = [];
	cName = [];
	for root, dirs, files in os.walk(os.path.abspath("data")):
		for name in files:
			filePath.append(('"' + os.path.join(root,name)[len(os.path.abspath("data"))+1:] + '"').replace("\\", "/"))
			cName.append(name.replace(".", "_"))
	for i in range(len(cName)):
		f.write(filePointerPrefix + cName[i] + filePointerSuffix)
		f.write(fileSizePrefix + cName[i] + fileSizeSuffix)
	f.write("""static RecourceFile RecourceList[] =
{
""")
	for i in range(len(filePath)):
		f.write('	{' + filePath[i] + ', ' + cName[i] + ', ' + cName[i] + '_size, NULL, 0},\n')
	f.write("""	{NULL, NULL, 0, NULL, 0}
};""")
	
				
