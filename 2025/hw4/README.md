# 41247044S

p1: For this problem, I believe I implemented it enough, but there are slight timing issues that I'm not quite sure if they are relevant to the correctness. To this, there's also the fact that the default no flag, `./hw0401` does not output anything heavily relevant. I suspect that its because it measures every cycle by its kernal processes which tend to be idle on my WSL. Also this implement also accepts `--count [count]`.

p2: Cool, also the API endpoints have been updated to need the `/currencies/` subdirectory in the URL. Also in my implement, I manually calculated each month. Classic TA's...

p3: The order is questionable here. Since it orders by alphabetical order, but since the TA's haven't said anything I will assume this does not matter in the slightest. If so, you should've said so, besides, by unpacking order, that is by directly appending, it looks messier.

**p4: Alright so please follow and listen to what I say, you MUST use `-gdwarf-2 -gz=none` to compile whatever you want to feed into the program. Here, it parses DWARF-2 but all proceeding versions are weirdly parsed. It also doesn't parse well subprograms. It will work for both GCC and Clang AS LONG AS you use `-gdwarf-2 -gz=none`**

p5: Fun research! I implemented my own `lsplus` command, but I decided to do this instead as its boring to just paste code, instead this idea is sorta cooler ig, chatgpt helped but comparing it to the official implement, you can see this is just an EDUCATIONAL version. The real ls is a lot more robust.