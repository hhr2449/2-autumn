## data1.in, n=m=1000

|              | linear_probe | quadratic_probe | overflow_strategy |
| ------------ | ------------ | --------------- | ----------------- |
| bad_hashing  | 0.00681388s  | 0.00569284s     | 0.00462075s       |
| good_hashing | 0.00519091s  | 0.00431629s     | 0.00488581s       |

## data2.in, n=50000,m=25000

|              | linear_probe | quadratic_probe | overflow_strategy |
| ------------ | ------------ | --------------- | ----------------- |
| bad_hashing  | 4.47764s     | 0.0430497s      | 1.36263s          |
| good_hashing | 0.01825s     | 0.0185516s      | 0.0297754s        |

## data3.in, n=100000,m=100000

|              | linear_probe | quadratic_probe | overflow_strategy |
| ------------ | ------------ | --------------- | ----------------- |
| bad_hashing  | 18.8461s     | 0.124682s       | 6.14423s          |
| good_hashing | 0.0295618s   | 0.0297596s      | 0.161079s         |

​	