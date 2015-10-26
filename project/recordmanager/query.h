//
// Created by huangsy13 on 10/26/15.
//

#ifndef DATABASE_QUERY_H
#define DATABASE_QUERY_H

//用于构成查询的类
class Query {
private:
    int start;
    int end;
    int *search_part;
public:
    Query() { }

    Query(int start, int end, int *search_part) : start(start), end(end), search_part(search_part) { }

    virtual ~Query() { }

    int getEnd() const {
        return end;
    }

    void setEnd(int end) {
        Query::end = end;
    }

    int getStart() const {
        return start;
    }

    void setStart(int start) {
        Query::start = start;
    }

    int *getSearch_part() const {
        return search_part;
    }

    void setSearch_part(int *search_part) {
        Query::search_part = search_part;
    }
};

#endif //DATABASE_QUERY_H
