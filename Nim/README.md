# AppleNim  
 This program demonstrates how minimax AI algorithm can be used for finding the best move in a Nim game.
 
## MiniMax with Alpha Beta Pruning wins over naive MiniMax  
### Actual Test Data  

| Apples | Computer | Human | MethodA Nodes | MethodA Max Depth | MethodB Nodes | MethodB Max Depth |
|--------|----------|-------|---------------|-------------------|---------------|-------------------|
| 26     |          | 3     | 3,171,045     | 22                | 7,059         | 10                |
| 23     | 2        |       |               |                   |               |                   |
| 21     |          | 4     |               |                   |               |                   |
| 17     | 1        |       | 61,822        | 16                | 856           | 7                 |
| 16     |          | 4     |               |                   |               |                   |
| 12     | 1        |       | 2,322         | 11                | 179           | 5                 |
| 11     |          | 3     |               |                   |               |                   |
| 8      | 2        |       | 167           | 7                 | 50            | 4                 |
| 6      |          | 2     |               |                   |               |                   |
| 4      | 3        |       | 11            | 3                 | 10            | 3                 |
| 1      |          | 1     |               |                   |               |                   |

**MethodA** = MiniMax | **MethodB** = MiniMax + Alpha-Beta Pruning


