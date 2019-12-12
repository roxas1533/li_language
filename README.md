# li_language
| Brainfuck | token | Description |
----|----|---- 
| > | il | Move the pointer to the right |
| < | li | Move the pointer to the left |
| + | ii | Increment the memory cell under the pointer |
| - | \|\| | Decrement the memory cell under the pointer |
| , | il | Input a character and store it in the cell at the pointer |
| . | li | Output the character signified by the cell at the pointer |
| \[ | \|l | label |
| ] | l\| | Jump past the matching \|l if the cell under the pointer is 0 |

Brainfuck's '\[ ]' means "while" but li's '\|l l\|' means "do-while" 
