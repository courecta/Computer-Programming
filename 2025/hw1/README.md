# 41247044S

pretty nice homework, first 3 problems were straightforward. No way p4 is going to be fine since Im too lazy to learn how that game works, but p5 might be better.

p1: implemented NULL checking, since using -Wnonnull actually reveals that the C standard implements do not check for NULL pointers

```
// that is, this is effectively not allowed
strchr(NULL, 'a');

// returns a pointer instead of a string, which means trying to print with %s throws a compiler error
```

p2: Should be according to how python's `.split()` function works (hopefully...)

p3: tested rigorously but had issues on normalization for the format, hopefully works

p4: RIP

p5: Passes the given examples, but some indentations might not work well, I wonder if this can bring partial points or not...

p6: fun to research as always!