# 41247044S

p1: For this problem, I believe I implemented it enough, but there are slight timing issues that I'm not quite sure if they are relevant to the correctness. To this, there's also the fact that the default no flag, `./hw0401` does not output anything heavily relevant. I suspect that its because it measures every cycle by its kernal processes which tend to be idle on my WSL. Also this implement also accepts `--count [count]`.

p2: Cool, also the API endpoints have been updated to need the `/currencies/` subdirectory in the URL. Also in my implement, I manually calculated each month. Classic TA's... [Remember to install libcurl on your machine!]

p3: The order is questionable here. Since it orders by alphabetical order, but since the TA's haven't said anything I will assume this does not matter in the slightest. If so, you should've said so, besides, by unpacking order, that is by directly appending, it looks messier. [Remember to install liblzma on your machine!]

p4: Can handle DWARF-5, so pleas use `-g`, but other versions should pass? like version 2 to 4. [Remember to install libelf on your machine!]

p5: Fun research! I implemented my own `lsplus` command, but I decided to do this instead as its boring to just paste code, instead this idea is sorta cooler ig, chatgpt helped but comparing it to the official implement, you can see this is just an EDUCATIONAL version. The real ls is a lot more robust.