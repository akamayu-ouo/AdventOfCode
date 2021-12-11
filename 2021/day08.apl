#!/usr/bin/dyalog -script
scores ← 42 17 34 39 30 37 41 25 49 45

fname ← 'day8.in'
input ← ⊃⎕NGET fname 1

bits     ← ('abcdefg'∊⊢)¨¨' '(≠⊆⊢)¨ input
tonumber ← (10*3 2 1 0)+.×⊢

freq   ← ↑(+/10↑⊢)¨
outs   ← 11↓¨⊢

digits ← (0,⍳9)+.×scores∘.=(⊃freq,.{⊂(⍺+.×⊢)¨⍵}outs) bits

⎕ ← 'part1 :',part1 ← +/2 3 4 7∊⍨∊(+/⊢)¨¨outs bits
⎕ ← 'part2 :',part2 ← +/tonumber¨digits

)OFF
