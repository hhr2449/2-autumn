1. 单条指令用时$t_0 = 10^{-8}s$
2. 时间限制$t_{limit} = 2t_{max}$，则$t_{max} = \frac{t_{limit}}{2}$
3. 设c(n)为输入规模为n的时候执行的指令条数，那么有：$t_{max} = c(n_{max})t_0$，则$t_{limit} = 2c(n_{max})t_0$

# 2A

$t_{limit} = 1s$，则$2\times 10n^2\times 10^{-8} = 1，则n^2 = 5\times 10^6$

$n = \sqrt{5 \times 10^6} \approx 2236.06$，由于条件要求不超过，故设置的最大输入规模应该为2236

# 2B

$c(n)=20nlog_2 n$，则有$2\times 20nlog_2n\times 10^{-8} = 1，则nlog_2n = 2.5\times 10^6$

$n \approx 145746.2$，则n应该取145746



# 3A

## solution_1.cpp：

* bug1：sum变量没有在每次查询之前进行重置，导致后面的运行结果会收到前面的结果的干扰

  发现方式：

  1. 静态检查：观察代码并且复述解题逻辑，容易发现每次循环之前应该重置sum
  2. 观察输出结果：输入测试样例，发现第一个输出结果正确，之后的输出结果显著大于答案，容易想到应该是由于每次查询的初始化导致错误

* bug2：数据类型选择错误

  根据题目给出的数据范围，sum值最大可能到达4e11，而程序中使用了int型的sum，最大只能储存约2e9大小的数据，有很大概率会导致溢出

  发现方式：

  1. 静态检查
  2. ai辅助

## solution_2.cpp:

- bug1：`sum`变量未在每次查询前重置（同`solution_1`）

- bug2：数据类型选择错误（同solution_1）

- bug3：前缀和数组定义为`rowsum[i][j]`代表第i行的[0, j]列的和，而代码中使用了`rowsum[x+j][y+b] - rowsum[x+j][y]`这样就没有包含第y列，但是多包含了第y+b列，导致出错

  发现方式：

  - 静态检查：前缀和公式与子矩阵范围不匹配。
  - 输出调试：打印每次累加的行和，发现与手动计算的子矩阵列范围不符。
  - 对拍：之前已经调试得到了正确的solution_1，通过solution_1正确输出来进行比较

## AIGC 工具调试能力

- 优点：
  * 可以快速识别一些细节错误，比如语法错误、循环变量初始化、索引错误等
  * 对于一些经典的算法调式能力很强，比如前缀和
  * 对于一些常见的错误调试能力强，比如索引错误，循环变量，数据溢出等
  * 对于一些符合编程范式，比较规范的代码可以快速匹配已有的数据来发现错误
- 局限性
  - 对于一些比较复杂的逻辑调试能力较弱，容易出现逻辑混乱
  - 对于具有较长上下文的代码理解能力比较弱，难以结合具体场景进行调试
  - 对于极端情况，一些边界条件容易忽略
  - 对于一些非典型的错误，比如一些经过多次修改和补丁的代码调试能力较弱
  - 对于一些运行时错误调试能力较弱

## 3B

在编译时加上-g选项来保存调试信息，如`g++ solution_1.cpp -o solution_1 -g `

1. 启动调试：gdb ./solution_1
2. 打断点：break <行号>/<函数名>，比如使用break main在main函数入口处打断点，使用break 15在第15行打断点
3. 运行程序：使用run开始运行
4. 单步执行：next是逐过程，step是逐语句
5. 查看变量：print sum打印sum值，watch sum监控sum值变化
6. continue：运行到下一个断点

## 3C

`srand(time(0))`

`srand(seed)`用于设定产生随机数的种子，不同的种子对应不同的随机数序列

time(0)则是用于获取当前的系统时间

两者结合起来可以实现在不同的时间运行程序都会给随机数生成器不同的种子，从而实现每次运行程序都会产生不同的测试数据

## 3D

1. system("g++ rand_input.cpp -o rand_input");

   对rand_input.cpp进行编译，输出结果为rand_input，这里的rand_input是用于随机生成测试数据的程序

2. system("g++ check_input.cpp -o check_input");

   对check_input.cpp进行编译，输出结果为check_input，这里的check_input是用于检查生成的数据是否合法的程序

3. system("g++ solution_1.cpp -o solution_1");

   对solution_1.cpp进行编译，输出结果为solution_1_input

4. system("g++ solution_2.cpp -o solution_2");

   对solution_2.cpp进行编译，输出结果为solution_2

5. system("./rand_input > rand.in");

   执行rand_input，生成随机测试数据，并且将输出内容重定向到rand.in中

6. system("./check_input < rand.in")!=0

   将rand.in中的数据输入check_in中进行验证，check_input中使用了assert来判断，如果输入内容不合法则会异常退出，main函数的返回值是非零值，结束循环，否则return 0 继续

7. system("./solution_1 < rand.in > 1.out");

   system("./solution_2 < rand.in > 2.out");

   执行solution_1和2，将rand.in作为输入数据，将输出内容重定向输出到1.out和2.out中

8. system("diff 1.out 2.out")!=0

   比较两个程序的输出是否一致，若不一致则终止循环并提示

## 3E

- 最大可能值
  - 单个元素最大绝对值：`1e5`
  - 当全选矩阵的时候取到子矩阵最大规模：`2000×2000 = 4e6`
  - 最大查询次数：`1e5`
  - 总和上限：`1e5 × 4e6 = 4e11`
  - 如果按照给出的solution中错误的代码，每次没有重置sum，那么总和上限则是：`1e5 × 4e6 × 1e5 = 4e16`
- 数据类型：int类型为32位，最大值为2147483647，约为2e9，远远小于总和上线，所以需使用64位整数long long存储



## 4A

思路：使用二维前缀和数组

1. 预处理，建立前缀和数组

   定义二维数组 `prefixsum[i][j]` 表示从矩阵左上角 `(1,1)` 到 `(i,j)` 的矩形区域内所有元素的和。
   构建公式`prefixsum[i][j]=matrix[i][j]+prefixsum[i−1][j]+prefixsum[i][j−1]−prefixsum[i−1][j−1]`

   (这里0行0列都存0，方便边界处理)

2. 查询计算：

   对于起点为 `(x,y)`、大小为 `(a,b)` 的子矩阵（终点为 `x+a-1, y+b-1`），其和为：`sum=prefixsum[x+a−1][y+b−1]−prefixsum[x−1][y+b−1]−prefixsum[x+a−1][y−1]+prefixsum[x−1][y−1]`,时间复杂度为常数级别



## 4B

1. test1

   原数组大，查询次数多，子数组大

   | n     | m     | p       | a    | b    |
   | ----- | ----- | ------- | ---- | ---- |
   | 10000 | 10000 | 1000000 | 8000 | 8000 |

   测试结果：

   |            | 1      | 2      | 3      | 平均   |
   | ---------- | ------ | ------ | ------ | ------ |
   | solution_1 | 1653ms | 1534ms | 1609ms | 1598ms |
   | solution_2 | 1945ms | 1897ms | 1808ms | 1883ms |
   | solution_3 | 584ms  | 545ms  | 551ms  | 560ms  |

   

2. test2

   原数组大，查询次数少，子数组小

   | n    | m     | p    | a    | b    |
   | ---- | ----- | ---- | ---- | ---- |
   | 2000 | 20000 | 1    | 10   | 10   |

   测试结果：

   |            | 1      | 2     | 3     | 平均   |
   | ---------- | ------ | ----- | ----- | ------ |
   | solution_1 | 828ms  | 840ms | 812ms | 827ms  |
   | solution_2 | 870ms  | 875ms | 861ms | 869ms  |
   | solution_3 | 1034ms | 997ms | 972ms | 1001ms |

3. test3

   原数组小，查询次数多，子数组行大列小

   | n    | m    | p       | a    | b    |
   | ---- | ---- | ------- | ---- | ---- |
   | 2000 | 20   | 1000000 | 1900 | 10   |

   测试结果：

   |            | 1       | 2       | 3       | 平均    |
   | ---------- | ------- | ------- | ------- | ------- |
   | solution_1 | 27271ms | 27186ms | 26002ms | 26820ms |
   | solution_2 | 3527ms  | 3483ms  | 3380ms  | 3463ms  |
   | solution_3 | 101ms   | 98ms    | 101ms   | 100ms   |



## 4c

| 程序       | 预处理时间复杂度 | 单次查询时间复杂度 | 总时间复杂度（*n*,*m*为矩阵规模，*a*,*b*为子矩阵规模，*p*为查询次数） |
| ---------- | ---------------- | ------------------ | ------------------------------------------------------------ |
| solution_1 | *O*(1)           | O(ab)              | *O*(abp)                                                     |
| solution_2 | O(nm)            | O(a)               | O(nm+ap)                                                     |
| solution_3 | O(nm)            | O(1)               | O(nm+p)                                                      |



solution_1在预处理的时间复杂度低，2，3在预处理的时间复杂度较高

solution_1查询的时间复杂度较高，2相对较低，但是对子数组列数敏感，3查询可在常数时间内完成

solution_3优势条件：

1. 原数组大小：当原数组大小较小时，预处理所用时间较少，后续查询成本可以摊薄预处理成本
2. 查询次数：当查询次数极大时，由于3的查询效率高于1和2，所以当查询次数多的时候solution_3有明显优势（当子数组的行数较少时，2，3的差距较小）
3. 子数组大小：当子数组规模大时，3有优势。子数组规模对3的查询没有影响，对于1，子数组规模对查询成本影响较大，对于2，子数组的行数影响较大



## 5A

网页代码：

```html
<!DOCTYPE html>
<html lang="zh-CN">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>二维前缀和算法可视化 (solution_3.cpp)</title>
    <script src="https://cdn.tailwindcss.com"></script>
    <style>
        /* 矩阵单元格样式 */
        .matrix-cell {
            width: 40px;
            height: 40px;
            text-align: center;
            border: 1px solid #333;
            transition: all 0.3s ease;
        }
        /* 前缀和矩阵高亮（计算中） */
        .prefix-highlight {
            background-color: #93c5fd;
            font-weight: bold;
        }
        /* 查询子矩阵高亮 */
        .query-highlight {
            background-color: #fbbf24;
        }
        /* 步骤指示器样式 */
        .step-active {
            background-color: #2563eb;
            color: white;
            font-weight: bold;
        }
    </style>
</head>
<body class="bg-gray-50 p-5">
    <div class="max-w-7xl mx-auto">
        <h1 class="text-3xl font-bold text-center text-gray-800 mb-8">二维前缀和算法可视化 (solution_3.cpp)</h1>
        
        <!-- 控制区：参数配置与操作按钮 -->
        <div class="bg-white rounded-lg shadow-md p-5 mb-6">
            <h2 class="text-xl font-semibold text-gray-700 mb-4">1. 配置参数</h2>
            <div class="grid grid-cols-1 md:grid-cols-4 gap-4 mb-4">
                <div>
                    <label class="block text-sm font-medium text-gray-600 mb-1">矩阵行数 n</label>
                    <input type="number" id="n" value="4" min="2" max="10" class="w-full p-2 border border-gray-300 rounded">
                </div>
                <div>
                    <label class="block text-sm font-medium text-gray-600 mb-1">矩阵列数 m</label>
                    <input type="number" id="m" value="4" min="2" max="10" class="w-full p-2 border border-gray-300 rounded">
                </div>
                <div>
                    <label class="block text-sm font-medium text-gray-600 mb-1">查询次数 q</label>
                    <input type="number" id="q" value="2" min="1" max="5" class="w-full p-2 border border-gray-300 rounded">
                </div>
                <div class="flex items-end">
                    <button id="generate-btn" class="w-full bg-blue-500 text-white py-2 px-4 rounded hover:bg-blue-600 transition">生成矩阵</button>
                </div>
            </div>

            <h2 class="text-xl font-semibold text-gray-700 mb-4">2. 算法步骤控制</h2>
            <div class="flex flex-wrap gap-3 mb-4">
                <button id="preprocess-btn" class="bg-green-500 text-white py-2 px-4 rounded hover:bg-green-600 transition" disabled>1. 计算二维前缀和</button>
                <button id="next-query-btn" class="bg-purple-500 text-white py-2 px-4 rounded hover:bg-purple-600 transition" disabled>2. 执行下一次查询</button>
                <button id="reset-btn" class="bg-red-500 text-white py-2 px-4 rounded hover:bg-red-600 transition">重置</button>
            </div>

            <!-- 查询参数配置（动态显示） -->
            <div id="query-config" class="hidden bg-gray-50 p-4 rounded mb-4">
                <h3 class="text-lg font-medium text-gray-700 mb-2">当前查询参数（1-based）</h3>
                <div class="grid grid-cols-1 md:grid-cols-4 gap-4">
                    <div>
                        <label class="block text-sm font-medium text-gray-600 mb-1">起始行 x</label>
                        <input type="number" id="query-x" min="1" class="w-full p-2 border border-gray-300 rounded">
                    </div>
                    <div>
                        <label class="block text-sm font-medium text-gray-600 mb-1">起始列 y</label>
                        <input type="number" id="query-y" min="1" class="w-full p-2 border border-gray-300 rounded">
                    </div>
                    <div>
                        <label class="block text-sm font-medium text-gray-600 mb-1">子矩阵高度 a</label>
                        <input type="number" id="query-a" min="1" class="w-full p-2 border border-gray-300 rounded">
                    </div>
                    <div>
                        <label class="block text-sm font-medium text-gray-600 mb-1">子矩阵宽度 b</label>
                        <input type="number" id="query-b" min="1" class="w-full p-2 border border-gray-300 rounded">
                    </div>
                </div>
                <button id="exec-query-btn" class="mt-3 bg-purple-500 text-white py-2 px-4 rounded hover:bg-purple-600 transition">执行查询</button>
            </div>
        </div>

        <!-- 可视化区：原始矩阵与前缀和矩阵 -->
        <div class="grid grid-cols-1 lg:grid-cols-2 gap-6 mb-6">
            <!-- 原始矩阵 -->
            <div class="bg-white rounded-lg shadow-md p-5">
                <h2 class="text-xl font-semibold text-gray-700 mb-4">原始矩阵（v(i,j)）</h2>
                <div id="original-matrix" class="flex flex-col items-center gap-1"></div>
            </div>
            <!-- 前缀和矩阵 -->
            <div class="bg-white rounded-lg shadow-md p-5">
                <h2 class="text-xl font-semibold text-gray-700 mb-4">二维前缀和矩阵（prefixsum[i][j]）</h2>
                <div class="text-sm text-gray-500 mb-2">公式：prefixsum[i][j] = v[i][j] + prefixsum[i-1][j] + prefixsum[i][j-1] - prefixsum[i-1][j-1]</div>
                <div id="prefix-matrix" class="flex flex-col items-center gap-1"></div>
            </div>
        </div>

        <!-- 结果区：查询记录与算法说明 -->
        <div class="bg-white rounded-lg shadow-md p-5">
            <h2 class="text-xl font-semibold text-gray-700 mb-4">3. 查询结果记录</h2>
            <div id="query-results" class="space-y-3 mb-6"></div>
            
            <h2 class="text-xl font-semibold text-gray-700 mb-4">4. 算法说明（对应 solution_3.cpp）</h2>
            <div class="text-gray-700 space-y-2">
                <p><span class="font-bold">预处理阶段</span>：时间复杂度 O(n×m)，构建二维前缀和矩阵，将子矩阵求和的计算成本转移到初始化阶段。</p>
                <p><span class="font-bold">查询阶段</span>：时间复杂度 O(1)，通过前缀和公式计算子矩阵和：</p>
                <p class="pl-5">sum = prefixsum[end_x][end_y] - prefixsum[x-1][end_y] - prefixsum[end_x][y-1] + prefixsum[x-1][y-1]</p>
                <p>其中，end_x = x + a - 1（子矩阵右下角行），end_y = y + b - 1（子矩阵右下角列）。</p>
            </div>
        </div>
    </div>

    <script>
        // 全局变量：存储矩阵数据、查询状态
        let originalMatrix = [];    // 原始矩阵
        let prefixMatrix = [];      // 前缀和矩阵
        let currentQuery = 0;       // 当前执行的查询序号（从1开始）
        let totalQueries = 0;       // 总查询次数

        // DOM 元素获取
        const nInput = document.getElementById('n');
        const mInput = document.getElementById('m');
        const qInput = document.getElementById('q');
        const generateBtn = document.getElementById('generate-btn');
        const preprocessBtn = document.getElementById('preprocess-btn');
        const nextQueryBtn = document.getElementById('next-query-btn');
        const resetBtn = document.getElementById('reset-btn');
        const queryConfig = document.getElementById('query-config');
        const queryX = document.getElementById('query-x');
        const queryY = document.getElementById('query-y');
        const queryA = document.getElementById('query-a');
        const queryB = document.getElementById('query-b');
        const execQueryBtn = document.getElementById('exec-query-btn');
        const originalMatrixEl = document.getElementById('original-matrix');
        const prefixMatrixEl = document.getElementById('prefix-matrix');
        const queryResultsEl = document.getElementById('query-results');

        // 1. 生成随机原始矩阵
        generateBtn.addEventListener('click', () => {
            const n = parseInt(nInput.value);
            const m = parseInt(mInput.value);
            totalQueries = parseInt(qInput.value);
            
            // 重置状态
            originalMatrix = [];
            prefixMatrix = [];
            currentQuery = 0;
            originalMatrixEl.innerHTML = '';
            prefixMatrixEl.innerHTML = '';
            queryResultsEl.innerHTML = '';
            queryConfig.classList.add('hidden');
            
            // 生成随机矩阵（值范围 0~20，便于可视化）
            for (let i = 1; i <= n; i++) {
                const row = [];
                for (let j = 1; j <= m; j++) {
                    row.push(Math.floor(Math.random() * 21)); // 0~20
                }
                originalMatrix.push(row);
            }

            // 渲染原始矩阵（1-based 显示）
            for (let i = 0; i < originalMatrix.length; i++) {
                const rowEl = document.createElement('div');
                rowEl.className = 'flex gap-1';
                for (let j = 0; j < originalMatrix[i].length; j++) {
                    const cellEl = document.createElement('div');
                    cellEl.className = 'matrix-cell flex items-center justify-center';
                    cellEl.textContent = originalMatrix[i][j];
                    cellEl.id = `orig-${i+1}-${j+1}`; // 1-based 编号
                    rowEl.appendChild(cellEl);
                }
                originalMatrixEl.appendChild(rowEl);
            }

            // 启用预处理按钮
            preprocessBtn.disabled = false;
            nextQueryBtn.disabled = true;
        });

        // 2. 计算并渲染二维前缀和矩阵（带步骤高亮）
        preprocessBtn.addEventListener('click', async () => {
            const n = originalMatrix.length;
            const m = originalMatrix[0].length;
            
            // 初始化前缀和矩阵（0行0列为0，便于计算）
            prefixMatrix = Array(n + 1).fill(0).map(() => Array(m + 1).fill(0));
            
            // 清空前缀和矩阵容器
            prefixMatrixEl.innerHTML = '';
            
            // 渲染 0 行（辅助行，灰色显示）
            const zeroRow = document.createElement('div');
            zeroRow.className = 'flex gap-1';
            zeroRow.appendChild(document.createElement('div')); // 空单元格（对应0行0列）
            for (let j = 1; j <= m; j++) {
                const cellEl = document.createElement('div');
                cellEl.className = 'matrix-cell flex items-center justify-center text-gray-400';
                cellEl.textContent = '0';
                zeroRow.appendChild(cellEl);
            }
            prefixMatrixEl.appendChild(zeroRow);
            
            // 逐行逐列计算前缀和（1-based），并高亮显示计算过程
            for (let i = 1; i <= n; i++) {
                const rowEl = document.createElement('div');
                rowEl.className = 'flex gap-1';
                
                // 渲染 0 列（辅助列，灰色显示）
                const zeroCell = document.createElement('div');
                zeroCell.className = 'matrix-cell flex items-center justify-center text-gray-400';
                zeroCell.textContent = '0';
                rowEl.appendChild(zeroCell);
                
                // 渲染当前行的前缀和单元格
                for (let j = 1; j <= m; j++) {
                    const cellEl = document.createElement('div');
                    cellEl.className = 'matrix-cell flex items-center justify-center';
                    cellEl.id = `prefix-${i}-${j}`;
                    rowEl.appendChild(cellEl);
                }
                prefixMatrixEl.appendChild(rowEl);
                
                // 逐列计算并更新（添加延迟，便于观察）
                for (let j = 1; j <= m; j++) {
                    const v = originalMatrix[i-1][j-1]; // 原始矩阵是0-based数组
                    const up = prefixMatrix[i-1][j];
                    const left = prefixMatrix[i][j-1];
                    const upLeft = prefixMatrix[i-1][j-1];
                    prefixMatrix[i][j] = v + up + left - upLeft;
                    
                    // 高亮当前计算的单元格
                    const currentCell = document.getElementById(`prefix-${i}-${j}`);
                    currentCell.classList.add('prefix-highlight');
                    await delay(500); // 延迟500ms，观察计算步骤
                    
                    // 更新单元格值，并移除高亮
                    currentCell.textContent = prefixMatrix[i][j];
                    currentCell.classList.remove('prefix-highlight');
                }
            }

            // 预处理完成，启用查询按钮
            preprocessBtn.disabled = true;
            nextQueryBtn.disabled = false;
        });

        // 3. 执行下一次查询（显示配置面板）
        nextQueryBtn.addEventListener('click', () => {
            currentQuery++;
            if (currentQuery > totalQueries) {
                alert('已完成所有查询！');
                nextQueryBtn.disabled = true;
                return;
            }
            
            // 显示查询配置面板，并设置参数范围（防止越界）
            queryConfig.classList.remove('hidden');
            const n = originalMatrix.length;
            const m = originalMatrix[0].length;
            queryX.max = n;
            queryY.max = m;
            queryX.value = 1; // 默认起始行1
            queryY.value = 1; // 默认起始列1
            queryA.max = n - queryX.value + 1;
            queryB.max = m - queryY.value + 1;
            queryA.value = 2; // 默认高度2
            queryB.value = 2; // 默认宽度2

            // 监听起始坐标变化，更新子矩阵大小的最大值
            queryX.addEventListener('input', updateQueryMax);
            queryY.addEventListener('input', updateQueryMax);
        });

        // 更新查询参数的最大值（防止子矩阵越界）
        function updateQueryMax() {
            const n = originalMatrix.length;
            const m = originalMatrix[0].length;
            const x = parseInt(queryX.value);
            const y = parseInt(queryY.value);
            queryA.max = n - x + 1;
            queryB.max = m - y + 1;
        }

        // 4. 执行查询（计算子矩阵和，并高亮显示）
        execQueryBtn.addEventListener('click', () => {
            const x = parseInt(queryX.value);
            const y = parseInt(queryY.value);
            const a = parseInt(queryA.value);
            const b = parseInt(queryB.value);
            const endX = x + a - 1;
            const endY = y + b - 1;
            const n = originalMatrix.length;
            const m = originalMatrix[0].length;

            // 校验参数合法性
            if (endX > n || endY > m) {
                alert('查询参数越界！子矩阵超出原始矩阵范围');
                return;
            }

            // 计算子矩阵和（O(1) 公式）
            const sum = prefixMatrix[endX][endY] 
                      - prefixMatrix[x-1][endY] 
                      - prefixMatrix[endX][y-1] 
                      + prefixMatrix[x-1][y-1];

            // 高亮原始矩阵中的查询子矩阵
            for (let i = x; i <= endX; i++) {
                for (let j = y; j <= endY; j++) {
                    const cell = document.getElementById(`orig-${i}-${j}`);
                    cell.classList.add('query-highlight');
                }
            }

            // 高亮前缀和矩阵中的关键位置（公式用到的4个点）
            const highlightPoints = [
                [endX, endY],
                [x-1, endY],
                [endX, y-1],
                [x-1, y-1]
            ];
            highlightPoints.forEach(([i, j]) => {
                if (i >= 0 && j >= 0) { // 0行0列也需要高亮
                    const cell = document.getElementById(`prefix-${i}-${j}`) || Array.from(prefixMatrixEl.children[i].children)[j+1];
                    if (cell) cell.classList.add('query-highlight');
                }
            });

            // 添加查询结果到记录
            const resultItem = document.createElement('div');
            resultItem.className = 'p-3 border border-gray-200 rounded';
            resultItem.innerHTML = `
                <div class="font-medium">查询 ${currentQuery}/${totalQueries}</div>
                <div>参数：起始坐标 (${x}, ${y})，子矩阵大小 (${a}, ${b})，右下角坐标 (${endX}, ${endY})</div>
                <div>子矩阵和：${sum}</div>
                <div class="text-sm text-gray-500">计算过程：prefixsum[${endX}][${endY}] - prefixsum[${x-1}][${endY}] - prefixsum[${endX}][${y-1}] + prefixsum[${x-1}][${y-1}] = ${prefixMatrix[endX][endY]} - ${prefixMatrix[x-1][endY]} - ${prefixMatrix[endX][y-1]} + ${prefixMatrix[x-1][y-1]} = ${sum}</div>
            `;
            queryResultsEl.appendChild(resultItem);

            // 隐藏查询配置面板，更新按钮状态
            queryConfig.classList.add('hidden');
            if (currentQuery >= totalQueries) {
                nextQueryBtn.disabled = true;
            }
        });

        // 5. 重置所有状态
        resetBtn.addEventListener('click', () => {
            location.reload(); // 简单重置：刷新页面
        });

        // 工具函数：延迟函数（用于步骤可视化）
        function delay(ms) {
            return new Promise(resolve => setTimeout(resolve, ms));
        }
    </script>
</body>
</html>
```

截图：

1. 生成矩阵：

   ![image-20250918103218106](https://raw.githubusercontent.com/hhr2449/pictureBed/main/img/image-20250918103218106.png)

2. 计算前缀和

   ![image-20250918103312504](https://raw.githubusercontent.com/hhr2449/pictureBed/main/img/image-20250918103312504.png)

3. 执行查询

   ![image-20250918103343344](https://raw.githubusercontent.com/hhr2449/pictureBed/main/img/image-20250918103343344.png)
