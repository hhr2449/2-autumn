#pragma once
#include "context.h"
#include <memory>
#include <thread>
#include <vector>

struct coroutine_pool;
extern coroutine_pool *g_pool;

/**
 * @brief 协程池
 * 保存所有需要同步执行的协程函数。并可以进行并行/串行执行。
 */
struct coroutine_pool {
  std::vector<basic_context *> coroutines;
  int context_id;

  // whether run in threads or coroutines
  bool is_parallel;

  ~coroutine_pool() {
    for (auto context : coroutines) {
      delete context;
    }
  }

  // add coroutine to pool
  // ...表示可变参数模板，可以接受多个参数
  template <typename F, typename... Args>
  void new_coroutine(F f, Args... args) {
    coroutines.push_back(new coroutine_context(f, args...));
  }

  /**
   * @brief 以并行多线程的方式执行所有协程函数
   */
  void parallel_execute_all() {
    g_pool = this;
    is_parallel = true;
    // 线程数组
    std::vector<std::thread> threads;
    // 为每一个协程创建一个线程，然后并行执行
    for (auto p : coroutines) {
      threads.emplace_back([p]() { p->run(); });
    }

    for (auto &thread : threads) {
      thread.join();
    }
  }

  /**
   * @brief 以协程执行的方式串行并同时执行所有协程函数
   * 所有协程共享同一个线程串行执行
   * TODO: Task 1, Task 2
   * 在 Task 1 中，我们不需要考虑协程的 ready
   * 属性，即可以采用轮询的方式挑选一个未完成执行的协程函数进行继续执行的操作。
   * 在 Task 2 中，我们需要考虑 sleep 带来的 ready
   * 属性，需要对协程函数进行过滤，选择 ready 的协程函数进行执行。
   *
   * 当所有协程函数都执行完毕后，退出该函数。
   */
  void serial_execute_all() {
    is_parallel = false;
    g_pool = this;
    // 轮询调用协程，直到所有协程都执行完毕（先不考虑是否ready）
    // 总的协程数
    int cnt = coroutines.size();
    // 只要还有协程，就要一直轮询执行
    while(cnt > 0) {
      for(int i = 0; i < coroutines.size(); i++) {
        auto context = coroutines[i];
        // 如果已经结束了，则退出
        if(context->finished) {
          continue;
        }
        // 如果不是就绪状态，检查是否超时
        // 如果超时了，则设置为就绪状态，否则continue
        if(!context->ready) {
          if(context->ready_func()) {
            context->ready = true;
          }
          else {
            continue;
          }
        }
        //经过前面的过滤，此处可以确保当前的协程未结束并且就绪
        //! 注意要设置当前正在执行的协程id
        context_id = i;
        // 使用resume，会切换到协程并且执行
        context->resume();
        // 如果结束了，则数量减1
        if(context->finished) {
          cnt--;
        }
        
      }
    }


    for (auto context : coroutines) {
      delete context;
    }
    coroutines.clear();
  }
};
