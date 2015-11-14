//
// Created by huangsy13 on 11/13/15.
//

//HeadInfo类：通过传入buffer和pageID,读出该文件页头的信息。

#ifndef DATABASE_PAGEINFO_H
#define DATABASE_PAGEINFO_H

class PageInfo{
public:
	int used;
	int body;
};

#endif //DATABASE_HEADINFO_H
