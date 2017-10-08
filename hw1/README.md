# Domácí úkol 1 pro PB173

## Sestavení a spuštění
```
make
make run
```

Spuštění trvá cca 30 sekund.

## Popis
Obsahuje implementaci hashovací tabulky pomocí linked list a linear probing.

## Výsledky
Implementace pomocí linked list je výkonnostně obecně velmi podobná std::unordered_set.
Značný rozdíl je v rychlosti vyhledávání čísel, moje implementace je v tomto případě až 2x rychlejší.
Naopak při práci s řetězci je moje implementace v případě vkládání a vyhledávání mírně pomalejší.

Implementace pomocí linear probing je v případě čísel o mnoho rychlejší oproti std::unordered_set. Test vkládání náhodných čísel
ukazuje na trojnásobnou rychlost, test vyhledávání náhodných čísel na dvojnásobnou rychlost.
Vkládání řetězců je rychlejší o cca 30%, hledání řetězců je podobně rychlé.

std::set je oproti std::unordered_set pomalejší jak ve vyhledávání tak vkládání.
