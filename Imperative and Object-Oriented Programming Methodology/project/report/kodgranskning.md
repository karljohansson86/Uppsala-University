
src/h_init.c
============

## Struktur
Koden ser bra ut och är konsekvent. h_init är ganska stor, men det är acceptabelt då den gör tydliga och repetiva grejer.
Iterering av pages sker manuellt och likadant flera gånger, detta bör extraheras och göras via en iterator istället. h_alloc_gc bör också brytas ner i underfunktioner.

## Variabler
Variabelnamnen är ganska lätta att förstå (det hjälper att många funktioner brutits ner i underfunktioner) och variablerna som finns behövs. 
## Loopar och villkorssatser
Det ser allmänt bra ut, is_space_newspace, is_addr_promoted samt alloc-funktioner kan lägga till variabler för ökad läsbarhet.

## Defensiv programmering
Många argument testas inte, istället antas att callers följer datatypens konvention och invariants.


src/object.c
========

Koden är rätt trevlig, funktionsdeklarationer och tillhörande dokumentation (i doxygen-stil) är snyggt separerad från implementationerna.

## Struktur
Koden ser bra stilistiskt och formateringsmässigt. 
Det finns några "magiska nummer", men de är noterade i kommentarer.
Det finns ett block av kod med "o_is_numeric" som skulle kunna extraheras.
Det finns flertaler läskiga funktioner som skulle kunna delas upp: o_get_pointer_xxx, o_move_object

## Variabler
Variabelnamnen är bra och enkla att förstå. De finns där det är naturligt.

## Loopar och villkorssatser
Alla case-satser har en default-klausul. De flesta looparna har villkor som är lätta att förstå, det finns en eller flera loopar med 't' i villkoret (som o_pointers_in_bitvector) som har mindre tydliga villkor. För några av looparna kanske man ska extrahera det som görs i looparna till funktioner (som anropas).

## Defensiv programmering
Många funktioner returnerar NULL vid "dåliga" argument, vilket är bra (vissa har istället "TODOs" där det är noterat att man bör checka dålig indata).



src/gc.c  <------------------ TODO
========

## Struktur

## Variabler

## Loopar och villkorssatser

## Defensiv programmering


src/list.c  <------------------ TODO
========

## Struktur

## Variabler

## Loopar och villkorssatser

## Defensiv programmering




Kodgranskningsprotokollet
=========================

Struktur
* Är koden välstrukturerad samt stilistiskt och formatteringsmässigt konsekvent?
* Finns det \död kod" eller metoder/rutiner som inte anropas eller behövs?
* Finns det block av (mer eller mindre) upprepad kod som kunde extraheras och kapslas in i en metod?
* Används symboler istället för "magiska nummer" eller strängkonstanter?
* Finns det klasser/metoder som är väldigt omfattande och/eller komplicerade och borde brytas upp i fler mindre klasser/metoder/rutiner?

Variabler
* Har alla variabler definierats med vettiga namn som är meningsfulla, tydliga och konsekventa?
* Initieras alla variabler med vettiga värden?
* Finns det redundanta variabler eller variabler som inte används?

Loopar och villkorssatser
* är alla loopar, villkorssatser och logiska uttryck kompletta, korrekta och korrekt nästlade?
* Testas de vanligaste fallen först i if . . . elseif . . . -kjedor?
* Har alla case-satser en default-klausul?
* är villkoren för loopars terminering uppenbara och alltid uppnåeliga?
* Initieras indexeringsvariabler korrekt innan de loopar där de används?
* Finns det uttryck och satser inuti loopar som kunde brytas ut och placeras utanför loopen?

Defensiv programmering
* Testas indexeringsvariabler och pekare korrekt mot gränsvärdena för arrayer, listor, etc.?
* Kontrolleras att indata (från filer, terminalen, etc.) är valida och komplett?
* Städas minne (alt. referenser) till allt allokerat data bort korrekt?
* Kontrolleras att filer som programmet öppnar existerar innan programmet gör åtkomst till dem?