# TODO

* 当前实现的只是打包解包，但是后期加入压缩和解压缩的话，程序需要确认该tar文件是否是经过压缩的。可能需要在文件头部写入魔数。
* tar_header 内部的filename有越界风险。另外要考虑是不是应该只保存文件名。
* load 以后所创建的文件节点链表没有什么实际作用，测试不了，无法保证正确性

