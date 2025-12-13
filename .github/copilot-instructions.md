# Copilot / AI 指南（针对本仓库）

目的：让 AI 编码助手快速上手本仓库并生成高质量补丁。文档聚焦于可被代码直接发现的约定、构建/运行/调试命令、以及最常修改的“热点”文件和模式。

1) 大局观（为什么这样组织）
- 本仓库是教学/实验类代码集合。各实验目录以课程名组织（例如 `introduction_to_computer_systems/coroutinelab`）。每个实验通常包含源码、汇编、Makefile、`bin/` 可执行文件以及 `REPORT.md`/`README.md`。
- `coroutinelab` 实验的目的是实现用户态协程库，关键点在于 C++ 层与汇编层（`lib/context.S`）之间通过寄存器保存/恢复实现上下文切换。

2) 关键文件/目录（优先参考）
- `introduction_to_computer_systems/coroutinelab/README.md`：实验说明与运行期预期输出。
- `introduction_to_computer_systems/coroutinelab/REPORT.md`：学生报告与代码实现说明，包含调用流程说明（resume/yield、coroutine_entry 等）。
- `introduction_to_computer_systems/coroutinelab/src/`：C++ 源码，协程池与测试程序（`sample`, `sleep_sort`, `binary_search` 相关实现）。
- `introduction_to_computer_systems/coroutinelab/inc/`：头文件（`common.h`, `context.h`, `coroutine_pool.h`）——查看类与接口定义。
- `introduction_to_computer_systems/coroutinelab/lib/context.S`：汇编实现——上下文切换的核心（rsp/rip/r12/r13 等寄存器的使用约定）。
- `introduction_to_computer_systems/coroutinelab/Makefile`：构建流程（在该目录 `make` 即可生成 `bin/` 下的可执行文件）。

3) 构建 / 运行 / 调试（不可或缺的命令）
- 编译（在 `coroutinelab` 目录）：
  ```bash
  cd introduction_to_computer_systems/coroutinelab
  make
  ```
- 运行示例：
  - `./bin/sample`：用于验证基本协程切换输出（见 `README.md` 中示例）。
  - `./bin/sleep_sort`：交互式测试 `sleep` 功能，按照 README 给出的示例输入查看输出。
- 使用调试符号：如果需要单步调试，请在 Makefile/CFLAGS 中启用 `-g`（Makefile 已默认支持），然后：
  ```bash
  gdb --args ./bin/sample
  ```
  在调试时，重点设置断点的位置：`coroutine_entry`、`coroutine_main`、`coroutine_switch`、以及 `lib/context.S` 中对应标签。

4) 项目中特殊约定与模式（AI 应遵守并利用）
- 协程上下文通过两个寄存器数组保存/恢复：`caller_registers` 与 `callee_registers`。切入与切出时保存位置不同（请参照 `REPORT.md` 的描述）。
- `rsp` 会被初始化为协程专属栈顶——因此栈空间为堆上分配，修改栈管理要非常小心。
- 汇编层使用 `r12` 保存 `coroutine_main` 地址，`r13` 保存协程对象指针；修改这些寄存器约定会导致严重问题。
- 代码中会出现 `TODO: Task 1` / `TODO: Task 2` 注释——这些是可定位的修改点。

5) 常见修改热点（AI 修改时优先关注）
- `coroutine_pool::serial_execute_all`：实现轮询/ready 判定与 resume/yield 逻辑（Task 1/2 的核心）。
- `context.S`（或 `lib/context.S`）：实现 `coroutine_switch` 汇编逻辑。任何变更都必须保留 ABI 与寄存器约定。
- `lookup_coroutine`：二分查找的协程化预取与 yield 插入位置（Task 3 优化点）。

6) 编写补丁时的具体建议与示例
- 小步修改、逐文件提交：先修改 C++ 层并通过单元运行，再在汇编层做微调并通过 `./bin/sample` 复验。
- 当要在协程入口/返回处插入日志或断言，优先在 `coroutine_main` / `run` 层增加可控的 `printf`，避免在汇编中大量打印。
- 示例：在实现 `sleep` 时，修改 `coroutine_pool::serial_execute_all` 的调度分支，伪代码参考：
  ```cpp
  if (!co->ready) {
    if (co->ready_func()) co->ready = true;
    else continue;
  }
  resume(co);
  ```

7) 不要做的事情（危险或不可行）
- 不要随意重命名或移除 `r12`/`r13` 相关寄存器用法；不要改变 `coroutine_switch` 的参数/返回 ABI。
- 尽量不要将协程栈从堆改为静态/全局内存——测试与分配假设依赖于堆。

8) 检查点 & 期望输出（用于验证补丁）
- 运行 `./bin/sample`，输出应与 `README.md` 中示例相符（交替的 `in show()` / `in main()` 值）。
- 运行 `./bin/sleep_sort`，输入 `1 3 4 5 2` 应输出从小到大每行一个数字（参考 README 示例）。

如果本文件有遗漏或需要补充的工程约定（例如特定编译器标志、CI 步骤、或其他实验目录的特殊约定），请告知具体目录或文件，我会把它合并进来并按需更新本文件。
