
- 项目功能
  - day1：实现两个client端（clientData2，clientData3）同时向一个server端发送数据，数据结构为统一的结构体student
  - day1：server端每接收1000条数据，就将数据保存为txt文件格式，文件以创建时间命名，保存时根据id排序
  - day2：另起XRCE-Agent接收源自另一client端（clientData1）通过dds传输的数据，将server和agent集成到center中统一处理
  - day3：统合三个client端发送的数据，在数据条目达到1000条时center将其保存为以时间戳命名的文件，并将文件名与时间戳发送给clientData1

- 技术细节
  - day1：client端向server端发送数据时，不能直接使用结构体结构传输，需要将结构体串行为字符数组，同时对字符串类型末端注'\0\'
  - day1：server端接收client端数据时，应对多client需要多线程
  - day2：使用XRCEGen生成指定idl文件的.c和.h文件，其中包括对指定结构的串行、反串行函数。clintData1利用生成的该函数封装需要发送的数据，并在agent处解码得到数据
  - day2：以Publish***为原型编写clientData1，快速实现数据发送功能
  - day2：使用vector容器保存数据，并在保存文件时对1000条数据进行排序，并利用erase清除已存入的数据，此过程中不需要对线程上锁，新加入的数据在vector末端，排序与插入只影响vector前1000条数据。对比set与map需要对写入过程上锁，vector可以保证接收其他client端数据（后统一采用queue）
  - day3：使用单例全局静态队列保存agent和server收到的数据信息，再由center统一处理
  - day3：一开始尝试使用CustomAgent编写agent功能，成功接收到二进制数据，但没能将其成功转换为ucdrBuffer格式再反串行，后使用Subscribe接收数据，即在agent外又创建了一个接收client，但没能同时实现收发功能
- 遇到的bug
  - day1：tcp传输时直接发送结构体乱码，发送string乱码，推测是\0问题，
  - day2：动态链接库问题，添加命令行参数后解决

> 目前的思路：
> - 自定义agent的方向还是可行的，customAgent中自定义了初始化、结束、接收、发送函数，但在收发函数处得到的是二进制uint*数组类型，阻碍的点在于如何向ucdrBuffer数据类型转变，再解串得到目标数据类型（尝试使用ucdr_init_buffer函数转换但失败）
> - 对于client不论是Publisher还是Subscriber都是在microclient动态链接库基础上调包实现，继续探索可以同时实现收发功能
  
- 开始
  - 运行四个可执行文件，server将接收来自clientData2和clientData3的数据，并执行保存，agent将接收来自clientData1的数据
  - 运行指令（build目录下）：
    - ../center udp4 -p 2019 -v 6
    - ../clientData1 127.0.0.1 2019
    - ../clientData2
    - ../clientData3
- 数据结构
  - student：
    - name：string
    - number：long，1~1000000的随机数
    - grade：long，不同client保有不同grade
- 进度
  - server可以同时接收多个client的输入信息
  - center整合了server和agent接收到的信息
- TODO
  - 添加线程的过程由线程池代替
  - 将txt文本格式替换为数据库保存数据
  - center将文件信息发送给clientData1