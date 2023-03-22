# gcc

> gcc [选项1] [选项2] [选项3] ...
> 
> 
> （1）-o 选项：可以指定编译后输出的可执行文件的名称。
> 如：helloworld.c 编译后指定输出文件名为test，使用方式如下：
>     gcc -o test helloworld.c
>  
> （2）-c 选项：只编译C语言代码，不进行汇编连接。
>     如 gcc -c helloworld.c 会产生一个叫 helloworld.o 的目标文件。
>  
> （3）-S 选项，编译并产生汇编源文件。
>     如 gcc -S helloworld.c 会产生一个 helloworld.s 的汇编源文件。
>  
> （4）-E 选项，只对C源文件进行预处理。
>     如 gcc -E helloworld.c 只对C源文件中的宏和预处理进行展开，不编译C源文件。
> 
> 如果要编译C语言文件，最简单的方式就是：
> gcc 源文件
>  
> 注意：使用这个方式直接编译的c源文件会默认生成 a.out 的可执行文件。
> 
> 如果有源文件：
> ++ main.cpp src/swap.cpp -Iinclude -o main

# cmake

> -DCMAKE_BUILD_TYPE=Debug