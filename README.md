# minios-arm

一个面向 ARM64 的 mini 操作系统实验项目。目标是参考 Linux 0.11 的结构和学习路径，从最小内核开始，逐步实现中断、内存管理、进程、系统调用、文件系统和 shell，最终移植到 Raspberry Pi 等真实硬件上运行。

## 当前目标

第一阶段先支持 QEMU ARM64 `virt` 机器：

- ARM64 裸机启动入口
- 清空 BSS
- 设置内核栈
- 初始化 PL011 UART
- 最小日志/printf 模块
- ARM64 异常向量表
- GICv2 IRQ 分发
- ARM generic timer
- 每秒打印 timer tick

## 目录结构

```text
.
├── Makefile
├── linker.ld
└── src
    ├── boot.S
    ├── drivers
    │   └── uart.c
    ├── include
    │   ├── irq.h
    │   ├── log.h
    │   ├── timer.h
    │   └── uart.h
    └── kernel
        ├── exception.S
        ├── irq.c
        ├── log.c
        ├── main.c
        └── timer.c
```

## 依赖

macOS 上建议安装：

```bash
brew install llvm lld qemu
```

如果 `clang` 或 `llvm-objcopy` 不在 PATH 中，可以把 Homebrew LLVM 加入 PATH：

```bash
export PATH="$(brew --prefix llvm)/bin:$PATH"
```

## 构建

```bash
make
```

生成物：

- `build/minios-arm.elf`：QEMU 调试用 ELF
- `build/kernel8.img`：后续 Raspberry Pi 启动镜像

## 运行

```bash
make run
```

预期输出：

```text
[info] miniOS ARM64 booting...
[info] initializing interrupts and timer
[info] timer frequency 62500000 Hz
[info] timer interrupt enabled
[info] tick 1
[info] tick 2
```

退出 QEMU：

```text
Ctrl-A X
```

## 路线图

1. QEMU ARM64 最小启动和 UART 输出。
2. 移植到 Raspberry Pi 3B / Zero 2 W，生成 `kernel8.img` 上板启动。
3. 加入异常向量和定时器中断。
4. 实现物理页分配和 ARM64 页表，开启 MMU。
5. 实现内核线程和简单调度器。
6. 加入 EL0 用户态、系统调用和第一个用户程序。
7. 实现 ramfs、init 和简易 shell。

## 和 Linux 0.11 的关系

本项目不直接移植 Linux 0.11 的 x86 代码，而是借鉴它的学习路径和内核结构。Linux 0.11 的启动、中断、分页和任务切换代码都强绑定 x86，树莓派使用 ARM64，需要重新实现底层架构相关部分。
