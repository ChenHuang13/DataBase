记录管理模块：
RecordManager类：被系统管理模块、查询解析模块和索引模块调用，执行底层的记录查询，记录插入，记录删除功能。
HeadInfo类：通过传入buffer和pageID,读出该文件页头的信息。
Record类:一条抽象记录类，包含数据段和其他信息段。
Query类：抽象查询请求，包含查询的区间和字段的值。
Field类：储存每种表的字段信息，会被Data类和Table类当成成员变量。
Data类：一条数据的类，记录数据信息。
Record* dataToRecord(data)函数：把数据打包成record，便于直接插入。
Data* recordToData(Record)函数：每种数据需要单独实现这个函数，用于解析记录成Data类。
Table类：表项基类，提供表的创建和查询。