/*
 * exec_delete.h
 *
 *  Created on: 2014年12月26日
 *      Author: lql
 */

#ifndef EXEC_DELETE_H_
#define EXEC_DELETE_H_

#include "../define.h"
#include "../parser.h"
void exec_delete(char* sql) {
	char* prev, *next;
	prev = sql;
	next = paser.getWord(prev, ' ');
	prev = next + 1;
	next = paser.getWord(prev, ' ');
	*next = '\0';
	if (cdbs == NULL) {
		printf("error: no db\n");
		return;
	}
	ctb = cdbs->getTable(prev);
	if (ctb == NULL) {
		printf("error: no table\n");
		return;
	}
	tabs.clear();
	tabs.push_back(ctb);
	prev = next + 1;
	next = paser.getWord(prev, ' ');
	paser.exec_where(next + 1);
	if (errwhere == 1) {
		return;
	}
	cidx = -1;
	int w = -1;
	for (int j = 0; j < ctb->cn; ++ j) {
		if (ctb->col[j].bpt[0] != NULL) {
			if (w == -1 || w > ctb->rangeCount(j)) {
				cidx = j;
				w = ctb->rangeCount(j);
			}
		}
	}
	ans1.clear();
	ctb->select(cidx, ctb->range, ans1);
	int m = ans1.size();
	for (int i = 0; i < m; ++ i) {
		int p = ans1[i].first;
		int s = ans1[i].second;
		ctb->erase(p, s);
	}
}
#endif /* EXEC_DELETE_H_ */
