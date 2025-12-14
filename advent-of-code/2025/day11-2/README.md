# day11-2

反向建图, 终点算一, 逐个移除入度为0的节点, 节点被移除时将值添加到子节点上, 直到 'svr' 节点被移除.

每个节点要维护的不再是一个value, 而是

+ num, visited dac, visited fft

的tuple, 初始状态为 (1, false, false)

如果当前节点为 dac, 则将当前状态的visited dec全部修改为 true, 添加到子结点上
如果当前节点为 fft, 则将当前状态的visited fft全部修改为 true, 添加到子结点上

当移除 'svr' 节点时, 输出 num 即可.

更进一步地说, 可以将一个tuple修改为一个 std::<array, 4>

+ [0] => both false
+ [1] => visited dac only
+ [2] => visited fft only
+ [3] => both true

核心算法不变.

1. 在到达dac 时, 只会出现 0,2, 因此只要将0加到1上, 2加到3上即可
2. 在到达fft之前, 只会出现0,1 将0映射到2, 1映射到3
3. 对于普通的, 只需要将4个值分别加到子节点上即可
