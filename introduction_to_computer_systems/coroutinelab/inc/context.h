#pragma once
#include <assert.h>
#include <cstdint>
#include <functional>
#include <tuple>
#include <type_traits>

//! 枚举寄存器，方便状态的储存
enum class Registers : int {
  RAX = 0,
  RDI,
  RSI,
  RDX,
  R8,
  R9,
  R10,
  R11,
  RSP,
  RBX,
  RBP,
  R12,
  R13,
  R14,
  R15,
  RIP,
  RegisterCount
};

//! 汇编接口
//! 如何在 C++ 中调用汇编代码？可以使用 extern "C" 来声明汇编函数的接口，在额外再汇编代码中实现这些函数
//! 从而实现再cpp代码中调用汇编代码编写的函数
extern "C" {
//! 协程入口函数
//! 调用这个函数会实现：调用统一协程入口函数 coroutine_main，开始执行协程
//! 作用：避开 c++ 函数调用的规范，直接从汇编层面进行函数的调用。
void coroutine_entry();
//! 协程切换
// 当前状态保存到save，从restore恢复
void coroutine_switch(uint64_t *save, uint64_t *restore);
}



//! 协程的状态，用于保存和恢复协程
// 只包含了基本的寄存器保存和栈信息
struct basic_context {
  // 协程自己的假栈
  //! 系统中的真实栈是为单线程进行设计的，无法承载多个协程的上下文（变量，调用等会互相覆盖）
  //! 所以我们为每个协程在堆空间上开辟一个连续的内存空间，作为协程的专属栈
  //! 如何让专属栈模拟真实栈的行为呢？实际上专属栈和真实的栈没有什么区别，都是一个连续的内存空间，只需要将rsp指向专属栈的栈顶就行了
  //! 注意要将rsp指向开辟出来的专属栈空间的高地址位置
  // 栈顶的位置也作为协程的状态进行储存
  uint64_t *stack;
  uint64_t stack_size;
  // 调度器协程的状态
  uint64_t caller_registers[(int)Registers::RegisterCount];
  // 当前协程本身的状态
  uint64_t callee_registers[(int)Registers::RegisterCount];
  bool finished;
  bool ready;
  std::function<bool()> ready_func;

  basic_context(uint64_t stack_size)
      : finished(false), ready(true), stack_size(stack_size) {
    stack = new uint64_t[stack_size];

    // TODO: Task 1
    // 在实验报告中分析以下代码
    // 对齐到 16 字节边界
    uint64_t rsp = (uint64_t)&stack[stack_size - 1];
    rsp = rsp - (rsp & 0xF);

    void coroutine_main(struct basic_context * context);

    callee_registers[(int)Registers::RSP] = rsp;
    // 协程入口是 coroutine_entry
    callee_registers[(int)Registers::RIP] = (uint64_t)coroutine_entry;
    // 设置 r12 寄存器为 coroutine_main 的地址
    callee_registers[(int)Registers::R12] = (uint64_t)coroutine_main;
    // 设置 r13 寄存器，用于 coroutine_main 的参数
    callee_registers[(int)Registers::R13] = (uint64_t)this;
  }

  ~basic_context() { delete[] stack; }

  virtual void run() = 0;
  virtual void resume() = 0;
};

// TODO: Task 1
// 在实验报告中分析以下代码
// 协程运行的主函数，是协程的统一入口，传入协程对象，调用这个函数就可以启动协程
void coroutine_main(struct basic_context *context) {
  // 多态调用，开始运行协程
  context->run();
  context->finished = true;
  coroutine_switch(context->callee_registers, context->caller_registers);

  // unreachable
  assert(false);
}

extern __thread basic_context *g_current_context;

// boilerplate code to handle variadic function arguments
//! 一堆宏定义，主要是因为函数的参数是可变的，需要将 tuple 中的参数展开成具体的参数传递给函数
// 这里实际上就是将args中的参数逐个拿出来，EXPAND_CALL_x就是从args中取出前x个参数，展开成"arg1, arg2, ..., argx"的形式
#define EXPAND_CALL_0(args)
#define EXPAND_CALL_1(args) (std::get<0>(args))
#define EXPAND_CALL_2(args) EXPAND_CALL_1(args), (std::get<1>(args))
#define EXPAND_CALL_3(args) EXPAND_CALL_2(args), (std::get<2>(args))
#define EXPAND_CALL_4(args) EXPAND_CALL_3(args), (std::get<3>(args))
#define EXPAND_CALL_5(args) EXPAND_CALL_4(args), (std::get<4>(args))
#define EXPAND_CALL_6(args) EXPAND_CALL_5(args), (std::get<5>(args))
#define EXPAND_CALL_7(args) EXPAND_CALL_6(args), (std::get<6>(args))

// if constexpr requires C++17,可以做到编译期条件判断
// 当参数个数x和args的参数个数相等时，展开对应的宏
#define CALLER_IMPL(func, x, args)                                             \
  if constexpr (std::tuple_size_v<std::decay_t<decltype(args)>> == x)          \
  func(EXPAND_CALL_##x(args))

// 编译期时，会依次将CALL(func, args) 展开为CALLER_IMPL(func, x, args)，x可以取到0~7
// 然后CALLER_IMPL(func, x, args)会进行编译期条件判断，只有当x和args的参数个数相等时，
// 才会展开对应的func(EXPAND_CALL_X(args))宏
// 这里的func就是协程函数，args就是协程函数的参数列表,通过EXPAND_CALL_X宏展开从args中取出参数来传递给func进行函数调用
#define CALL(func, args)                                                       \
  CALLER_IMPL(func, 0, args);                                                  \
  CALLER_IMPL(func, 1, args);                                                  \
  CALLER_IMPL(func, 2, args);                                                  \
  CALLER_IMPL(func, 3, args);                                                  \
  CALLER_IMPL(func, 4, args);                                                  \
  CALLER_IMPL(func, 5, args);                                                  \
  CALLER_IMPL(func, 6, args);                                                  \
  CALLER_IMPL(func, 7, args);

/**
 * @brief
 * 协程运行时资源管理。存储了协程函数，以及协程函数的运行时栈即寄存器内容等。
 *
 * @tparam F 协程函数类
 * @tparam Args 协程函数所需要的参数列表
 * 在当前情况下，协程函数支支持展开至多 7 个参数。
 * 如果需要更多的参数需要
 *   1. 参考修改 CALL 的宏定义以及添加对应的 EXPAND_CALL_X 的宏定义。
 *   2. 需要修改构造函数中的 static_assert。
 */
//! 进一步包装形成协程对象
//! 包含以下部分：
// 1. 协程执行的函数：F f
// 2. 协程函数的参数：std::tuple<Args...> args（使用tuple来进行保存）
// 3. 继承 basic_context，保存协程的寄存器和栈信息
// 4. 构造函数，初始化协程函数和参数，并且设置协程栈信息
// 5. 开始运行协程，暂停协程，重新恢复运行等函数
template <typename F, typename... Args>
struct coroutine_context : public basic_context {
  F f;
  std::tuple<Args...> args;

  // construct a stacked coroutine, with stack size 16 KB
  coroutine_context(F f, Args... args)
      : f(f), args(std::tuple<Args...>(args...)),
        basic_context(16 * 1024 / sizeof(uint64_t)) {
    static_assert(sizeof...(args) <= 7);
  }

  // construct a stacked coroutine, with stack_size (in KB)
  coroutine_context(uint64_t stack_size, F f, Args... args)
      : f(f), args(std::tuple<Args...>(args...)),
        basic_context(stack_size * 1024 / sizeof(uint64_t)) {
    static_assert(sizeof...(args) <= 7);
  }

  /**
   * @brief 恢复协程函数运行。
   * TODO: Task 1
   * 你需要保存 callee-saved 寄存器，并且设置协程函数栈帧，然后将 rip 恢复到协程
   * yield 之后所需要执行的指令地址。
   */
  virtual void resume() {
    // 调用 coroutine_switch
    // 在汇编中保存 callee-saved 寄存器，设置协程函数栈帧，然后将 rip 恢复到协程 yield 之后所需要执行的指令地址。
    // 第一个参数是保存当前状态（caller），第二个参数是恢复到的状态（callee）
    coroutine_switch(caller_registers, callee_registers);
  }

  virtual void run() { CALL(f, args); }
};