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
    BufType search_part;
public:
    Query(int start = -1, int end = -1, BufType search_part = nullptr) : start(start), end(end),
                                                                         search_part(search_part) { }

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

    BufType getSearch_part() const {
        return search_part;
    }

    void setSearch_part(BufType search_part) {
        Query::search_part = search_part;
    }
};

#endif //DATABASE_QUERY_H
