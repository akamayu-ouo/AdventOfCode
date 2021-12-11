#!/usr/bin/dyalog -script
⍝)load buildse
⍝BUILD_SESSION 'UK'
⍝]box on -style=min

fname ← 'day10.ex'
input ← ' '(≠⊆⊢)¨⊃⎕NGET fname 1

A ← '([{<'
B ← ')]}>'

elide ← ∊((~¯1∘⌽∨⊢)(⊃(A,¨B)∨.⍷⊂))⊆∊

min ← elide⍣≡¨input

⍝  Part 1
⎕ ← +/(3 57 1197 25137+.×B∊(⊃B∩⍨⊢))¨min
⍝  Part 2
⎕ ← ((⌈2÷⍨≢)⌷(⊂∘⍋⌷⊢))5⊥¨(⍳4)+.×A∘.=⌽¨⊃,/((~↓(1∊B∊⊢)¨⊢)⊆⊢)min

)OFF
