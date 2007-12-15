#include "mtfile.h"

#ifdef Q_WS_MAC
#ifndef USE_UNIX_TOUCH_COMMAND

#import <Foundation/NSFileManager.h>
#import <Foundation/NSString.h>

bool MTFile::copy(QString dest)
{
	const char * str1 = QByteArray().append(dest).constData();
	NSString * nsdest = [[NSString alloc] initWithUTF8String:str1];
	QFileInfo srcinfo(*this);
	const char * str2 = QByteArray().append(srcinfo.absoluteFilePath()).constData();
	NSString * nssrc = [[NSString alloc] initWithUTF8String:str2];
	NSFileManager * fileManager = [NSFileManager defaultManager];
	if ([fileManager fileExistsAtPath:nssrc]) {
		if ([fileManager copyPath:nssrc toPath:nsdest handler:nil] == YES) {
			return true;
		} else {
			return false;
		}
	}
	return false;
}

#endif
#endif
